// Parameters
#include "params.h"

// Logging
#include <zephyr/logging/log.h> // LOG_[ERR, WRN, INF, DBG]

// Random number generator
#include <zephyr/random/random.h> // sys_rand32_get()

#define SLEEP_TIME_MS 1000

// Register the module in the logger
LOG_MODULE_REGISTER(thd_1, LOG_LEVEL_DBG);

// Call a semaphore extern variable
extern struct k_sem instance_monitor_sem;

static inline void emulate_work()
{
  for (volatile int count_out = 0; count_out < 150000; count_out++)
    ;
}

// Function for releasing access of resource
void release_access(void)
{
  LOG_INF("Resource given");

  /* STEP 10.2 - Give semaphore after finishing access to resource */
  k_sem_give(&instance_monitor_sem);
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
      if (ret == -EBUSY)
      {
        k_msleep(10);
        goto repeat_send;
      }
      LOG_ERR("UART TX failed thd1: %d", ret);
    }

    time_stamp = k_uptime_get();
    emulate_work();
    delta_time = k_uptime_delta(&time_stamp);

    LOG_INF("thread1 yielding this round in %lld ms", delta_time);

    // Release access to the resource
    release_access();
    // Assume the resource instance access is released at this point
    k_msleep(500 + sys_rand32_get() % 100);
  }
}