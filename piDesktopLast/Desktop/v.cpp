#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

#define MOTOR_PIN 1  // Assuming GPIO pin 1 corresponds to pin 18

int main() {
    wiringPiSetup();
    softPwmCreate(MOTOR_PIN, 0, 100);  // Initialize PWM pin with range 0-100

    for (int i = 0; i < 100; ++i) {
        softPwmWrite(MOTOR_PIN, 100);  // Set PWM duty cycle to 100%

        delay(1000);  // 1 second ON

        softPwmWrite(MOTOR_PIN, 0);  // Set PWM duty cycle to 0%

        delay(1000);  // 1 second OFF
    }

    return 0;
}
