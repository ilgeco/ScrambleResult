#!/usr/bin/env bash
set -euo pipefail

if (( $# > 1 )); then
    printf 'usage: %s [half]\n' "$0" >&2
    exit 2
fi

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd -- "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

CLANG="${CLANG:-clang}"
AR="${AR:-llvm-ar}"
MCPU="${MCPU:-cortex-m3}"
MFPU="${MFPU:-soft}"
TARGET="${TARGET:-arm-none-eabi}"
SYSROOT="${SYSROOT:-}"

CROSS_OPT=(
    -nostdlib
    -nodefaultlibs
    -funroll-loops
    "-mcpu=$MCPU"
    "-mfloat-abi=$MFPU"
    "--target=$TARGET"
    -Wall
    -fdata-sections
    -ffunction-sections
    -fomit-frame-pointer
)
if [[ -n "$SYSROOT" ]]; then
    CROSS_OPT+=("--sysroot=$SYSROOT")
fi
HALF_OPT=()
if (( $# == 1 )); then
    HALF_OPT=(-DHALF)
fi

SOURCES=(
    "$SCRIPT_DIR/src/aes-sbox.c"
    "$SCRIPT_DIR/src/aes-sbox-keyschedule.c"
    "$SCRIPT_DIR/src/entry.c"
)
OBJECTS=(aes-sbox.o aes-sbox-keyschedule.o entry.o)

mkdir -p "$BUILD_DIR" "$ROOT_DIR/Core/Lib"
(
    cd "$BUILD_DIR"
    rm -f assemblo.log objecto.log
    "$CLANG" "${CROSS_OPT[@]}" "${HALF_OPT[@]}" "-I$SCRIPT_DIR/include" "${SOURCES[@]}" -S \
        2> >(tee assemblo.log >&2)
    "$CLANG" "${CROSS_OPT[@]}" "${HALF_OPT[@]}" "-I$SCRIPT_DIR/include" "${SOURCES[@]}" -c \
        2> >(tee objecto.log >&2)
    "$AR" rcs libtmp.a "${OBJECTS[@]}"
)
cp "$BUILD_DIR/libtmp.a" "$ROOT_DIR/Core/Lib/libtmp.a"
