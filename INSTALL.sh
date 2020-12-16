#!/bin/sh
mkdir build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. build/
make -C build/
ln batch.sh build/batch.sh






