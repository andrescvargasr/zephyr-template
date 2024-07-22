// Parameters
#include "params.h"

#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>

#define SLEEP_TIME_MS 500

static inline void emulate_work()
{
  for (volatile int count_out = 0; count_out < 150000; count_out++)
    ;
}

void thread1(void)
{
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
      if (ret == -EBUSY)
        goto repeat_send;
      printk("UART TX failed thd1: %d\n\r", ret);
    }

    time_stamp = k_uptime_get();
    emulate_work();
    delta_time = k_uptime_delta(&time_stamp);

    printk("thread1 yielding this round in %lld ms\n", delta_time);
    /* STEP 8 - Make the thread yield */
    // k_yield();
    /* STEP 10 - Put the thread to sleep */
    k_msleep(SLEEP_TIME_MS);
    /* Remember to comment out the line from STEP 8 */
  }
}