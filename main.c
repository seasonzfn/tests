#include "pico/stdlib.h" // Standard library for Pico
#include "stdio.h"

int main() {
    stdio_init_all(); // needed for picotool to autoload

    __uint8_t mask = 0x7F;

    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, mask);
    uint8_t value = (1 << 0) | (1 << 1);
    int direction = 1;

    

    // Never-ending superloop
    while (true) {
        gpio_put_masked(mask, value);
        sleep_ms(100);

        if (direction == 1) {
            value <<= 1;
        } else {
            value >>= 1;
        }

        if (value == ((1 << 6) | (1 << 7))) {
            direction = -1;
        }
        if (value == ((1 << 0) | (1 << 1))) {
            direction = 1;
        }

    }
}