#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

void turn_servo(uint pin, double wrap);
void stop_servo(uint pin, double wrap);

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

    turn_servo(pwm_pin, 39061);
    // full round 360
    // sleep_ms(1110);
    // 90 deg
    // sleep_ms(585);
    sleep_ms(280);
    pwm_set_gpio_level(pwm_pin, 0);
    while (true) 
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
        
    }
}

void turn_servo(uint pin, double wrap)
{
    // set pulse width for controlling the servo
    // duty cycle = pulse width / period * wrap value
    // 1.0 ms / 20 ms * wrap for clockwise direction
    // 2.0 ms for anti-clockwise
    double duty_cycle {(0.001 / 0.020) * wrap};
    pwm_set_gpio_level(pin, duty_cycle);


}

void stop_servo(uint pin, double wrap)
{
    // set pulse width for controlling the servo
    // duty cycle = pulse width / period * wrap value
    // 1.0 ms / 20 ms * wrap for clockwise direction
    // 2.0 ms for anti-clockwise
    double duty_cycle {(0.0015 / 0.020) * wrap};
    pwm_set_gpio_level(pin, duty_cycle);


}
