import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

# Define the GPIO pins for A and B channels
A_PIN = 11
B_PIN = 12

# Setup GPIO pins as inputs
GPIO.setup(A_PIN, GPIO.IN)
GPIO.setup(B_PIN, GPIO.IN)

# Initialize variables
position = 0
last_state_A = GPIO.input(A_PIN)

try:
    while True:
        print("A: {}, B: {}".format(GPIO.input(A_PIN), GPIO.input(B_PIN)))
        
'''


'''
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

A_PIN = 12
B_PIN = 11

GPIO.setup(A_PIN, GPIO.IN)
GPIO.setup(B_PIN, GPIO.IN)

try:
    while True:
        print("A: {}, B: {}".format(GPIO.input(A_PIN), GPIO.input(B_PIN)))
        

except KeyboardInterrupt:
    GPIO.cleanup()
'''
'''
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

# Define the GPIO pin you want to measure
target_pin = 38 # Change this to your GPIO pin

# Setup the GPIO pin as input
GPIO.setup(target_pin, GPIO.IN)

try:
    while True:
        # Read the voltage on the GPIO pin
        voltage = GPIO.input(target_pin)
        print(f"Voltage on GPIO pin {target_pin}: {voltage}")
        time.sleep(1)

except KeyboardInterrupt:
    GPIO.cleanup()
except Exception as e:
    print(f"An error occurred: {e}")
    GPIO.cleanup()

