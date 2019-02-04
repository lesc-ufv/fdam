#!/bin/sh

cd cpp
rm -rf output build
mkdir output
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make
cd ../..

fpgaconf ../hw/synth/cgra_128_radix8.gbs

./cpp/build/main "loopback" > cpp/output/loopback_out.txt

./cpp/build/main "sobel" > cpp/output/sobel_out.txt

./cpp/build/main "kmeans" > cpp/output/kmeans_out.txt

./cpp/build/main "fir" > cpp/output/fir_out.txt

./cpp/build/main "paeth" > cpp/output/paeth_out.txt
