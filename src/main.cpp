#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#include "servo_utils.h"


int main()
{
    stdio_init_all();
    // setup button to control the servo
    constexpr uint button_pin {17};
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
    // adc result
    uint16_t result {0};
    // add photoresistor
    constexpr uint photoResistor_pin {26};
    adc_init(); //initiliaze adc module
    adc_gpio_init(photoResistor_pin);
    adc_select_input(0); // selects ADC0 as the input

    initialize_motor();
    
    while (true) 
    {
        sleep_ms(2);
        uint16_t result {adc_read()};
        printf("photoresistor value %u\n", result);
        volatile bool button_pressed {gpio_get(button_pin) == 0}; // Active low
        // move_servo_duration(1000 + 30, button_pressed);
        move_servo_in_light(result, button_pressed);
        
    }
}
