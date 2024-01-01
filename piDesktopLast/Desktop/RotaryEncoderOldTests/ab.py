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
last_state_B = GPIO.input(B_PIN)

try:
  while True:
    current_state_A = GPIO.input(A_PIN)
    current_state_B = GPIO.input(B_PIN)
    
    if current_state_A == last_state_A and current_state_B == last_state_B:
      continue
    
    if last_state_A == 0 and last_state_B == 0:
      if current_state_A == 1 and current_state_B == 0:
        position -= 1
      elif current_state_A == 0 and current_state_B == 1:
        position += 1
      
    if last_state_A == 1 and last_state_B == 1:
      if current_state_A == 0 and current_state_B == 1:
        position -= 1
      elif current_state_A == 1 and current_state_B == 0:
        position += 1
      
    if last_state_A == 0 and last_state_B == 1:
      if current_state_A == 0 and current_state_B == 0:
        position -= 1
      elif current_state_A == 1 and current_state_B == 1:
        position += 1
      
    if last_state_A == 1 and last_state_B == 0:
      if current_state_A == 1 and current_state_B == 1:
        position -= 1
      elif current_state_A == 0 and current_state_B == 0:
        position += 1
    
    last_state_A = current_state_A
    last_state_B = current_state_B
    print(position)

except KeyboardInterrupt:
    GPIO.cleanup()
