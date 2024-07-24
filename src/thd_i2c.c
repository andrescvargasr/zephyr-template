/**
 * @file thd_i2c.h
 * @author Andres C. Vargas R. (camilo.vargas@technaid.com gh: @andrescvargasr)
 * @brief
 * @version 0.1
 * @date 2024-07-24
 *
 *
 */

#include "params.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 10

// I2C_NODE is the devicetree node identifier for the node with alias "DAC63204".
#define I2C1_NODE DT_NODELABEL(dac63204)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C1_NODE);

#if LOG_EN_LEVEL == 4
LOG_MODULE_REGISTER(thd_i2c, LOG_LEVEL_DBG);
#elif LOG_EN_LEVEL == 3
LOG_MODULE_REGISTER(thd_i2c, LOG_LEVEL_INF);
#elif LOG_EN_LEVEL == 2
LOG_MODULE_REGISTER(thd_i2c, LOG_LEVEL_WRN);
#elif LOG_EN_LEVEL == 1
LOG_MODULE_REGISTER(thd_i2c, LOG_LEVEL_ERR);
#endif

/**
 * @brief Check the correct availability of I2C device.
 *
 * @return true If successful.
 * @return false If error.
 */
bool init_i2c(void)
{
  if (!device_is_ready(dev_i2c.bus))
  {
    LOG_ERR("I2C bus %s is not ready!", dev_i2c.bus->name);
    return false;
  }
  return true;
}

/**
 * @brief Read multiple bytes from an internal address of an I2C device.
 *
 * This routine reads multiple bytes from an internal address of an
 * I2C device synchronously.
 *
 * Instances of this may be replaced by write_read_i2c_pkt().
 *
 * @param start_addr Internal address from which the data is being read.
 * @param buf Memory pool that stores the retrieved data.
 * @param num_bytes Number of bytes being read.
 * @param name_register Name of the first register to read.
 *
 * @retval 0 If successful.
 * @retval -EIO General input / output error.
 */
int read_i2c_pkt(const uint8_t start_addr,
                 uint8_t *buf,
                 uint32_t num_bytes,
                 const uint8_t *name_register)
{
  int ret = 0;
  ret = i2c_burst_read_dt(&dev_i2c, start_addr, buf, sizeof(num_bytes));
  if (ret < 0)
  {
    LOG_ERR("Could not write ADDR %i with I2C (err %i)", start_addr, ret);
  }
  else
  {
    LOG_INF("I2C read in %s:", name_register);
    LOG_HEXDUMP_INF(buf, num_bytes, "");
  }

  return ret;
}

/**
 * @brief Write multiple bytes to an internal address of an I2C device.
 *
 * This routine writes multiple bytes to an internal address of an
 * I2C device synchronously.
 *
 * @param start_addr Internal address to which the data is being written.
 * @param buf Memory pool from which the data is transferred.
 * @param num_bytes Number of bytes being written.
 * @param name_register Name of the first register to read.
 *
 * @retval 0 If successful.
 * @retval -EIO General input / output error.
 */
int write_i2c_pkt(const uint8_t start_addr,
                  uint8_t *buf,
                  uint32_t num_bytes,
                  const uint8_t *name_register)
{
  int ret = 0;
  ret = i2c_burst_write_dt(&dev_i2c, start_addr, buf, num_bytes);
  if (ret < 0)
  {
    LOG_ERR("Could not write ADDR %i with I2C (err %i)", start_addr, ret);
  }
  else
  {
    LOG_INF("I2C write in %s:", name_register);
    LOG_HEXDUMP_INF(buf, num_bytes, "");
  }

  return ret;
}

/**
 * @brief Write then read data from an I2C device.
 *
 * This supports the common operation "this is what I want", "now give
 * it to me" transaction pair through a combined write-then-read bus
 * transaction.
 *
 * @param start_addr Address of the I2C device
 * @param write_buf Pointer to the data to be written
 * @param num_write Number of bytes to write
 * @param read_buf Pointer to storage for read data
 * @param num_read Number of bytes to read
 * @param name_register Name of the first register to read.
 *
 * @retval 0 If successful.
 * @retval -EIO General input / output error.
 * @retval -EIO(x2) General input & output error.
 */
int write_read_i2c_pkt(const uint8_t start_addr,
                       uint8_t *write_buf,
                       uint32_t num_write,
                       uint8_t *read_buf,
                       uint32_t num_read,
                       const uint8_t *name_register)
{
  int ret = 0;

  ret =
      write_i2c_pkt(start_addr, write_buf, num_write, name_register);

  ret += read_i2c_pkt(start_addr, read_buf, num_read, name_register);

  return ret;
}

void thread_i2c(void)
{
  int ret;
  /****************************************************************************/
  /*                     I2C                                                  */
  /****************************************************************************/
  bool i2c_ok = false;
  uint8_t who_am_i[2] = {0, 0};

  // Initialize the I2C device
  i2c_ok = init_i2c();

  LOG_DBG("I2C initialization: %s", i2c_ok ? "Ok" : "Error");

  // Read data
  ret = read_i2c_pkt(GENERAL_STATUS, who_am_i, sizeof(who_am_i),
                     "GENERAL_STATUS");

  // Send data
  LOG_DBG("Set output 1");
  uint8_t data_i2c_pkt_msdb = 0;
  uint8_t data_i2c_pkt_lsdb = 0;
  uint8_t data_i2c_pkt_sent[2] = {data_i2c_pkt_msdb, data_i2c_pkt_lsdb};
  uint8_t data_i2c_pkt_recv[2] = {0};

  LOG_DBG("Set DAC_1_MARGIN_HIGH");
  data_i2c_pkt_msdb = 0xe6;
  data_i2c_pkt_lsdb = 0x60;
  data_i2c_pkt_sent[0] = data_i2c_pkt_msdb;
  data_i2c_pkt_sent[1] = data_i2c_pkt_lsdb;
  ret = write_read_i2c_pkt(DAC_1_MARGIN_HIGH, data_i2c_pkt_sent,
                           sizeof(data_i2c_pkt_sent), data_i2c_pkt_recv,
                           sizeof(data_i2c_pkt_recv), "DAC_1_MARGIN_HIGH");

  LOG_DBG("Set DAC_1_MARGIN_LOW");
  data_i2c_pkt_msdb = 0x19;
  data_i2c_pkt_lsdb = 0xA0;
  data_i2c_pkt_sent[0] = data_i2c_pkt_msdb;
  data_i2c_pkt_sent[1] = data_i2c_pkt_lsdb;
  ret = write_read_i2c_pkt(DAC_1_MARGIN_LOW, data_i2c_pkt_sent,
                           sizeof(data_i2c_pkt_sent), data_i2c_pkt_recv,
                           sizeof(data_i2c_pkt_recv), "DAC_1_MARGIN_LOW");

  LOG_DBG("Set DAC_1_DATA");
  data_i2c_pkt_msdb = 0x5A;
  data_i2c_pkt_lsdb = 0x00;
  // uint8_t dac_data_msdb = 0xE2;
  // uint8_t dac_data_lsdb = 0x60;
  data_i2c_pkt_sent[0] = data_i2c_pkt_msdb;
  data_i2c_pkt_sent[1] = data_i2c_pkt_lsdb;
  ret = write_read_i2c_pkt(DAC_1_DATA, data_i2c_pkt_sent,
                           sizeof(data_i2c_pkt_sent), data_i2c_pkt_recv,
                           sizeof(data_i2c_pkt_recv), "DAC_1_DATA");

  LOG_DBG("Set COMMON_CONFIG");
  data_i2c_pkt_msdb = 0x0F;
  data_i2c_pkt_lsdb = 0xCF;
  data_i2c_pkt_sent[0] = data_i2c_pkt_msdb;
  data_i2c_pkt_sent[1] = data_i2c_pkt_lsdb;
  write_read_i2c_pkt(COMMON_CONFIG, data_i2c_pkt_sent,
                     sizeof(data_i2c_pkt_sent), data_i2c_pkt_recv,
                     sizeof(data_i2c_pkt_recv), "COMMON_CONFIG");

  for (;;)
  {
    static uint16_t offset = 0x000FU; /* First 4 bits don't care (12 bits DAC)*/
    static uint16_t data = 0x0000U;

#ifdef CONFIG_DAC63204
#ifdef CONFIG_I2C
    LOG_INF("Running a little test:");
    data_i2c_pkt_sent[0] = ((data >> 0x8U) & 0x00FF);
    data_i2c_pkt_sent[1] = (data & 0x00FF);
    write_i2c_pkt(DAC_1_DATA, data_i2c_pkt_sent,
                  sizeof(data_i2c_pkt_sent), "DAC Test");
    data += offset;
#endif
#endif

    k_msleep(SLEEP_TIME_MS);
  }
}