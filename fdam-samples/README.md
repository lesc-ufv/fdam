# Samples

SW C++:

```
$ source fdam/scripts/setup.fdam
$ cd <sample/sw/cpp>
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
$ make

```
SW Java:
```
$ source fdam/scripts/setup.fdam
$ cd <sample/sw/java>
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
$ make

```

HW Simulator: 

```
$ source fdam/scripts/setup.fdam
$ cd <sample/hw/sim>
$ ./sim_setup
$ cd afu_sim
$ make
$ make sim
```
