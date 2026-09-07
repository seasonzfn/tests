// example programme interrupt & gpio mask - saxion (c) 2025

#include "pico/stdlib.h"
#include "stdio.h"

#define LED 1
#define BUTTON 16

volatile bool led_on = false;

// one general gpio callback function
void gpio_callback(uint gpio, uint32_t events) {
	if (gpio == BUTTON && (events & GPIO_IRQ_EDGE_FALL)) {
		gpio_xor_mask( (1<<LED) );
	} 
	// else if (GPIO == OTHERBUTTON) && (events & GPIO_IRQ_EDGE_FALL)) {
	// do_something_else();
	// } 
}

int main() {
    stdio_init_all(); // needed for picotool to auto load

	// init IO
	gpio_init_mask( (1<<LED) | (1 <<BUTTON) );
 	gpio_set_dir_all_bits( (1<<LED) | (0 <<BUTTON) );
	gpio_pull_up( BUTTON );
	gpio_xor_mask( (1<<LED) );
		
	// register interrupt callback
	gpio_set_irq_enabled_with_callback(
		BUTTON,
		GPIO_IRQ_EDGE_FALL,
		true,
		&gpio_callback
	);
	
	while (true) {
		tight_loop_contents(); // Idle loop
	}
}