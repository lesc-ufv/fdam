#!/bin/sh

DATA_SIZE=1048576
NUM_THREAD=8

rm -rf build $1
mkdir $1
mkdir build
cd build

cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make -j16

echo "generate graphs..."
./main "gen_dot"
echo "done!"