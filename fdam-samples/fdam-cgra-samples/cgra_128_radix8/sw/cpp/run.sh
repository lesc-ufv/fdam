#!/usr/bin/env bash

#for simulation type = ase
#for harp type = harp
#for test, one bit to each test

type="_"$1
do_build=$2
tests=$3

bitstream_cgra="$FDAM_BASEDIR/fdam-samples/fdam-cgra-samples/cgra_128_radix8/hw/synth/cgra_128_radix8.gbs"

if test "$do_build" == "exec_harp"
then 
   fpgaconf $bitstream_cgra
fi

cd Chebyshev/
if test "$do_build" == "build"
then
    echo "Build Chebyshev ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Chebyshev ..."
    cd build
    ./chebyshev$type $tests
fi

cd ../../
cd Fir
if test "$do_build" == "build"
then
    echo "Build Fir ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Fir ..."
    cd build
    ./fir$type $tests
fi

cd ../../
cd Kmeans
if test "$do_build" == "build"
then
    echo "Build Kmeans ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Kmeans ..."
    cd build
    ./kmeans$type $tests
fi

cd ../../
cd Mibench
if test "$do_build" == "build"
then
    echo "Build Mibench ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Mibench ..."
    cd build
    ./mibench$type $tests
fi

cd ../../
cd Paeth
if test "$do_build" == "build"
then
    echo "Build Paeth ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Paeth ..."
    cd build
    ./paeth$type $tests
fi

cd ../../
cd Poly5
if test "$do_build" == "build"
then
    echo "Build Poly5 ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Poly5 ..."
    cd build
    ./poly5$type $tests
fi

cd ../../
cd Poly6
if test "$do_build" == "build"
then
    echo "Build Poly6 ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Poly6 ..."
    cd build
    ./poly6$type $tests
fi

cd ../../
cd Poly8
if test "$do_build" == "build"
then
    echo "Build Poly8 ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Poly8 ..."
    cd build
    ./poly8$type $tests
fi

cd ../../
cd Qspline
if test "$do_build" == "build"
then
    echo "Build Qspline ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Qspline ..."
    cd build
    ./qspline$type $tests
fi

cd ../../
cd Sgfilter
if test "$do_build" == "build"
then
    echo "Build Sgfilter ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run Sgfilter ..."
    cd build
    ./sgfilter$type $tests
fi

cd ../../
cd SobelFilter
if test "$do_build" == "build"
then
    echo "Build SobelFilter ..."
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    echo "Run SobelFilter ..."
    cd build
    ./sobel_filter$type $tests
fi
