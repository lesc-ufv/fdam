#!/bin/sh

rm -rf build
mkdir build
cd build

cmake .. -DCMAKE_PREFIX_PATH=$FDAM_INSTALLDIR
make -j16

echo "generate graphs..."
./main "gen_dot"
echo "done!"

cd ..

for entry in "dot_dataflows"/*.dot
do
  dot -Tpdf $entry -o ${entry//.dot/.pdf}
  echo  ${entry//.dot/.pdf}
done
