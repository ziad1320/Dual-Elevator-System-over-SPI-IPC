# Dual-Elevator-System-over-SPI-IPC

## Target Hardware
- **MCU**: STM32F401VE (ARM Cortex-M4)

## Project Overview
This project implements a dual-elevator control system using two independent STM32F4 MCUs communicating via SPI. The codebase is designed to be modular and cross-platform (Linux/Windows).

## Directory Structure (MCAL/HAL/APP)
The project follows a strict layered architecture:
- **src/**: Main application entry points (`master.c`, `slave.c`).
- **lib/**: Shared standard types and bit manipulation utilities.
- **Mcal/**: Microcontroller Abstraction Layer (RCC, GPIO, SPI, etc.).
- **Hal/**: Hardware Abstraction Layer (Button, Motor, etc.).
- **App/**: Application Layer (State Machines, Dispatcher).
- **cmake/**: Modular CMake build system.
- **STM32-base/**: CMSIS and Device-specific startup/linker files.

## Build Instructions

### Prerequisites
- CMake (version 3.28+)
- Arm GNU Toolchain (`arm-none-eabi-gcc`)
- Make or Ninja

### Building the Project
1. Open a terminal in the project root.
2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
3. Configure the project:
   ```bash
   cmake ..
   ```
4. Build:
   ```bash
   make  # or ninja
   ```

### Output Files
The build process generates separate executables for both controllers in the `build/` directory:
- `Master_Elevator.hex` / `Master_Elevator.bin`
- `Slave_Elevator.hex` / `Slave_Elevator.bin`
