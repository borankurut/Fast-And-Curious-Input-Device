import RPi.GPIO as GPIO  # sudo apt-get install python-rpi.gpio

"""
class Driver:
    def __init__(self):
        GPIO.setmode(GPIO.BOARD)
        self.R_EN = 15
        self.L_EN = 16
        self.RPWM = 36
        self.LPWM = 37
        GPIO.setup(self.R_EN, GPIO.OUT)
        GPIO.setup(self.RPWM, GPIO.OUT)
        GPIO.setup(self.L_EN, GPIO.OUT)
        GPIO.setup(self.LPWM, GPIO.OUT)
        GPIO.output(self.R_EN, True)
        GPIO.output(self.L_EN, True)

    def neutral(self):
        GPIO.output(self.RPWM, False)  # Stop turning right
        GPIO.output(self.LPWM, False)  # stop turning left

    def right(self):
        GPIO.output(self.LPWM, False)  # stop turning left
        GPIO.output(self.RPWM, True)  # start turning right

    def left(self):
        GPIO.output(self.RPWM, False)  # Stop turning right
        GPIO.output(self.LPWM, True)  # start turning left

    def cleanup(self):
        GPIO.cleanup()

driver = Driver()
driver.right()  # turns right
driver.left()  # turns left
driver.neutral()  # stops turning
driver.cleanup()

"""
# https://sourceforge.net/p/raspberry-gpio-python/wiki/PWM/
"""

import time


# We are going to use the BCM not BOARD layout
# https://i.stack.imgur.com/yHddo.png - BCM are the "GPIO#" ones not the ordered pins
GPIO.setmode(GPIO.BCM)  
GPIO.setwarnings(False)


RPWM = 18;  # GPIO pin 19 to the RPWM on the BTS7960
LPWM = 13;  # GPIO pin 26 to the LPWM on the BTS7960

# For enabling "Left" and "Right" movement
L_EN = 22;  # connect GPIO pin 20 to L_EN on the BTS7960
R_EN = 23;  # connect GPIO pin 21 to R_EN on the BTS7960

GPIO.cleanup()
# Set all of our PINS to output
GPIO.setup(RPWM, GPIO.OUT)
GPIO.setup(LPWM, GPIO.OUT)
GPIO.setup(L_EN, GPIO.OUT)
GPIO.setup(R_EN, GPIO.OUT)


# Enable "Left" and "Right" movement on the HBRidge
GPIO.output(L_EN, True)
GPIO.output(R_EN, True)



rpwm= GPIO.PWM(RPWM, 100)
lpwm= GPIO.PWM(LPWM, 100)

lpwm.ChangeDutyCycle(0)
rpwm.ChangeDutyCycle(0)
lpwm.start(0)
rpwm.start(0)

while 1:

  
  for x in range(20):
    rpwm.ChangeDutyCycle(0)
    print("Speeding up " + str(x))
    lpwm.ChangeDutyCycle(x)
    time.sleep(0.25)

  time.sleep(5)

  for x in range(20):

    print("Slowing down " + str(x))
    lpwm.ChangeDutyCycle(20-x)
    time.sleep(0.25)
"""

import RPi.GPIO as GPIO
import time
from datetime import datetime

class Driver:
    def __init__(self):
        GPIO.setmode(GPIO.BOARD)
        self.R_EN = 15
        self.L_EN = 16
        self.RPWM = 33
        self.LPWM = 12

        GPIO.setup(self.R_EN, GPIO.OUT)
        GPIO.setup(self.RPWM, GPIO.OUT)
        GPIO.setup(self.L_EN, GPIO.OUT)
        GPIO.setup(self.LPWM, GPIO.OUT)

        # Initialize PWM for the motors
        self.r_pwm = GPIO.PWM(self.RPWM, 100)  # 100 Hz frequency
        self.l_pwm = GPIO.PWM(self.LPWM, 100)

        # Start with 0% duty cycle (motor stopped)
        self.r_pwm.start(0)
        self.l_pwm.start(0)

        GPIO.output(self.R_EN, True)
        GPIO.output(self.L_EN, True)

    def neutral(self):
        self.r_pwm.ChangeDutyCycle(0)  # Stop turning right
        self.l_pwm.ChangeDutyCycle(0)  # Stop turning left

    def right(self, speed):
        self.l_pwm.ChangeDutyCycle(0)  # Stop turning left
        self.r_pwm.ChangeDutyCycle(speed)  # Set the speed for turning right

    def left(self, speed):
        self.r_pwm.ChangeDutyCycle(0)  # Stop turning right
        self.l_pwm.ChangeDutyCycle(speed)  # Set the speed for turning left

    def cleanup(self):
        self.r_pwm.stop()
        self.l_pwm.stop()
        GPIO.cleanup()

    def vibratefor(self, seconds):
        # Get the current time
        current_time = datetime.now().timestamp()
        while True:
            updated_time = datetime.now().timestamp()
            if updated_time - current_time > seconds:
                break
            # Move right with speed 50 (you can adjust the speed)
            driver.left(3)
            time.sleep(0.1)  # Run for 2 seconds

            # Stop
            driver.neutral()
            time.sleep(0.01)

            # Move left with speed 75 (you can adjust the speed)
            driver.right(3)
            time.sleep(0.1)  # Run for 2 seconds

            # Stop and clean up
            driver.neutral()
            time.sleep(0.01)


# Example usage
try:
    driver = Driver()

    driver.right(3)
    time.sleep(3)
    driver.vibratefor(0.5)
    driver.right(3)
    time.sleep(3)
    driver.vibratefor(0.5)
    driver.left(3)
    time.sleep(3)
    driver.neutral()
    driver.cleanup()

except KeyboardInterrupt:
    GPIO.cleanup()
