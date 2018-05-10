#!/usr/bin/env bash

FULLPATH="`pwd`/${BASH_SOURCE[@]}"
MYPATH=${FULLPATH%/*}

echo "cleaning up..."
echo ""
cd $MYPATH/..
rm -rf installation
cd $MYPATH/..
cd opae-sdk
rm -rf mybuild
cd $MYPATH/..
cd intel-fpga-bbb
rm -rf mybuild
cd $MYPATH/..
cd fam-sw
rm -rf mybuild
echo "end of cleaning up!"
echo ""

cd $MYPATH/..
mkdir installation
cd installation
INSTALL_DIR=`pwd`

echo "installing opae-sdk ..."
echo ""
cd $MYPATH/..
cd opae-sdk
mkdir mybuild
cd mybuild
cmake .. -DBUILD_ASE=1 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make
make install
echo "end of installing opae-sdk"
echo ""

echo "installing intel-fpga-bbb..."
echo ""
cd $MYPATH/..
cd intel-fpga-bbb
mkdir mybuild
cd mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make
make install 
echo "end of installing intel-fpga-bbb"
echo ""

echo "installing fam..."
echo ""
cd $MYPATH/..
cd fam-sw
mkdir mybuild
cd mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make
make install 
echo "end of installing fam"
echo ""
