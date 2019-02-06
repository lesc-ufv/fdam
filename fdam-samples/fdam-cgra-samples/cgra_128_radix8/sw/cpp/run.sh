#!/bin/sh


rm -rf build $1
mkdir $1
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make

echo "fpgaconf..."
fpgaconf ../../../hw/synth/cgra_128_radix8.gbs

#echo "running loopback..."
#./main "loopback" > $1/loopback_out.txt 8 1000000

echo "running sobel..."
./main "sobel" > $1/sobel_out.txt 8 10200 10200

echo "running kmeans..."
./main "kmeans" > $1/kmeans_out.txt 8 134217728

echo "running fir..."
./main "fir" > $1/fir_out.txt 8 134217728

echo "running paeth..."
./main "paeth" > $1/paeth_out.txt 8 134217728
