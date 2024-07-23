// Parameters
#include "params.h"

#include <zephyr/logging/log.h> // LOG_[ERR, WRN, INF, DBG]

// Random number generator
#include <zephyr/random/random.h> // sys_rand32_get()

#define SLEEP_TIME_MS 1000

LOG_MODULE_REGISTER(thd_1, LOG_LEVEL_DBG);
// LOG_MODULE_DECLARE(thd_2, LOG_LEVEL_DBG);

static inline void emulate_work()
{
  for (volatile int count_out = 0; count_out < 150000; count_out++)
    ;
}

void thread1(void)
{
  LOG_INF("Thread1 started");
  while (1)
  {
    uint64_t time_stamp;
    int64_t delta_time;
    /* STEP 3 - Call printk() to display a simple string "Hello, I am thread1" */
    // printk("Hello, I am thread1\n");
    static uint8_t tx_buf[] = "Hello, I am thread1\n\r";
    int ret;
    /* Send the data over UART */
  repeat_send:
    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
    if (ret)
    {
      if (ret == -EBUSY) {
        goto repeat_send;
      }
      LOG_ERR("UART TX failed thd1: %d\n\r", ret);
    }

    time_stamp = k_uptime_get();
    emulate_work();
    delta_time = k_uptime_delta(&time_stamp);


    // Release access to the resource
    release_access();
    // Assume the resource instance access is released at this point
    k_msleep(500 + sys_rand32_get() % 100);
  }
}