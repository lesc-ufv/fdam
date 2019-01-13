#include <fdam/cgra/omega.h>

Omega::Omega(int size, int radix, int num_extra_stagies) : size(size), radix(radix) {

    Omega::num_stagies = Omega::intLog(size, radix) + num_extra_stagies;
    Omega::num_swicth_stages = static_cast<int>(std::ceil(size / radix));
    Omega::num_total_switch = Omega::num_stagies * Omega::num_swicth_stages;
    for (int i = 0; i < Omega::num_total_switch; ++i) {
        Omega::conf_array.emplace_back(i, Omega::radix);
    }
    for (int port_in = 0; port_in < Omega::size; ++port_in) {
        for (int port_out = 0; port_out < Omega::size; ++port_out) {
            Omega::used_route[std::pair<int, int>(port_in, port_out)] = false;
        }
    }

    Omega::createSwitchGraph();

}

Omega::~Omega() {
    Omega::conf_array.clear();
    Omega::used_route.clear();
    Omega::route_table.clear();
    Omega::graph.clear();
}

int Omega::bitRotateLeft(int val, int r_bits, int max_bits) {
    return (val << r_bits % max_bits) & ((1 << max_bits) - 1) | (
            (val & ((1 << max_bits) - 1)) >> (max_bits - (r_bits % max_bits)));
}

int Omega::bitRotateRigth(int val, int r_bits, int max_bits) {
    return ((val & ((1 << max_bits) - 1)) >> r_bits % max_bits) | (
            val << (max_bits - (r_bits % max_bits)) & ((1 << max_bits) - 1));
}

bool Omega::addRoute(int port_in, int port_out) {

    auto max_bits = static_cast<int>(std::ceil(Omega::intLog(Omega::size, 2)));
    auto r_bits = static_cast<int>(std::ceil(Omega::intLog(Omega::radix, 2)));
    int sw_src = Omega::bitRotateLeft(port_in, r_bits, max_bits) >> r_bits;
    int sw_dst = Omega::bitRotateRigth(port_out, r_bits, max_bits) >> r_bits;
    sw_dst = (Omega::num_swicth_stages * (Omega::num_stagies - 1)) + sw_dst;

    std::pair<int, int> key(port_in, port_out);

    if (Omega::used_route[key]) {
        return false;
    }

    if (Omega::route_table.find(key) == Omega::route_table.end()) {
        PathFinder::pathFinder(sw_src, sw_dst, graph, Omega::route_table[std::pair<int, int>(port_in, port_out)]);
    }
    std::vector<std::pair<int, int>> switchs_disc;

    bool path_invalid;
    for (auto path:Omega::route_table[key]) {
        path_invalid = false;
        int mask_port = (1 << intLog(radix, 2)) - 1;
        int port_in_local = Omega::bitRotateLeft(port_in, r_bits, max_bits) & mask_port;
        int port_out_global = 0;
        int port_out_local = 0;
        int sw1 = 0;
        int sw2 = 0;
        for (int j = 0; j < path.size() - 1; ++j) {
            sw1 = path[j];
            sw2 = path[j + 1];
            port_out_global = Omega::getPortOut(sw1, sw2);
            port_out_local = port_out_global & mask_port;
            if (!Omega::conf_array[sw1].connectPort(port_in_local, port_out_local)) {
                path_invalid = true;
                break;
            } else {
                switchs_disc.emplace_back(sw1, port_out_local);
            }
            port_in_local = Omega::bitRotateLeft(port_out_global, r_bits, max_bits) & mask_port;
        }
        if (!path_invalid) {
            port_out_local = Omega::bitRotateRigth(port_out, r_bits, max_bits) & mask_port;
            if (Omega::conf_array[sw2].connectPort(port_in_local, port_out_local)) {
                Omega::used_route[key] = true;
                return true;
            }
        }
        for (auto s:switchs_disc) {
            Omega::conf_array[s.first].disconnectPort(s.second);
        }
        switchs_disc.clear();

    }

    return false;
}

void Omega::removeRoute(int port_in, int port_out) {

    auto max_bits = static_cast<int>(std::ceil(Omega::intLog(Omega::size, 2)));
    auto r_bits = static_cast<int>(std::ceil(Omega::intLog(Omega::radix, 2)));
    std::pair<int, int> key(port_in, port_out);
    Omega::used_route[key] = false;

    for (auto path:Omega::route_table[key]) {
        int mask_port = (1 << intLog(radix, 2)) - 1;
        int port_out_global = 0;
        int port_out_local = 0;
        int sw1 = 0;
        int sw2 = 0;
        for (int j = 0; j < path.size() - 1; ++j) {
            sw1 = path[j];
            sw2 = path[j + 1];
            port_out_global = Omega::getPortOut(sw1, sw2);
            port_out_local = port_out_global & mask_port;
            Omega::conf_array[sw1].disconnectPort(port_out_local);
        }
        port_out_local = Omega::bitRotateRigth(port_out, r_bits, max_bits) & mask_port;
        Omega::conf_array[sw2].disconnectPort(port_out_local);
    }

}


int Omega::getPortOut(int sw1, int sw2) {
    auto max_bits = static_cast<int>(std::ceil(Omega::intLog(size, 2)));
    auto r_bits = static_cast<int>(std::ceil(Omega::intLog(radix, 2)));
    int mask_switch = (1 << Omega::intLog(Omega::num_swicth_stages, 2)) - 1;
    int sw1_local = sw1 & mask_switch;
    for (int k = 0; k < Omega::radix; ++k) {
        int sw1_port_global = sw1_local * Omega::radix + k;
        int sw2_port_global = Omega::bitRotateLeft(sw1_port_global, r_bits, max_bits);
        if ((sw2_port_global >> r_bits) == (sw2 & mask_switch)) {
            return sw1_port_global;
        }
    }

    return -1;

}

void Omega::createRouteTable() {

    auto max_bits = static_cast<int>(std::ceil(Omega::intLog(size, 2)));
    auto r_bits = static_cast<int>(std::ceil(Omega::intLog(radix, 2)));

#pragma omp parallel
    {
#pragma omp for collapse(2)
        for (int port_in = 0; port_in < Omega::size; ++port_in) {
            for (int port_out = 0; port_out < Omega::size; ++port_out) {
                int sw_src = Omega::bitRotateLeft(port_in, r_bits, max_bits) >> r_bits;
                int sw_dst = Omega::bitRotateRigth(port_out, r_bits, max_bits) >> r_bits;
                sw_dst = (Omega::num_swicth_stages * (Omega::num_stagies - 1)) + sw_dst;
                PathFinder::pathFinder(sw_src, sw_dst, graph,
                                       Omega::route_table[std::pair<int, int>(port_in, port_out)]);
            }
        }
    };
}

void Omega::printRouteTable() {
    printf("ROUTE TABLE REDE OMEGA %d x %d radix %d\n\n", Omega::size, Omega::size, Omega::radix);
    printf("PORT PE_IN;PORT PE_OUT;");
    for (int i = 0; i < Omega::num_stagies; ++i) {
        printf("SW STAGE %d;", i);
    }
    printf("\n");
    for (auto p: Omega::route_table) {
        for (auto r:p.second) {
            printf("%d;%d;", p.first.first, p.first.second);
            for (auto s:r) {
                printf("%d;", s);
            }
            printf("\n");
        }
    }
}

void Omega::createSwitchGraph() {
    auto max_bits = static_cast<int>(std::ceil(Omega::intLog(size, 2)));
    auto r_bits = static_cast<int>(std::ceil(Omega::intLog(radix, 2)));

    for (int j = 0; j < Omega::num_stagies - 1; ++j) {
        for (int l = 0; l < Omega::num_swicth_stages; ++l) {
            for (int k = 0; k < Omega::radix; ++k) {
                int v1 = j * Omega::num_swicth_stages + l;
                int v2 = Omega::bitRotateLeft(l * Omega::radix + k, r_bits, max_bits) >> r_bits;
                v2 = (j + 1) * Omega::num_swicth_stages + v2;
                graph[v1].push_back(v2);
            }
        }
    }
}

void Omega::reset() {
    for (int port_in = 0; port_in < Omega::size; ++port_in) {
        for (int port_out = 0; port_out < Omega::size; ++port_out) {
            std::pair<int, int> key(port_in, port_out);
            Omega::used_route[key] = false;
        }
    }
    for (int i = 0; i < Omega::num_total_switch; ++i) {
        Omega::conf_array[i].reset();
    }
}

const std::vector<Switch> &Omega::getConfArray() const {
    return conf_array;
}

int Omega::intLog(double x, double base) {
    return (int) std::ceil(log(x) / log(base));
}

int Omega::getNumSwicthStages() const {
    return num_swicth_stages;
}

int Omega::getNumStages() {
    return Omega::num_stagies;
}

