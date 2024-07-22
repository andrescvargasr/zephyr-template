# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
# set(DTC_OVERLAY_FILE "boards/nucleo_f411re.overlay")
set(DTC_OVERLAY_FILE "boards/stm32_min_dev_black.overlay")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr-template)

target_sources(app PRIVATE ../src/main.c)
