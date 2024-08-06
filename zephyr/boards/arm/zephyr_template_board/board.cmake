# SPDX-License-Identifier: EUPL-1.2

# Set 128k version STM32F103CB, not 64k version STM32F103C8
board_runner_args(jlink "--device=STM32F103CB" "--speed=4000")

include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
