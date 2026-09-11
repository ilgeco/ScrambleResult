#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ST_FLASH="${ST_FLASH:-st-flash}"
AES_COMMUNICATOR="${AES_COMMUNICATOR:-$ROOT_DIR/AESComunicator/target/release/SerialCommunicator}"
AES_COMMUNICATOR_DIR="${AES_COMMUNICATOR_DIR:-$ROOT_DIR/AESComunicator}"
SERIAL_PORT="${SERIAL_PORT:-/dev/ttyUSB0}"
SERIAL_BAUD="${SERIAL_BAUD:-115200}"

make -C "$ROOT_DIR" clean
"$ROOT_DIR/AES/compile.sh" "$@"
make -C "$ROOT_DIR"
"$ST_FLASH" --reset write "$ROOT_DIR/build/AES_CORTEX.bin" 0x08000000
(
    cd "$AES_COMMUNICATOR_DIR"
    "$AES_COMMUNICATOR" "$SERIAL_PORT" "$SERIAL_BAUD"
)
