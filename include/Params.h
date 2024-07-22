/**
 * @file Params.h
 * @author Andres C. Vargas R. (camilo.vargas@technaid.com gh: @andrescvargasr)
 * @brief 
 * @version 0.1
 * @date 2024-07-22
 * 
 * 
 */

#ifndef PARAMS_H
#define PARAMS_H

#define SOFTWARE_VERSION "v0.1.0"
#define SOFTWARE_VERSION_SEMVER 0x0010U /* MAJOR.MINOR.PATCH [0x0.M.m.P] */

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
#include "Params_dac.h"
#endif

#endif  // End PARAMS_H