# Samples

SW C++:

```
$ source $FDAM_BASEDIR/scripts/setup.fdam
$ cd <sample/sw/cpp>
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
$ make

```
SW Java:
```
$ source $FDAM_BASEDIR/scripts/setup.fdam
$ cd <sample/sw/java>
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
$ make

```

HW Simulator: 

```
$ source $FDAM_BASEDIR/scripts/setup.fdam
$ cd <sample/hw/sim>
$ ./sim_setup
$ cd afu_sim
$ make
$ make sim
```
