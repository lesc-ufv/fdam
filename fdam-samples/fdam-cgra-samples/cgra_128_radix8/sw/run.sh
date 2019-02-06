#!/bin/sh

cd cpp
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make

echo "fpgaconf..."
fpgaconf ../../../hw/synth/cgra_128_radix8.gbs

mkdir ../$1

#echo "running loopback..."
#./main "loopback" > ../$1/loopback_out.txt

echo "running sobel..."
./main "sobel" > ../$1/sobel_out.txt

echo "running kmeans..."
./main "kmeans" > ../$1/kmeans_out.txt

echo "running fir..."
./main "fir" > ../$1/fir_out.txt

echo "running paeth..."
./main "paeth" > ../$1/paeth_out.txt
