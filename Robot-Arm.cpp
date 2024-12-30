#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"


int main()
{
    stdio_init_all();

    constexpr uint pwm_pin {16};
    // Initialize GPIO pin for PWM
    gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
    // get pwm slice and configure
    uint slice {pwm_gpio_to_slice_num(pwm_pin)};
    pwm_config config {pwm_get_default_config()};
    // assume wrap = 39061
    // clkdiv = system clock / desired frequency * (wrap + 1)
    // clkdiv = 125 MHz / 50 Hz * (39061 + 1) = 64.0
    pwm_config_set_clkdiv(&config, 64.0f);
    // wrap = system clock / clkdiv * frequency
    // wrap = 125MHz / 64 * 50 Hz
    pwm_config_set_wrap(&config, 39061);
    // initialize pwm
    pwm_init(slice, &config, true);
    
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
