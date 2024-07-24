/**
 * @file main.c
 * @author Andres C. Vargas R. (camilo.vargas@technaid.com gh: @andrescvargasr)
 * @brief
 * @version 0.1
 * @date 2024-07-22
 *
 *
 */

// Parameters
#include "params.h" /* DACParams.h and I2CParams.h are here */

#ifndef SOFTWARE_VERSION /* 1st definition at Params, 2nd here */
#define SOFTWARE_VERSION "v0.3.0"
#define SOFTWARE_VERSION_SEMVER 0x0030U /* MAJOR.MINOR.PATCH [0x0.M.m.P] */
#endif

/**
 * @brief Register code with logger intead of printk()
 * | LEVEL | SEVERITY	| MACRO |
 * | --- | --- | --- |
 * | 1 (most severe) | Error | LOG_LEVEL_ERR |
 * | 2 | Warning | LOG_LEVEL_WRN |
 * | 3 | Info | LOG_LEVEL_INF |
 * | 4(least severe) | Debug | LOG_LEVEL_DBG |
 *
 * Description:
 * - Error: Severe error conditions
 * - Warning: Conditions that should be taken care of
 * - Info: Informational messages that require no action
 * - Debug: Debugging messages
 *
 */
#if LOG_EN_LEVEL == 4
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);
#elif LOG_EN_LEVEL == 3
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);
#elif LOG_EN_LEVEL == 2
LOG_MODULE_REGISTER(main, LOG_LEVEL_WRN);
#elif LOG_EN_LEVEL == 1
LOG_MODULE_REGISTER(main, LOG_LEVEL_ERR);
#endif

// Define sempahore
K_SEM_DEFINE(instance_monitor_sem, 10, 10);

// Define mutex
K_MUTEX_DEFINE(instance_monitor_mutex);

// UART
#define RECEIVE_BUFF_SIZE 10
#define RECEIVE_TIMEOUT 100

/* LED0_NODE is the devicetree node identifier for the node with alias "led0". */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define PWM_LED0_NODE DT_ALIAS(pwm_led0)
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(PWM_LED0_NODE);

// UART_NODE is the devicetree node identifier for the node with alias "usart2".
// uart device is used in other files, so it is defined as a global variable.
#define UART_NODE DT_ALIAS(uart0)
const struct device *uart = DEVICE_DT_GET(UART_NODE);

// I2C_NODE is the devicetree node identifier for the node with alias "DAC63204".
// #define I2C1_NODE DT_NODELABEL(dac63204)
// static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C1_NODE);

// PWM
#define NUM_STEPS 50U
#define SLEEP_MSEC 25U

// UART: Define buffers
static uint8_t tx_buf[] = {"<uart_async>: Technaid SL Connect DMA\n\r"};
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

// UART: Define the callback function for UART
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type)
	{
	case UART_TX_DONE:
		/* add code here */
		break;
	case UART_RX_RDY:
		if (evt->data.rx.len == 1)
		{
			if (evt->data.rx.buf[evt->data.rx.offset] == '1')
				gpio_pin_toggle_dt(&led);
		}
		break;
	case UART_RX_DISABLED:
		uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
		break;

	default:
		break;
	}
}

/******************************************************************************/
/*   MAIN                                                                     */
/******************************************************************************/
int main(void)
{
	uint32_t pulse_width = 0U;
	uint32_t step = pwm_led0.period / NUM_STEPS;
	uint8_t dir = 1U;
	int ret;

	LOG_INF("Zephyr Template");
	LOG_INF("Software version: %s", SOFTWARE_VERSION);

	if (!device_is_ready(led.port))
	{
		return EXIT_FAILURE;
	}

	if (!pwm_is_ready_dt(&pwm_led0))
	{
		LOG_ERR("PWM device %s is not ready",
				pwm_led0.dev->name);
		return 0;
	}

	if (!device_is_ready(uart))
	{
		LOG_ERR("%s is not ready!", uart->name);
		return EXIT_FAILURE;
	}

	// // I2C ready?
	// if (!device_is_ready(dev_i2c.bus))
	// {
	// 	LOG_ERR("I2C bus %s is not ready!", dev_i2c.bus->name);
	// 	return EXIT_FAILURE;
	// }

	// CONFIGURE
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		LOG_ERR("%s is not ready: %d", led.port->name, ret);
		return EXIT_FAILURE;
	}

	// Register the UART callback function
	ret = uart_callback_set(uart, uart_cb, NULL);
	if (ret)
	{
		return EXIT_FAILURE;
	}

	ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
	if (ret)
	{
		LOG_ERR("%s failed to send msg: %d", uart->name, ret);
		return EXIT_FAILURE;
	}

	ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
	if (ret)
	{
		return EXIT_FAILURE;
	}

	// // Write on the I2C bus
	// uint8_t reg_dir = 0x19;			 // Address 19h: DAC-0-DATA
	// uint8_t value[2] = {0x12, 0x34}; // [0]: msb; [1]: lsb
	// LOG_DBG("I2C wr:\t\t\t0x%02x\t0x%02x", value[0], value[1]);
	// // printk("I2C write: 0x%04x\n\r", htons((uint16_t)*value & 0xFFF0));
	// // ret = i2c_write_dt(&dev_i2c, config, sizeof(config));
	// ret = i2c_burst_write_dt(&dev_i2c, reg_dir, value, sizeof(value));
	// if (ret != 0)
	// {
	// 	printk("Failed to write to I2C device address %x at reg. %x \n\r", dev_i2c.addr, reg_dir);
	// }
	// // Read on the I2C bus
	// uint8_t data[2]; // [0]: msb; [1]: lsb
	// ret = i2c_burst_read_dt(&dev_i2c, reg_dir, data, sizeof(data));
	// if (ret != 0)
	// {
	// 	printk("Failed to read from I2C device address %x at Reg. %x \n\r", dev_i2c.addr, reg_dir);
	// }
	// LOG_DBG("I2C rd (recv):\t\t0x%02x\t0x%02x", data[0], data[1]);

	// data[0] = 0, data[1] = 0;
	// // Do a burst read of 6 bytes as each color channel is 2 bytes
	// ret = i2c_burst_read_dt(&dev_i2c, reg_dir, data, sizeof(data));

	// LOG_DBG("I2C rd burst (recv):\t0x%02x\t0x%02x", data[0], data[1]);

	while (1)
	{
		ret = pwm_set_pulse_dt(&pwm_led0, pulse_width);
		if (ret)
		{
			LOG_ERR("Error %d: failed to set pulse width", ret);
			return EXIT_FAILURE;
		}
		// printk("Using pulse width %d%%\n", 100 * pulse_width / pwm_led0.period);

		if (dir)
		{
			pulse_width += step;
			if (pulse_width >= pwm_led0.period)
			{
				pulse_width = pwm_led0.period - step;
				dir = 0U;
			}
		}
		else
		{
			if (pulse_width >= step)
			{
				pulse_width -= step;
			}
			else
			{
				pulse_width = step;
				dir = 1U;
			}
		}

		k_sleep(K_MSEC(SLEEP_MSEC));
	}
	return EXIT_FAILURE;
}