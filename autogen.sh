#!/bin/bash

if [ ! -d "./i686-elf-cc/" ]; then
  echo "i686-elf-cc directory not present, exiting"
  exit 1
fi

if ! which autoreconf >> /dev/null; then
  echo "Autotools not installed, exiting"
  exit 1
else
  echo "Autotools installed, generating build files"
  autoreconf --install --force --verbose

  mkdir -p build
  cd build
  ../configure --build x86_64-linux-gnu --host i686-goose
  echo "Build files generated, run \"make all\" from the build directory"
  cd ..
fi