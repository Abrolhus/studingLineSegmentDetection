#!/bin/sh
mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cd ..
make -C build/
ln batch.sh build/batch.sh






