/**
 * @file params.h
 * @author Andres C. Vargas R. (camilo.vargas@technaid.com gh: @andrescvargasr)
 * @brief
 * @version 0.1
 * @date 2024-07-22
 *
 *
 */

#ifndef PARAMS_H
#define PARAMS_H

#include <stdlib.h>        // EXIT_[SUCCESS, FAILURE]
#include <zephyr/kernel.h> // k_msleep()
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>  // printk()
#include <zephyr/logging/log.h> // LOG_[ERR, WRN, INF, DBG]
// GPIO
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h> // PWM_DT_SPEC_GET(), pwm_is_ready_dt()
// Serial Communication (UART)
#include <zephyr/drivers/uart.h>
// Two Wire Interface (I2C)
#include <zephyr/drivers/i2c.h>

#define SOFTWARE_VERSION "v0.4.0"
#define SOFTWARE_VERSION_SEMVER 0x0040U /* MAJOR.MINOR.PATCH [0x0.M.m.P] */

#define THD_0
#define THD_1
#ifdef CONFIG_I2C /* CONFIG_I2C is directly from Zephyr */
// #define THD_I2C
#endif

/**
 * @brief
 * | LEVEL | SEVERITY	| MACRO |
 * | --- | --- | --- |
 * | 1 (most severe) | Error | LOG_LEVEL_ERR |
 * | 2 | Warning | LOG_LEVEL_WRN |
 * | 3 | Info | LOG_LEVEL_INF |
 * | 4(least severe) | Debug | LOG_LEVEL_DBG |
 *
 */
#define LOG_EN_LEVEL 4 /* Enable Debugging messages in all the */

#ifdef CONFIG_DAC63204
#define DAC63204 /* Define DAC part number */
#include "params_dac.h"
#endif

#define WORQ_THREAD_STACK_SIZE 512
#define WORKQ_PRIORITY 4

// Call a uart extern variable
extern const struct device *uart;

#endif // End PARAMS_H