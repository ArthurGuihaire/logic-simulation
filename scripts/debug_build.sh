#!/bin/bash
./delete_cmake_files.sh

#flags to add for address sanitizer

#  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" \
#  -DCMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" \

cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  ..

make -j"$(nproc)"
