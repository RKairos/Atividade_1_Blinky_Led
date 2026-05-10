#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 3000

// Define o LED usando Device Tree
#define LED0_NODE DT_ALIAS(led0) // verde
#define LED2_NODE DT_ALIAS(led2) // vermelho

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LED0_NODE, okay) && DT_NODE_HAS_STATUS(LED2_NODE, okay)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

void main(void)
{
    int ret;
    int ret_1;

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&led_1)) {
        printk("Error: LED device %s is not ready\n", led.port->name);
        return;
    }

    // Configura o pino como saída
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    ret_1 = gpio_pin_configure_dt(&led_1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0 || ret_1 < 0) {
        printk("Error %d: failed to configure LED pin\n", ret);
        return;
    }

    printk("LED blinking on %s pin %d\n", led.port->name, led.pin);

    while (1) {
        // Toggle do LED usando a nova API

        //começa verde
        gpio_pin_toggle_dt(&led_1); // desliga vermelho (verde ligado)
        k_msleep(2*SLEEP_TIME_MS);

        //vira amarelo --> tempo menor
        gpio_pin_toggle_dt(&led_1); // liga vermelho (verde ligado)
        k_msleep(SLEEP_TIME_MS);

        //vira vermelho
        gpio_pin_toggle_dt(&led); // desliga verde (vermelho ligado)
        k_msleep(2*SLEEP_TIME_MS);

        //volta para ponto inicial (dois ligados)
        gpio_pin_toggle_dt(&led); // liga verde (vermelho ligado)
        //não tem atraso pois é uma mudança imediata
        //reinicia o while sempre na mesma condição

    }
}