cmake_minimum_required(VERSION 3.13...3.27)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(main)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(main main.c)

target_link_libraries(main pico_stdlib)

#Creates a map/bin/hex/uf2 in addition to the ELF file (executables)
pico_add_extra_outputs(main)