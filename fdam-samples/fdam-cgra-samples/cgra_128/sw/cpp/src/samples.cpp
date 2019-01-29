//
// Created by lucas on 13/01/19.
//

#include <samples.h>

DataFlow *Samples::loopback(int id, int pipe, int copies) {

    char name[20];
    sprintf(name, "%s_%d_%d", "loopback", pipe, copies);
    DataFlow *df = new DataFlow(id, name);
    int idx = 0;
    for (int j = 0; j < copies; ++j) {
        auto in = new Operator(idx++, OP_PASS_A, OP_IN);
        auto out = new Operator(idx++, OP_PASS_A, OP_OUT);
        for (int i = 0; i < pipe - 1; ++i) {
            auto r = new Operator(idx++, OP_PASS_A, OP_BASIC);
            df->connect(in, r, PORT_A);
            in = r;
        }
        df->connect(in, out, PORT_A);
    }

    return df;
}

DataFlow *Samples::fir(int id, short **coef, int n, int copies) {
    auto df = new DataFlow(id, "fir");
    int idx = 0;
    std::vector<Operator *> in_cp;
    std::vector<Operator *> out_cp;

    for (int j = 0; j < copies; ++j){
        in_cp.push_back(new InputStream(idx++));
    }
    for (int j = 0; j < copies; ++j){
        out_cp.push_back(new OutputStream(idx++));
    }
    for (int j = 0; j < copies; ++j) {
        Operator *op, *op1, *op2;
        std::vector<Operator *> add;
        add.reserve((unsigned long) n - 1);
        for (int i = 0; i < n; ++i) {
            auto m = new Multi(idx++, coef[j][n - i - 1]);
            if (i == 0) {
                op = new PassA(idx++);
            } else {
                op = new Add(idx++);
            }
            add.push_back(op);
            df->connect(in_cp[j], m, PORT_A);
            df->connect(m, op, PORT_A);
        }
        for (int i = 0; i < n - 1; ++i) {
            op1 = add[i];
            op2 = add[i + 1];
            df->connect(op1, op2, PORT_B);
        }
        op1 = add[n - 1];
        df->connect(op1,out_cp[j], PORT_A);
    }

    return df;
}

DataFlow *Samples::vetorialSum(int id, int copies) {

    auto df = new DataFlow(id, "VetorialSum");
    int idx = 0;
    for (int i = 0; i < copies; ++i) {
        auto in1 = new InputStream(idx++);
        auto in2 = new InputStream(idx++);
        auto out = new OutputStream(idx++);
        auto add = new Add(idx++);

        df->connect(in1, add, add->getPortA());
        df->connect(in2, add, add->getPortB());
        df->connect(add, out, out->getPortA());
    }

    return df;
}

DataFlow *Samples::kmeans(int id, int k, int d) {

    auto df = new DataFlow(id, "kmeans");
    int idx = 0;
    std::vector<Operator *> inputs;
    std::map<int, std::vector<Operator *>> subs;
    std::map<int, std::vector<Operator *>> abs;
    std::vector<Operator *> adds_end;
    std::vector<Operator *> aux;
    std::vector<Operator *> aux_reduz;
    std::vector<Operator *> slt_reduz;
    std::vector<Operator *> mux_reduz;
    std::vector<Operator *> constants;
    int outId;

    if (k > 1) {
        for (int j = 0; j < k; ++j) {
            for (int i = 0; i < d; ++i) {
                auto s = new Subi(idx++, (int) random() % 255);
                subs[j].push_back(s);
            }
        }
        inputs.reserve(static_cast<unsigned long>(d));
        for (int i = 0; i < d; ++i) {
            inputs.push_back(new InputStream(idx++));
        }
        outId = idx++;
        for (int j = 0; j < k; ++j) {
            for (int i = 0; i < d; ++i) {
                df->connect(inputs[i], subs[j][i], PORT_A);
            }
            for (auto s:subs[j]) {
                auto a = new Abs(idx++);
                df->connect(s, a, PORT_A);
                abs[j].push_back(a);
            }
            aux.clear();
            aux_reduz.clear();
            for (auto a:abs[j]) {
                aux.push_back(a);
            }
            while (aux.size() > 1) {
                int start = 0;
                if (aux.size() % 2 != 0) {
                    auto passA = new PassA(idx++);
                    df->connect(aux[0], passA, PORT_A);
                    aux_reduz.push_back(passA);
                    start = 1;
                }
                for (int l = start; l < aux.size(); l += 2) {
                    auto add = new Add(idx++);
                    df->connect(aux[l], add, PORT_A);
                    df->connect(aux[l + 1], add, PORT_B);
                    aux_reduz.push_back(add);
                }
                aux.clear();
                for (auto a:aux_reduz) {
                    aux.push_back(a);
                }
                aux_reduz.clear();
            }
            for (auto a:aux) {
                adds_end.push_back(a);
            }
        }
        aux.clear();
        aux_reduz.clear();
        for (auto a:adds_end) {
            aux.push_back(a);
        }
        while (aux.size() > 1) {
            int offset = 0;
            if (aux.size() % 2 != 0) {
                auto passA = new PassA(idx++);
                df->connect(aux[aux.size() - 1], passA, PORT_A);
                aux_reduz.push_back(passA);
                offset = 1;
            }
            for (int l = 0; l < aux.size() - offset; l += 2) {
                auto slt = new Slt(idx++);
                df->connect(aux[l], slt, PORT_B);
                df->connect(aux[l + 1], slt, PORT_A);
                aux_reduz.push_back(slt);
                slt_reduz.push_back(slt);
            }
            aux.clear();
            for (auto a:aux_reduz) {
                aux.push_back(a);
            }
            aux_reduz.clear();
        }

        for (auto &m : slt_reduz) {
            auto mux = new Mux(idx++);
            df->connect(m, mux, PORT_BRANCH);
            mux_reduz.push_back(mux);
        }
        for (int n = 0; n < mux_reduz.size() - 1; ++n) {
            df->connect(mux_reduz[n], mux_reduz[n + 1], PORT_A);
        }
        for (int i1 = 0; i1 < k; ++i1) {
            auto c = new PassBi(idx++, i1);
            if (i1 == 0) {
                df->connect(c, mux_reduz[0], PORT_A);
            } else {
                df->connect(c, mux_reduz[i1 - 1], PORT_B);
            }
        }
        auto out = new OutputStream(outId);
        df->connect(mux_reduz[mux_reduz.size() - 1], out, PORT_A);
    } else {
        auto in = new InputStream(idx++);
        auto out = new OutputStream(idx++);
        auto reg = new PassBi(idx, 0);
        df->connect(in, out, PORT_B);
        df->connect(reg, out, PORT_A);
    }

    return df;
}

DataFlow *Samples::sobelFilter() {
    auto df = new DataFlow(0, "sobel_filter");
    int idx = 0;
    Operator *inputs[9];
    Operator *output[2];
    std::vector<Operator *> aux0;
    std::vector<Operator *> aux1;
    std::vector<Operator *> adds;

    int gx_gy[2][9] = {{1,  2, 1, 0,  0, 0, -1, -2, -1},
                       {-1, 0, 1, -2, 0, 2, -1, 0,  1}};


    for (int i = 0; i < 9; ++i) {
        if (i != 4)
            inputs[i] = new InputStream(idx++);
    }
    output[0] = new OutputStream(idx++);
    //output[1] = new output_stream(idx++);

    for (int i = 0; i < 9; ++i) {
        if (i == 4) {
            auto r = new PassBi(idx++, 0);
            inputs[i] = r;
        } else {
            auto r = new PassB(idx++);
            df->connect(inputs[i], r, PORT_B);
            inputs[i] = r;
        }
    }

    for (int l = 0; l < 2; ++l) {
        aux0.clear();
        aux1.clear();
        for (int j = 0; j < 9; ++j) {
            auto mul = new Multi(idx++, gx_gy[l][9 - j - 1]);
            df->connect(inputs[j], mul, PORT_A);
            aux0.push_back(mul);
        }
        while (aux0.size() > 1) {
            int r = 0;
            if (aux0.size() % 2 != 0) {
                auto reg = new PassA(idx++);
                df->connect(aux0[aux0.size() - 1], reg, PORT_A);
                aux1.push_back(reg);
                r = 1;
            }
            for (int k = 0; k < aux0.size() - r; k += 2) {
                auto add = new Add(idx++);
                df->connect(aux0[k], add, PORT_A);
                df->connect(aux0[k + 1], add, PORT_B);
                aux1.push_back(add);
            }
            aux0.clear();
            for (auto a:aux1) {
                aux0.push_back(a);
            }
            aux1.clear();
        }
        adds.push_back(aux0[0]);
    }
    auto mult1 = new Mult(idx++);
    auto mult2 = new Mult(idx++);
    auto add = new Add(idx++);

    df->connect(adds[0], mult1, PORT_A);
    df->connect(adds[0], mult1, PORT_B);
    df->connect(adds[1], mult2, PORT_A);
    df->connect(adds[1], mult2, PORT_B);
    df->connect(mult1, add, PORT_A);
    df->connect(mult2, add, PORT_B);
    df->connect(add, output[0], PORT_A);

    return df;
}

DataFlow *Samples::test() {
    auto df = new DataFlow(0, "test");
    auto in = new InputStream(0);
    auto out = new OutputStream(1);
    auto max = new Maxi(2, 0xfffffffe);

    df->connect(in, max, PORT_A);
    df->connect(max, out, PORT_A);

    return df;
}
