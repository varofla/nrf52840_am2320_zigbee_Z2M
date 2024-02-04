cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nrf52840_am2320_zigbee_Z2M)

file(GLOB_RECURSE SRC_FILES
  main/*.c
)

target_sources(app PRIVATE ${SRC_FILES})

include_directories(
  main/hw
  main/hw/driver/am2320
)