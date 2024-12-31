#ifndef SERVO_UTILS 
#define SERVO_UTILS

void initialize_motor();
// void turn_servo();
// void stop_servo();
void move_servo_duration(uint32_t time_interval, bool button_press);
#endif // SERVO_UTILS