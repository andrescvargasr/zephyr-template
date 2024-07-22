// Parameters
#include "params.h"

#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>

#define SLEEP_TIME_MS 500

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

void thread0(void)
{
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
    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
    if (ret)
    {
      printk("UART TX failed thd0: %d\n\r", ret);
    }
    time_stamp = k_uptime_get();
    k_work_submit_to_queue(&offload_work_q, &my_work.work);
    // emulate_work();
    delta_time = k_uptime_delta(&time_stamp);

    printk("thread0 yielding this round in %lld ms\n", delta_time);
    /* STEP 6 - Make the thread yield */
    // k_yield();
    /* STEP 10 - Put the thread to sleep */
    k_msleep(SLEEP_TIME_MS);
    /* Remember to comment out the line from STEP 6 */
  }
}