#!/usr/bin/env bash

set -e

echo '----------------------------------------------------------------------------'
echo 'Welcome to FDAM Setup Wizard.'
echo ''
echo '----------------------------------------------------------------------------'

CALLPATH=`pwd`
FULLPATH="${BASH_SOURCE[@]}"
FULLPATH=${FULLPATH%/*}
if [[ -d $FULLPATH ]] 
then
    cd $FULLPATH
fi
MYPATH=`pwd`
MYPATH=$MYPATH/..

# read command line args
SUDO=''
INSTALL_DIR='/usr/local/'
JOBS='-j2'

for i in "$@"
do
case $i in
  -h|--help)
  echo ''
  echo 'Usage: sh install.sh [OPTION]'
  echo ''
  echo '-h  , --help         show arguments'
  echo '      --prefix=PATH  override default install location'
  echo '      --install-as-root  root user is necessary to install the applications'
  echo '-j=N, --jobs=N       specifies the number of jobs to run simultaneously'
  exit 0
  shift # past argument=value
  ;;
  --prefix=*)
  INSTALL_DIR="${i#*=}"
  shift # past argument=value
  ;;
  --install-as-root)
  SUDO='sudo '
  shift # past argument with no value
  ;;
  -j=*|--jobs=*)
  JOBS="-j${i#*=}"
  shift # past argument=value
  ;;
  *)
  # unknown option
  echo ''
  echo 'install.sh: invalid option!'
  echo ''
  echo "Try: 'bash install.sh --help' for more information."
  exit 1
  ;;
esac
done

cd $INSTALL_DIR
INSTALL_DIR=`pwd`
cd $CALLPATH

echo ''
echo 'checking libraries ...'
echo ''

check_lib() {
  LIB_FILE=$1.'so'
  CHECK=$(ldconfig -p | grep ${LIB_FILE})

  if [[ ${CHECK} != *${LIB_FILE}* ]]
  then
    tput setaf 1; echo 'Error: '${LIB_FILE}' is not installed!'
    exit 1
  fi
}

check_lib libuuid
check_lib libjson-c

echo "cleaning up..."
echo ""
rm -rf $MYPATH/opae-sdk/mybuild
rm -rf $MYPATH/intel-fpga-bbb/mybuild
rm -rf $MYPATH/sw/cpp/mybuild
rm -rf $MYPATH/sw/java/mybuild
echo "end of cleaning up!"
echo ""

echo "installing opae-sdk ..."
echo ""
mkdir $MYPATH/opae-sdk/mybuild
cd  $MYPATH/opae-sdk/mybuild
cmake .. -DBUILD_ASE=1 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make $JOBS
$SUDO make install
echo "end of installing opae-sdk"
echo ""

echo "installing intel-fpga-bbb..."
echo ""
mkdir $MYPATH/intel-fpga-bbb/mybuild
cd $MYPATH/intel-fpga-bbb/mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make $JOBS
$SUDO make install
echo "end of installing intel-fpga-bbb"
echo ""

echo "installing fdam-cpp..."
echo ""
mkdir $MYPATH/sw/cpp/mybuild
cd $MYPATH/sw/cpp/mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make $JOBS
$SUDO make install
echo "end of installing fdam-cpp"
echo ""

echo "installing fdam-java..."
echo ""
mkdir $MYPATH/sw/java/mybuild
cd $MYPATH/sw/java/mybuild
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make $JOBS
$SUDO make install 
echo "end of installing fdam-java"
echo ""

echo "Installation completed successfully!"
echo ""

cd $CALLPATH
