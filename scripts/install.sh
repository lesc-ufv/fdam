#!/usr/bin/env bash

CALLPATH=`pwd`
FULLPATH="${BASH_SOURCE[@]}"
FULLPATH=${FULLPATH%/*}
if [[ -d $FULLPATH ]] 
then
    cd $FULLPATH
fi
MYPATH=`pwd`
MYPATH=$MYPATH/..

echo "cleaning up..."
echo ""
rm -rf $MYPATH/installation
rm -rf $MYPATH/opae-sdk/mybuild
rm -rf $MYPATH/intel-fpga-bbb/mybuild
rm -rf $MYPATH/fam-sw/fam-cpp/mybuild
rm -rf $MYPATH/fam-sw/fam-java/mybuild
echo "end of cleaning up!"
echo ""

INSTALL_DIR=$MYPATH/installation
mkdir $INSTALL_DIR

echo "installing opae-sdk ..."
echo ""
mkdir $MYPATH/opae-sdk/mybuild
cd  $MYPATH/opae-sdk/mybuild
cmake .. -DBUILD_ASE=1 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make -j7
make install
echo "end of installing opae-sdk"
echo ""

echo "installing intel-fpga-bbb..."
echo ""
mkdir $MYPATH/intel-fpga-bbb/mybuild
cd $MYPATH/intel-fpga-bbb/mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make -j7
make install 
echo "end of installing intel-fpga-bbb"
echo ""

echo "installing fam-cpp..."
echo ""
mkdir $MYPATH/fam-sw/fam-cpp/mybuild
cd $MYPATH/fam-sw/fam-cpp/mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make -j7
make install 
echo "end of installing fam-cpp"
echo ""

echo "installing fam-java..."
echo ""
mkdir $MYPATH/fam-sw/fam-java/mybuild
cd $MYPATH/fam-sw/fam-java/mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make -j7
make install 
echo "end of installing fam-java"
echo ""

cd $CALLPATH
