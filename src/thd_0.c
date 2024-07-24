// Parameters
#include "params.h"

// Logging
#include <zephyr/logging/log.h> // LOG_[ERR, WRN, INF, DBG]

// Random number generator
#include <zephyr/random/random.h>

#define SLEEP_TIME_MS 1000

// Register the module in the logger
LOG_MODULE_REGISTER(thd_0, LOG_LEVEL_DBG);

// Call a semaphore extern variable
extern struct k_sem instance_monitor_sem;

// Define stack area used by workqueue thread
extern K_THREAD_STACK_DEFINE(my_stack_area, WORQ_THREAD_STACK_SIZE);

// Define queue structure
static struct k_work_q offload_work_q = {0};

static inline void emulate_work()
{
  for (volatile int count_out = 0; count_out < 150000; count_out++)
    ;
  uart_tx(uart, "emulate_work() done\n\r", 21, SYS_FOREVER_US);
}

// Creat a workqueue
struct work_info
{
  struct k_work work;
  char name[25];
} my_work;

void offload_function(struct k_work *work_term)
{
  emulate_work();
}

// Function for getting access of resource
void get_access(void)
{
  k_sem_take(&instance_monitor_sem, K_FOREVER);
  LOG_INF("Resource taken");
}

void thread0(void)
{
  LOG_INF("Thread0 started");
  uint64_t time_stamp;
  int64_t delta_time;

  k_work_queue_start(&offload_work_q, my_stack_area,
                     K_THREAD_STACK_SIZEOF(my_stack_area), WORKQ_PRIORITY,
                     NULL);

  strcpy(my_work.name, "Thread0 emulate_work()");
  k_work_init(&my_work.work, offload_function);

  while (1)
  {
    static uint8_t tx_buf[] = "Hello, I am thread0\n\r";
    int ret;
    /* Send the data over UART */
  repeat_send:
    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
    if (ret)
    {
      if (ret == -EBUSY)
        goto repeat_send;
      LOG_ERR("UART TX failed thd0: %d", ret);
    }
    time_stamp = k_uptime_get();
    k_work_submit_to_queue(&offload_work_q, &my_work.work);
    // emulate_work();
    delta_time = k_uptime_delta(&time_stamp);

    LOG_INF("thread0 yielding this round in %lld ms", delta_time);

    get_access();
    k_msleep(300 + sys_rand32_get() % 10);
  }
}