#!/bin/sh

DATA_SIZE=1048576
NUM_THREAD=8

rm -rf build $1
mkdir $1
mkdir build
cd build

cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make -j16

echo "fpgaconf..."
fpgaconf ../../../hw/synth/cgra_128_radix8.gbs

echo "running loopback..."
./main "loopback" > ../$1/loopback_out.txt $NUM_THREAD $DATA_SIZE

echo "running fir..."
./main "fir" > ../$1/fir_out.txt $NUM_THREAD $DATA_SIZE

echo "running kmeans..."
./main "kmeans" > ../$1/kmeans_out.txt $NUM_THREAD $DATA_SIZE

echo "running paeth..."
./main "paeth" > ../$1/paeth_out.txt $NUM_THREAD $DATA_SIZE

echo "running loopback..."
./main "loopback" > ../$1/loopback_out.txt $NUM_THREAD $DATA_SIZE

echo "running sobel..."
./main "sobel" > ../$1/sobel_out.txt $NUM_THREAD 1333 1333

echo "running chebyshev..."
./main "chebyshev" > ../$1/chebyshev.txt $NUM_THREAD $DATA_SIZE

echo "running mibench..."
./main "mibench" > ../$1/mibench.txt $NUM_THREAD $DATA_SIZE

echo "running qspline..."
./main "qspline" > ../$1/qspline.txt $NUM_THREAD $DATA_SIZE

echo "running sgfilter..."
./main "sgfilter" > ../$1/sgfilter.txt $NUM_THREAD $DATA_SIZE

echo "running poly5..."
./main "poly5" > ../$1/poly5.txt $NUM_THREAD $DATA_SIZE

echo "running poly6..."
./main "poly6" > ../$1/poly6.txt $NUM_THREAD $DATA_SIZE

echo "running poly8..."
./main "poly8" > ../$1/poly8.txt $NUM_THREAD $DATA_SIZE
