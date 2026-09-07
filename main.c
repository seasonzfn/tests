#include "pico/stdlib.h" // Standard library for Pico
#include "stdio.h"

int main() {
    stdio_init_all(); // needed for picotool to autoload

    __uint8_t mask = 0x7F;

    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, mask);

    // Never-ending superloop
    while (true) {
        for(uint pin = 0; pin <= 6; pin++){
            uint8_t value = (1 << pin) | (1 << pin) + 1;
            gpio_put_masked(mask, value);
            sleep_ms(50);
        }
        for(int pin = 6; pin >= 0; pin--){
            uint8_t value = (1 << pin) | (1 << pin) + 1;
            gpio_put_masked(mask, value);
            sleep_ms(50);
        }

    }
}