#!/bin/bash

pushd ../toolchain
./build-toolchain.sh
popd

mkdir -p ../build >> /dev/null
pushd ../build >> /dev/null

cmake ..
make all -j$(nproc)

popd

./create_iso.sh
DISPLAY=172.26.0.1:0 qemu-system-i386 -s -S -serial stdio -m 4G -cdrom goose.iso
