# AES Cortex-M4

Firmware and host serial communicator for the STM32F207ZG.

## Required software

For the firmware build:

- Bash
- GNU Make
- Arm GNU Toolchain (`arm-none-eabi-gcc`, `arm-none-eabi-objcopy`, `arm-none-eabi-size`)
- [LLVM 17 with the Scrambler changes](https://github.com/llvm/llvm-project/tree/release/17.x), including `clang` and `llvm-ar`

For the complete hardware workflow:

- ST-LINK tools (`st-flash`)
- Rust (`rustc` and `cargo`)
- OpenSSL development files, `pkg-config`, and `libudev` development files
- Access to the STM32 serial port

The scrambled LLVM build is required for the AES compiler step. If its tools
are not on `PATH`, provide their locations:

```sh
CLANG=/path/to/scrambled/clang \
AR=/path/to/scrambled/llvm-ar \
SYSROOT=/path/to/arm-none-eabi \
./run.sh
```

On Debian/Ubuntu, the usual system packages are:

```sh
sudo apt install git make gcc-arm-none-eabi binutils-arm-none-eabi \
  libnewlib-arm-none-eabi stlink-tools rustc cargo pkg-config libssl-dev \
  libudev-dev
```

## Build

The host communicator is provided as the [`AESComunicator`](AESComunicator/)
Git submodule. Initialize it, then build it and the firmware separately:

```sh
git submodule update --init
cargo build --release --manifest-path AESComunicator/Cargo.toml
make
```

The host binary is written to
`AESComunicator/target/release/SerialCommunicator`. The firmware files are
written to `build/`; generated build directories are ignored by Git.

## Flash and run

After building the communicator, connect the board and run:

```sh
./run.sh
```

`run.sh` builds, flashes, and starts the submodule communicator. The
defaults are `/dev/ttyUSB0` at `115200` baud; override them when needed:

```sh
SERIAL_PORT=/dev/ttyACM0 SERIAL_BAUD=115200 ./run.sh
```

To build the reduced `HALF` variant:

```sh
./run.sh half
```
