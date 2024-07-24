/**
 * @file thd_i2c.h
 * @author Andres C. Vargas R. (camilo.vargas@technaid.com gh: @andrescvargasr)
 * @brief 
 * @version 0.1
 * @date 2024-07-24
 * 
 * 
 */
#ifndef I2C_PARAMS_H
#define I2C_PARAMS_H

#include <zephyr/kernel.h>

bool init_i2c(void);

int read_i2c_pkt(const uint16_t dev_addr,
                      const uint8_t start_addr,
                      uint8_t* buf,
                      uint32_t num_bytes,
                      const uint8_t* name_register);

int write_i2c_pkt(const uint16_t dev_addr,
                  const uint8_t start_addr,
                  uint8_t* buf,
                  uint32_t num_bytes,
                  const uint8_t* name_register);

int write_read_i2c_pkt(const uint16_t dev_addr,
                  const uint8_t start_addr,
                  uint8_t* write_buf,
                  uint32_t num_write,
                  uint8_t* read_buf,
                  uint32_t num_read,
                  const uint8_t* name_register);

// Thread
void thread_i2c(void);

#endif  // End I2C_PARAMS_H