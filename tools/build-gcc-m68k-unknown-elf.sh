#!/bin/bash

set -e

CROSSTOOL_PATH="./crosstool-ng"
BUILD_PATH="./m68k-unknown-elf"

if [ -f "$CROSSTOOL_PATH/ct-ng" ]; then
  echo "crosstool-ng already exist"
else
  git clone https://github.com/crosstool-ng/crosstool-ng
  pushd crosstool-ng
  ./bootstrap
  ./configure --enable-local
  make -j$(nproc)
  popd
fi

if [ -d "$BUILD_PATH" ]; then
  echo "$BUILD_PATH already exist"
  exit 1
fi

CT_NG="$(realpath $CROSSTOOL_PATH/ct-ng)"

mkdir -p "$BUILD_PATH"
pushd "$BUILD_PATH"
"$CT_NG" m68k-unknown-elf
"$CT_NG" build
popd
