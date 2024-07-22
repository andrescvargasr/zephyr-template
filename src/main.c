/**
 * @file main.c
 * @author Andres C. Vargas R. (camilo.vargas@technaid.com gh: @andrescvargasr)
 * @brief 
 * @version 0.1
 * @date 2024-07-22
 * 
 * 
 */

#include <stdlib.h>		   // EXIT_[SUCCESS, FAILURE]
#include <zephyr/kernel.h> // k_msleep()
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
// GPIO
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h> // PWM_DT_SPEC_GET(), pwm_is_ready_dt()
// Serial Communication (UART)
#include <zephyr/drivers/uart.h>
// Two Wire Interface (I2C)
#include <zephyr/drivers/i2c.h>

// UART
#define RECEIVE_BUFF_SIZE 10
#define RECEIVE_TIMEOUT 100

/* LED0_NODE is the devicetree node identifier for the node with alias "led0". */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define PWM_LED0_NODE DT_ALIAS(pwm_led0)
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(PWM_LED0_NODE);

// UART_NODE is the devicetree node identifier for the node with alias "usart2".
#define UART_NODE DT_ALIAS(uart0)
static const struct device *uart = DEVICE_DT_GET(UART_NODE);

// I2C_NODE is the devicetree node identifier for the node with alias "DAC63204".
#define I2C1_NODE DT_NODELABEL(dac63204)
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C1_NODE);

// PWM
#define NUM_STEPS 50U
#define SLEEP_MSEC 25U

// UART: Define buffers
static uint8_t tx_buf[] = {"Technaid SL Connect DMA\n\r"};
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

// UART: Define the callback function for UART
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type)
	{
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

int main(void)
{
	uint32_t pulse_width = 0U;
	uint32_t step = pwm_led0.period / NUM_STEPS;
	uint8_t dir = 1U;
	int ret;

	printk("Techstim-fw\n");

	if (!device_is_ready(led.port))
	{
		return EXIT_FAILURE;
	}

	if (!pwm_is_ready_dt(&pwm_led0))
	{
		printk("Error: PWM device %s is not ready\n",
			   pwm_led0.dev->name);
		return 0;
	}

	if (!device_is_ready(uart))
	{
		printk("%s is not ready!", uart->name);
		return EXIT_FAILURE;
	}

	// I2C ready?
	if (!device_is_ready(dev_i2c.bus))
	{
		printk("I2C bus %s is not ready!", dev_i2c.bus->name);
		return EXIT_FAILURE;
	}

	// CONFIGURE
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
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
		printk("Hello: %d\n\r", ret);
		return EXIT_FAILURE;
	}

	ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
	if (ret)
	{
		return EXIT_FAILURE;
	}

	// Write on the I2C bus
	uint8_t reg_dir = 0x19;			 // Address 19h: DAC-0-DATA
	uint8_t value[2] = {0x12, 0x34}; // [0]: msb; [1]: lsb
	printk("I2C write:\t\t");
	printk("0x%02x\t", value[0]);
	printk("0x%02x\n\r", value[1]);
	// printk("I2C write: 0x%04x\n\r", htons((uint16_t)*value & 0xFFF0));
	// ret = i2c_write_dt(&dev_i2c, config, sizeof(config));
	ret = i2c_burst_write_dt(&dev_i2c, reg_dir, value, sizeof(value));
	if (ret != 0)
	{
		printk("Failed to write to I2C device address %x at reg. %x \n\r", dev_i2c.addr, reg_dir);
	}
	// Read on the I2C bus
	uint8_t data[2]; // [0]: msb; [1]: lsb
	ret = i2c_burst_read_dt(&dev_i2c, reg_dir, data, sizeof(data));
	if (ret != 0)
	{
		printk("Failed to read from I2C device address %x at Reg. %x \n\r", dev_i2c.addr, reg_dir);
	}
	printk("I2C read (recv):\t");
	printk("0x%02x\t", data[0]);
	printk("0x%02x\n\r", data[1]);

	data[0] = 0, data[1] = 0;
	// Do a burst read of 6 bytes as each color channel is 2 bytes
	ret = i2c_burst_read_dt(&dev_i2c, reg_dir, data, sizeof(data));
	printk("I2C read burst (recv):\t");
	printk("0x%02x\t", data[0]);
	printk("0x%02x\n\r", data[1]);

	while (1)
	{
		ret = pwm_set_pulse_dt(&pwm_led0, pulse_width);
		if (ret)
		{
			printk("Error %d: failed to set pulse width\n", ret);
			return 0;
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
	return 0;
}