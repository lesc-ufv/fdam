# FDAM: Framework for Dataflow Accelerators Management  

This repository we propose a novel framework to automatically integrate hardware accelerator cores in a final architecture by generating a HW/SW interface for an Intel CPU-FPGA modern platform. We have developed a software API in C++ to be used similarly to CUDA language in  high performance GPU accelerators.

Our goal is to simplify the Intel Open Programmable Acceleration Engine (OPAE) by introducing a novel abstraction layer with a simple stream protocol channel. We've built a energy efficient and fast implementation, capable of peforming 131.7 Gops/s and a power efficiency of up to 353.7 Gops/W even when we bound the memory bandwidth to 12 GB/s.

To perform the installation see the project's [Wiki](https://github.com/ComputerArchitectureUFV/fdam/wiki).
  
# Publication
  
Lucas Bragança, Fredy Alves, Jeronimo Costa Penha, Gabriel Coimbra, Ricardo Ferreira, José Augusto M Nacif, [Simplifying HW/SW integration to deploy multiple accelerators for CPU-FPGA heterogeneous platforms](https://dl.acm.org/doi/pdf/10.1145/3229631.3229651) in Proceedings of the 18th International Conference on Embedded Computer Systems: Architectures, Modeling, and Simulation.
  
If you find FDAM useful or if you use it in your work, please cite the following paper:
  
  ```
@inproceedings{fdam,
author = {Bragan\c{c}a, Lucas and Alves, Fredy and Penha, Jeronimo Costa and Coimbra, Gabriel and Ferreira, Ricardo and Nacif, Jos\'{e} Augusto M.},
title = {Simplifying HW/SW Integration to Deploy Multiple Accelerators for CPU-FPGA Heterogeneous Platforms},
year = {2018},
isbn = {9781450364942},
publisher = {Association for Computing Machinery},
address = {New York, NY, USA},
url = {https://doi.org/10.1145/3229631.3229651},
doi = {10.1145/3229631.3229651},
booktitle = {Proceedings of the 18th International Conference on Embedded Computer Systems: Architectures, Modeling, and Simulation},
pages = {97–104},
numpages = {8},
location = {Pythagorion, Greece},
series = {SAMOS ’18}
}

```


  
