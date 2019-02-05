#!/bin/sh

cd cpp
rm -rf output build
mkdir output
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make

fpgaconf ../../../hw/synth/cgra_128_radix4.gbs

./main "loopback" > ../output/loopback_out.txt

./main "sobel" > ../output/sobel_out.txt

./main "kmeans" > ../output/kmeans_out.txt

./main "fir" > ../output/fir_out.txt

./main "paeth" > ../output/paeth_out.txt
