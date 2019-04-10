#!/bin/sh

rm -rf build $1
mkdir $1
mkdir build
cd build

cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make -j16

echo "generate graphs..."
./main "gen_dot"
echo "done!"