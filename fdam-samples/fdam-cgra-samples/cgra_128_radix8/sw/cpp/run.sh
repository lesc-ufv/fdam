#!/bin/sh

DATA_SIZE=8388608
NUM_THREAD=1

rm -rf build $1
mkdir $1
mkdir build
cd build

cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make -j16

echo "fpgaconf..."
fpgaconf ../../../hw/synth/cgra_128_radix8.gbs

echo "running paeth..."
./main "paeth" > ../$1/paeth_out.txt $NUM_THREAD $DATA_SIZE

echo "running loopback..."
./main "loopback" > ../$1/loopback_out.txt $NUM_THREAD $DATA_SIZE

echo "running kmeans..."
./main "kmeans" > ../$1/kmeans_out.txt $NUM_THREAD $DATA_SIZE

echo "running fir..."
./main "fir" > ../$1/fir_out.txt $NUM_THREAD $DATA_SIZE

echo "running sobel..."
./main "sobel" > ../$1/sobel_out.txt $NUM_THREAD 1333 1333

