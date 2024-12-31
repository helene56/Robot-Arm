#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>

namespace motor
{
    constexpr uint control_pwm_pin {16};
    
    constexpr uint16_t wrap {39061};
    constexpr float clkdiv {64.0f};

} // namespace motor

namespace Time
{
    uint32_t interval               = 800;
    static uint32_t last_frame_time = 0;
    static uint32_t current_time    = to_ms_since_boot(get_absolute_time());
    //  frames::last_frame_time = current_time;
    bool switch_frame = (current_time - last_frame_time >= interval);
} // namespace Time

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

static inline void turn_servo()
{
    // set pulse width for controlling the servo
    // duty cycle = pulse width / period * wrap value
    // 1.0 ms / 20 ms * wrap for clockwise direction
    // 2.0 ms for anti-clockwise highest speed
    // <~1.4 ms slowest, decrease to get higher speed clockwise direction
    // <2.0 ms fastest, decrease to get slower speed anticlockwise direction 
    pwm_set_gpio_level(motor::control_pwm_pin, ((0.0017 / 0.020) * motor::wrap));
}

static inline void stop_servo()
{
    // set pulse width for controlling the servo
    pwm_set_gpio_level(motor::control_pwm_pin, 0);
}

void move_servo_duration(uint32_t time_interval, bool servo_on)
{
    // static bool first_time {true};
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    static uint32_t last_time {0};
    if (servo_on)
    {
        turn_servo();
    }
    
    if ((current_time - last_time >= time_interval))
    {
        stop_servo();
        last_time = current_time;
    }
    
}