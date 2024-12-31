#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>

namespace motor
{
    constexpr uint control_pwm_pin {16};
    constexpr uint16_t wrap {39061};
    constexpr float clkdiv {64.0f};
} // namespace motor

void initialize_motor()
{
    // Initialize GPIO pin for PWM
    gpio_set_function(motor::control_pwm_pin, GPIO_FUNC_PWM);
    // get pwm slice and configure
    uint slice {pwm_gpio_to_slice_num(motor::control_pwm_pin)};
    pwm_config config {pwm_get_default_config()};
    // assume wrap = 39061
    // clkdiv = system clock / desired frequency * (wrap + 1)
    // clkdiv = 125 MHz / 50 Hz * (39061 + 1) = 64.0
    pwm_config_set_clkdiv(&config, motor::clkdiv);
    // wrap = system clock / clkdiv * frequency
    // wrap = 125MHz / 64 * 50 Hz
    pwm_config_set_wrap(&config, motor::wrap);
    // initialize pwm
    pwm_init(slice, &config, true);
}

inline void turn_servo()
{
    // set pulse width for controlling the servo
    // duty cycle = pulse width / period * wrap value
    // 1.0 ms / 20 ms * wrap for clockwise direction
    // 2.0 ms for anti-clockwise
    pwm_set_gpio_level(motor::control_pwm_pin, ((0.001 / 0.020) * motor::wrap));
}

inline void stop_servo()
{
    // set pulse width for controlling the servo
    pwm_set_gpio_level(motor::control_pwm_pin, 0);
}