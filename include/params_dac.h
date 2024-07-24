/**
 * @file params_dac.h
 * @author Andres C. Vargas R. (camilo.vargas@technaid.com gh: @andrescvargasr)
 * @brief
 * @version 0.1
 * @date 2024-07-24
 *
 *
 */

#ifndef DAC_PARAMS_H
#define DAC_PARAMS_H

/******************************************************************************/
/*                                                                            */
/*                      DAC63204 Interface                                    */
/*                                                                            */
/******************************************************************************/
// DAC ADDRESS
/**
 * @brief DAC63204 Address byte - 7 bits
 * - AD[6..3]: 1001
 * AD[2..0]:
 * | TARGET ADDRESS | A0 PIN |
 * | 000						| AGND 	 |
 * | 001						| VDD 	 |
 * | 010						| SDA 	 |
 * | 011						| SCL 	 |
 *
 * AD = 0b0100_1XXX
 */
#define DAC63204_ADDR 0x49

#ifdef DAC63204

#define NOP (0x00U)
#define DAC_0_MARGIN_HIGH (0x01U)
#define DAC_0_MARGIN_LOW (0x02U)
#define DAC_0_VOUT_CMP_CONFIG (0x03U)
#define DAC_0_IOUT_MISC_CONFIG (0x04U)
#define DAC_0_CMP_MODE_CONFIG (0x05U)
#define DAC_0_FUNC_CONFIG (0x06U)
#define DAC_1_MARGIN_HIGH (0x07U)
#define DAC_1_MARGIN_LOW (0x08U)
#define DAC_1_VOUT_CMP_CONFIG (0x09U)
#define DAC_1_IOUT_MISC_CONFIG (0x0AU)
#define DAC_1_CMP_MODE_CONFIG (0x0BU)
#define DAC_1_FUNC_CONFIG (0x0CU)
#define DAC_2_MARGIN_HIGH (0x0DU)
#define DAC_2_MARGIN_LOW (0x0EU)
#define DAC_2_VOUT_CMP_CONFIG (0x0FU)
#define DAC_2_IOUT_MISC_CONFIG (0x10U)
#define DAC_2_CMP_MODE_CONFIG (0x11U)
#define DAC_2_FUNC_CONFIG (0x12U)
#define DAC_3_MARGIN_HIGH (0x13U)
#define DAC_3_MARGIN_LOW (0x14U)
#define DAC_3_VOUT_CMP_CONFIG (0x15U)
#define DAC_3_IOUT_MISC_CONFIG (0x16U)
#define DAC_3_CMP_MODE_CONFIG (0x17U)
#define DAC_3_FUNC_CONFIG (0x18U)
#define DAC_0_DATA (0x19U)
#define DAC_1_DATA (0x1AU)
#define DAC_2_DATA (0x1BU)
#define DAC_3_DATA (0x1CU)
#define COMMON_CONFIG (0x1FU)
#define COMMON_TRIGGER (0x20U)
#define COMMON_DAC_TRIG (0x21U)
#define GENERAL_STATUS (0x22U)
#define CMP_STATUS (0x23U)
#define GPIO_CONFIG (0x24U)
#define DEVICE_MODE_CONFIG (0x25U)
#define INTERFACE_CONFIG (0x26U)
#define SRAM_CONFIG (0x2BU)
#define SRAM_DATA (0x2CU)
#define DAC_0_DATA_8BIT (0x40U)
#define DAC_1_DATA_8BIT (0x41U)
#define DAC_2_DATA_8BIT (0x42U)
#define DAC_3_DATA_8BIT (0x43U)
#define BRDCAST_DATA (0x50U)

/*** Bit definition for NOP register ******************************************/

/*** Bit definition for DAC_X_MARGIN_HIGH register ****************************/
#define DAC_MARGIN_HIGH 0xFFF0U
#define DAC_MARGIN_HIGH_Pos 0x4U
#define DAC_MARGIN_HIGH_Num_Bits 0x0FFFU
#define DAC_MARGIN_HIGH_Msk (DAC_MARGIN_HIGH_Num_Bits << DAC_MARGIN_HIGH_Pos)

/*** Bit definition for DAC_X_MARGIN_LOW register *****************************/
#define DAC_MARGIN_LOW 0xFFF0U
#define DAC_MARGIN_LOW_Pos 0x4U
#define DAC_MARGIN_LOW_Num_Bits 0x0FFFU
#define DAC_MARGIN_LOW_Msk (DAC_MARGIN_LOW_Num_Bits << DAC_MARGIN_LOW_Pos)

/*** Bit definition for DAC_X_VOUT_CMP_CONFIG register ************************/
#define VOUT_GAIN 0x1C00U
#define CMP_OD_EN 0x0010U
#define CMP_OUT_EN 0x0008U
#define CMP_HIZ_IN_DIS 0x0004U
#define CMP_INV_EN 0x0002U
#define CMP_EN 0x0001U

/*** Bit definition for DAC_X_IOUT_MISC_CONFIG register ***********************/
#define IOU_RANGE 0x1E00U

/*** Bit definition for DAC_X_CMP_MODE_CONFIG register ************************/
#define CMP_MODE 0x0C00U

/*** Bit definition for DAC_X_FUNC_CONFIG register ****************************/
#define CLR_SEL 0x8000U
#define SYNC_CONFIG 0x4000U
#define BRD_CONFIG 0x2000U

#define FUNC_GEN_CONFIG_BLOCK 0x1FFFU
#ifdef LINEAR_SLEW_MODE
#define PHASE_SEL 0x1800U
#define FUNC_CONFIG 0x0700U
#define LOG_SLEW_EN 0x0080U
#define CODE_STEP 0x0070U
#define SLEW_RATE 0x000FU
#endif
#ifdef LOG_SLEW_MODE
#define PHASE_SEL 0x1800U
#define FUNC_CONFIG 0x0700U
#define LOG_SLEW_EN 0x0080U
#define RISE_SLEW 0x0070U
#define FALL_SLEW 0x000EU
#endif

/*** Bit definition for DAC_X_DATA register ***********************************/
#define DAC_DATA 0xFFF0U

/*** Bit definition for COMMON_CONFIG register ********************************/
#define WIN_LATCH_EN 0x8000U
#define DEV_LOCK 0x4000U
#define EE_READ_ADDR 0x2000U
#define EN_INT_REF 0x1000U
#define VOUT_PDN_3 0x0C00U
#define IOUT_PDN_3 0x0200U
#define VOUT_PDN_2 0x0180U
#define IOUT_PDN_2 0x0040U
#define VOUT_PDN_1 0x0030U
#define IOUT_PDN_1 0x0008U
#define VOUT_PDN_0 0x0006U
#define IOUT_PDN_0 0x0001U

/*** Bit definition for COMMON_TRIGGER register *******************************/
#define DEV_UNLOCK 0xF000U
#define RESET 0x0F00U
#define LDAC 0x0080U
#define CLR 0x0040U
#define FAULT_DUMP 0x0010U
#define PROTECT 0x0008U
#define READ_ONE_TRIG 0x0004U
#define NVM_PROG 0x0002U
#define NVM_RELOAD 0x0001U

/*** Bit definition for COMMON_DAC_TRIG register ******************************/
#define RESET_CMP_FLAG_0 0x8000U
#define TRIG_MAR_LO_0 0x4000U
#define TRIG_MAR_HI_0 0x2000U
#define START_FUNC_0 0x1000U
#define RESET_CMP_FLAG_1 0x0800U
#define TRIG_MAR_LO_1 0x0400U
#define TRIG_MAR_HI_1 0x0200U
#define START_FUNC_1 0x0100U
#define RESET_CMP_FLAG_2 0x0080U
#define TRIG_MAR_LO_2 0x0040U
#define TRIG_MAR_HI_2 0x0020U
#define START_FUNC_2 0x0010U
#define RESET_CMP_FLAG_3 0x0008U
#define TRIG_MAR_LO_3 0x0004U
#define TRIG_MAR_HI_3 0x0002U
#define START_FUNC_3 0x0001U

/*** Bit definition for GENERAL_STATUS register *******************************/
#define NVM_CRC_FAIL_INT 0x8000U
#define NVM_CRC_FAIL_USER 0X4000U
#define DAC_3_BUSY 0x1000U
#define DAC_2_BUSY 0x0800U
#define DAC_1_BUSY 0x0400U
#define DAC_0_BUSY 0x0200U
#define DEVICE_ID 0x00FCU
#define VERSION_ID 0x0003U

#define NVM_CRC_FAIL_INT_Pos 0xFU
#define NVM_CRC_FAIL_INT_Size 0x1U
#define NVM_CRC_FAIL_INT_Msk \
    (NVM_CRC_FAIL_INT_Size << NVM_CRC_FAIL_INT_Pos) /*!< 0x8000 */

#define NVM_CRC_FAIL_USER_Pos 0xEU
#define NVM_CRC_FAIL_USER_Msk (0x1U << NVM_CRC_FAIL_USER_Pos) /*!< 0x8000 */

#define DAC_3_BUSY_Pos 0x1000U
#define DAC_2_BUSY_Pos 0x0800U
#define DAC_1_BUSY_Pos 0x0400U
#define DAC_0_BUSY_Pos 0x0200U

#define DEVICE_ID_Pos 0x2U
#define DEVICE_ID_Msk (0x3FU << DEVICE_ID_Pos) /*!< 0x00FC */

#define VERSION_ID_Pos 0x0003U

/*** Bit definition for CMP_STATUS register ***********************************/
#define PROTECT_FLAG 0x0100U
#define WIN_CMP_3 0x0080U
#define WIN_CMP_2 0x0040U
#define WIN_CMP_1 0x0020U
#define WIN_CMP_0 0x0010U
#define CMP_FLAG_3 0x0008U
#define CMP_FLAG_2 0x0004U
#define CMP_FLAG_1 0x0002U
#define CMP_FLAG_0 0x0001U

/*** Bit definition for GPIO_CONFIG register **********************************/
#define GF_EN 0x8000U
#define GPO_EN 0x2000U
#define GPO_CONFIG 0x1E00U
#define GPI_CH_SEL 0x01E0U

#ifdef GPI_CH_SEL
#define GPI_CH3_SEL 0x0100U
#define GPI_CH2_SEL 0x0080U
#define GPI_CH1_SEL 0x0040U
#define GPI_CH0_SEL 0x0020U
#endif

#define GPI_CONFIG 0x001EU
#define GPI_EN 0x0001U

/*** Bit definition for DEVICE_MODE_CONFIG register ***************************/
#define DIS_MODE_IN 0x2000U
#define PROTECT_CONFIG 0x0300U

/*** Bit definition for INTERFACE_CONFIG register *****************************/
#define TIMEOUT_EN 0x1000U
#define EN_PMBUS 0x0100U
#define FSDO_EN 0x0004U
#define SDO_EN 0x0001U

/*** Bit definition for SRAM_CONFIG register **********************************/
#define SRAM_ADDR 0x00FFU

/*** Bit definition for SRAM_DATA register ************************************/
#define SRAM_DATA_Msk 0xFFFFU /* Same as the register name */

/*** Bit definition for DAC_0_DATA_8BIT register ******************************/
#define DAC_0_DATA_8BIT_Msk 0xFF00U /* Same as the register name */

/*** Bit definition for DAC_1_DATA_8BIT register ******************************/
#define DAC_1_DATA_8BIT_Msk 0xFF00U /* Same as the register name */

/*** Bit definition for DAC_2_DATA_8BIT register ******************************/
#define DAC_2_DATA_8BIT_Msk 0xFF00U /* Same as the register name */

/*** Bit definition for DAC_3_DATA_8BIT register ******************************/
#define DAC_3_DATA_8BIT_Msk 0xFF00U /* Same as the register name */

/*** Bit definition for BRDCAST_DATA register *********************************/
#define BRDCAST_DATA_Msk 0xFFF0U /* Same as the register name */

#endif // End DAC_63204

#endif // End DAC_PARAMS_H