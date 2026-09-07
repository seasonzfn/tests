#include "pico/stdlib.h"
#include "stdio.h"

#define BUTTON1 16   // Start button
#define BUTTON2 17   // Stop button

uint8_t mask = 0xFF;   // LEDs on GPIO 0–7

// Scanner pattern
uint8_t value = (1 << 0) | (1 << 1);
int direction = 1;

// Interrupt callback
void gpio_callback(uint gpio, uint32_t events) {

    // BUTTON1 → START (RISING EDGE)
    if (gpio == BUTTON1 && (events & GPIO_IRQ_EDGE_RISE)) {

        gpio_put_masked(mask, value);
        sleep_ms(100);

        // Move scanner
        if (direction == 1) {
            value <<= 1;
        } else {
            value >>= 1;
        }

        // Flip direction at edges
        if (value == ((1 << 6) | (1 << 7))) {
            direction = -1;
        }
        if (value == ((1 << 0) | (1 << 1))) {
            direction = 1;
        }
    }

    // BUTTON2 → STOP (FALLING EDGE)
    if (gpio == BUTTON2 && (events & GPIO_IRQ_EDGE_FALL)) {
        gpio_put_masked(mask, 0);   // turn off LEDs
        value = (1 << 0) | (1 << 1); // reset scanner
        direction = 1;
    }
}

int main() {
    stdio_init_all();

    // Init LEDs
    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, mask);

    // Init buttons
    gpio_init(BUTTON1);
    gpio_init(BUTTON2);

    gpio_set_dir(BUTTON1, GPIO_IN);
    gpio_set_dir(BUTTON2, GPIO_IN);

    gpio_pull_up(BUTTON1);
    gpio_pull_up(BUTTON2);

    // Register callback ONCE
    gpio_set_irq_enabled_with_callback(
        BUTTON1,
        GPIO_IRQ_EDGE_RISE,
        true,
        &gpio_callback
    );

    // BUTTON2 uses same callback
    gpio_set_irq_enabled(
        BUTTON2,
        GPIO_IRQ_EDGE_FALL,
        true
    );

    while (true) {
        tight_loop_contents();
    }
}
