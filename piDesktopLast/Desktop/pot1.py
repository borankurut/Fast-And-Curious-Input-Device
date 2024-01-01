import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

# Define the GPIO pins for A and B channels
A_PIN = 32

# Setup GPIO pins as inputs
GPIO.setup(A_PIN, GPIO.IN)

last_state_A = GPIO.input(A_PIN)

try:
    while True:
        print("A: {}".format(GPIO.input(A_PIN)))
       
except KeyboardInterrupt:
    GPIO.cleanup()
