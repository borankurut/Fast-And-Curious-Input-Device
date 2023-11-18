import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

# phase a and phase b pins. 
A_PIN = 11
B_PIN = 13

GPIO.setup(A_PIN, GPIO.IN)
GPIO.setup(B_PIN, GPIO.IN)

# position is the current sum of decoded phase values
position = 0
last_state_A = GPIO.input(A_PIN)
last_state_B = GPIO.input(B_PIN)


while True:     # todo: maybe refactor later
  current_state_A = GPIO.input(A_PIN)
  current_state_B = GPIO.input(B_PIN)
  
  if current_state_A == last_state_A and current_state_B == last_state_B:
    continue
  
  if last_state_A == 0:
    if current_state_B == 0:
      position -= 1
    else:
      position += 1
    
  else:
    if current_state_B == 1:
      position -= 1
    else:
      position += 1

  last_state_A = current_state_A
  last_state_B = current_state_B

  angle = (position/4090)*360

  print(angle)
    
