#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "servo_utils.h"


int main()
{
    stdio_init_all();
    // setup button to control the servo
    constexpr uint button_pin {17};
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
    initialize_motor();
    
    while (true) 
    {
        bool button_pressed {gpio_get(button_pin) == 0}; // Active low
        move_servo_duration(1000, button_pressed);
        // sleep_ms(3000);
        
    }
}
