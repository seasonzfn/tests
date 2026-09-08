#include "pico/stdlib.h"
#include "stdio.h"

#define BUTTON1 16
#define BUTTON2 17

uint8_t mask = 0x7F;
uint8_t value = (1 << 0) | (1 << 1);
int direction = 1;
volatile int scanning = 0;

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON1 && (events & GPIO_IRQ_EDGE_RISE)) {
        scanning = 1;
    }

    if (gpio == BUTTON2 && (events & GPIO_IRQ_EDGE_FALL)) {
        scanning = 0;
        gpio_put_masked(mask, 0);
        value = (1 << 0) | (1 << 1);
        direction = 1;
    }
}

int main() {
    stdio_init_all();

    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, mask);

    gpio_init(BUTTON1);
    gpio_init(BUTTON2);

    gpio_set_dir(BUTTON1, GPIO_IN);
    gpio_set_dir(BUTTON2, GPIO_IN);

    gpio_pull_up(BUTTON1);
    gpio_pull_up(BUTTON2);

    gpio_set_irq_enabled_with_callback(
        BUTTON1,
        GPIO_IRQ_EDGE_RISE,
        true,
        &gpio_callback
    );

    gpio_set_irq_enabled(
        BUTTON2,
        GPIO_IRQ_EDGE_FALL,
        true
    );

    while (true) {
        if (scanning == 1) {
            gpio_put_masked(mask, value);
            sleep_ms(100);

            if (direction == 1) value <<= 1;
            else value >>= 1;

            if (value == ((1 << 5) | (1 << 6))) direction = -1;
            if (value == ((1 << 0) | (1 << 1))) direction = 1;
        }

        tight_loop_contents();
    }
}
