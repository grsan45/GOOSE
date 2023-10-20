#!/bin/bash

pushd $( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd ) >> /dev/null

SYSROOT_DIR=sysroot
INCLUDE_DIR=usr/include

LIBC_INCLUDE=libc/include/.

echo "Building binutils"
pushd ./binutils-gdb
if git apply --check ../binutils.patch ; then
  echo "Applying binutils patch..."
  git apply ../binutils.patch
else
  echo "Patch applied"
fi

popd

echo "Constructing sysroot..."
pushd .. >> /dev/null

if [ -d $SYSROOT_DIR ]; then
  echo "Existing sysroot found, deleting."
  rm -r $SYSROOT_DIR
fi

mkdir -p sysroot/usr/include
cp -R $LIBC_INCLUDE $SYSROOT_DIR/$INCLUDE_DIR

popd >> /dev/null

echo "Building binutils"

popd >> /dev/null