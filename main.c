#include "pico/stdlib.h" // Standard library for Pico
#include "stdio.h"

int main() {
    stdio_init_all(); // needed for picotool to autoload

    __uint8_t mask = 0x7F;

    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, mask);

    // Never-ending superloop
    while (true) {
        // 4. Turn all selected pins ON
        gpio_put_masked(mask, mask);
        sleep_ms(500);

        // 5. Turn all selected pins OFF
        gpio_put_masked(mask, 0);
        sleep_ms(500);
    }
}