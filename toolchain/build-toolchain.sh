#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

pushd $SCRIPT_DIR >> /dev/null

SYSROOT_DIR=$SCRIPT_DIR/sysroot
INCLUDE_DIR=usr/include

LIBC_DIR=$SCRIPT_DIR/../libc/.
LIBC_INCLUDE=$LIBC_DIR/include/.

TARGET=i686-goose
PREFIX=$SCRIPT_DIR/$TARGET

REBUILD=0

while getopts 'r' opt; do
  case "$opt" in
    r)
      echo "Rebuilding..."
      REBUILD=1
      ;;
    *)
      ;;
  esac
done

echo "Constructing sysroot..."

if [ -d $SYSROOT_DIR ]; then
  echo "Existing sysroot found, deleting."
  rm -r $SYSROOT_DIR
fi

mkdir -p $SYSROOT_DIR/$INCLUDE_DIR
cp -R $LIBC_INCLUDE $SYSROOT_DIR/$INCLUDE_DIR

echo "Running codegen on sysroot dir"
python3 -m codegen $SYSROOT_DIR

if [[ ! -f $PREFIX/bin/i686-goose-gcc || $REBUILD == 1 ]] ; then
  echo "Setting up binutils build"
  if git apply --check ./binutils.patch ; then
    echo "Applying binutils patch..."
    git apply ./binutils.patch
  else
    echo "Patch already applied"
  fi

  echo "Building binutils"

  pushd $SCRIPT_DIR/binutils-gdb >> /dev/null
  autoreconf --force --install
  popd >> /dev/null

  mkdir -p build-binutils
  pushd $SCRIPT_DIR/build-binutils >> /dev/null
  ../binutils-gdb/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot=$SYSROOT_DIR --disable-werror
  make -j$(nproc) && make install
  popd >> /dev/null

  if [ ! -d $PREFIX ]; then
      echo "Binutils build failed. Check logs. Exiting"
      exit 2
  else
    echo "Binutils build complete."
  fi

  echo "Setting up gcc build"
  if git apply --check ./gcc.patch ; then
    echo "Applying gcc patch..."
    git apply ./gcc.patch
  else
    echo "Patch already applied"
  fi

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

  echo "GCC build complete."

  popd >> /dev/null

fi

echo "Building libc..."

mkdir -p $LIBC_DIR/build
pushd $LIBC_DIR/build >> /dev/null

cmake ..
make -j$(nproc) all

mkdir -p $SYSROOT_DIR/usr/lib
cp libc.a $SYSROOT_DIR/usr/lib

# this is dirty but necessary (for now)
pushd CMakeFiles >> /dev/null

cp c.dir/crt0.s.o $SYSROOT_DIR/usr/lib/crt0.o
cp c.dir/crti.s.o $SYSROOT_DIR/usr/lib/crti.o
cp c.dir/crtn.s.o $SYSROOT_DIR/usr/lib/crtn.o

popd >> /dev/null

popd >> /dev/null
