from gpiozero import PWMOutputDevice
import time

motor = PWMOutputDevice(18) # clock input 

for i in range(100):
	motor.value = 1         # between 0 and 1, for setting vibration level

	time.sleep(1)

	motor.value = 0

	time.sleep(1)

