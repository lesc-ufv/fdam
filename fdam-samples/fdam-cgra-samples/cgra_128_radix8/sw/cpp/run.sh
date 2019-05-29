#!/usr/bin/env bash

#for simulation type = ase
#for harp type = harp

type="_"$1
do_build=$2

echo "Run Chebyshev ..."
cd Chebyshev/
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./chebyshev$type
fi

cd ../../
echo "Run Fir ..."
cd Fir
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./fir$type
fi

cd ../../
echo "Run Kmeans ..."
cd Kmeans
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./kmeans$type
fi

cd ../../
echo "Run Mibench ..."
cd Mibench
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./mibench$type
fi

cd ../../
echo "Run Paeth ..."
cd Paeth
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./paeth$type
fi

cd ../../
echo "Run Poly5 ..."
cd Poly5
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./poly5$type
fi

cd ../../
echo "Run Poly6 ..."
cd Poly6
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./poly6$type
fi

cd ../../
echo "Run Poly8 ..."
cd Poly8
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./poly8$type
fi

cd ../../
echo "Run Qspline ..."
cd Qspline
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./qspline$type
fi

cd ../../
echo "Run Sgfilter ..."
cd Sgfilter
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./sgfilter$type
fi

cd ../../
echo "Run SobelFilter ..."
cd SobelFilter
if test "$do_build" == "build"
then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
    make -j7
else
    cd build
    ./sobel_filter$type
fi
