# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
# set(DTC_OVERLAY_FILE "boards/nucleo_f411re.overlay")
# set(DTC_OVERLAY_FILE "boards/stm32_min_dev_black.overlay")
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr-template)

# target_sources(app PRIVATE ../src/main.c)
# target_sources_ifdef(CONFIG_I2C app PRIVATE ../src/I2CParams.c)

# To add all the files at the same time
FILE(GLOB app_sources ../src/*.c)
target_sources(app PRIVATE ${app_sources})