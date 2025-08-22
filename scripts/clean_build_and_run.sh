#!/bin/bash
./delete_cmake_files.sh
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make -j$(nproc)
./logicSim
