import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

# Define the GPIO pins for A and B channels
A_PIN = 11
B_PIN = 13

# Setup GPIO pins as inputs
GPIO.setup(A_PIN, GPIO.IN)
GPIO.setup(B_PIN, GPIO.IN)

# Initialize variables
position = 0
last_state_A = GPIO.input(A_PIN)

try:
    while True:
        # Read the current state of A and B channels
        state_A = GPIO.input(A_PIN)
        state_B = GPIO.input(B_PIN)

        # Check for state changes on channel A
        if state_A != last_state_A:
            if state_A != state_B:
                print("sssrg")
                position += 1
            else:
                position -= 1

            print(f"Current Position: {position}")

        last_state_A = state_A
        time.sleep(0.001)  # Adjust the sleep time as needed
except KeyboardInterrupt:
    GPIO.cleanup()
except Exception as e:
    print(f"An error occurred: {e}")
    GPIO.cleanup()
