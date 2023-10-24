#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

pushd $SCRIPT_DIR >> /dev/null

SYSROOT_DIR=$SCRIPT_DIR/sysroot
INCLUDE_DIR=usr/include

LIBC_INCLUDE=$SCRIPT_DIR/../libc/include/.

TARGET=i686-goose
PREFIX=$SCRIPT_DIR/$TARGET

echo "Constructing sysroot..."

if [ -d $SYSROOT_DIR ]; then
  echo "Existing sysroot found, deleting."
  rm -r $SYSROOT_DIR
fi

mkdir -p $SYSROOT_DIR/$INCLUDE_DIR
cp -R $LIBC_INCLUDE $SYSROOT_DIR/$INCLUDE_DIR

echo "Setting up binutils build"
pushd ./binutils-gdb
if git apply --check ../binutils.patch ; then
  echo "Applying binutils patch..."
  git apply ../binutils.patch
else
  echo "Patch applied"
fi
popd >> /dev/null

echo "Building binutils"

pushd $SCRIPT_DIR/binutils-gdb >> /dev/null
autoreconf --force --install
popd >> /dev/null

mkdir -p build-binutils
pushd $SCRIPT_DIR/build-binutils >> /dev/null
../binutils-gdb/configure --target=i686-goose --prefix="$PREFIX" --with-sysroot=$SYSROOT_DIR --disable-werror
make -j$(nproc) && make install
popd >> /dev/null

echo "Binutils build complete."

echo "Setting up gcc build"
pushd ./gcc
if git apply --check ../gcc.patch ; then
  echo "Applying gcc patch..."
  git apply ../gcc.patch
else
  echo "Patch applied"
fi
popd >> /dev/null

echo "Starting GCC Build"

pushd $SCRIPT_DIR/gcc >> /dev/null
autoreconf --force --install
popd >> /dev/null

mkdir -p $SCRIPT_DIR/build-gcc
pushd $SCRIPT_DIR/build-gcc >> /dev/null
$SCRIPT_DIR/gcc/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot=$SYSROOT_DIR --enable-languages=c
echo "GCC configured"
make -j$(nproc) all-gcc all-target-libgcc && make -j$(nproc) install-gcc install-target-libgcc
popd >> /dev/null

popd >> /dev/null