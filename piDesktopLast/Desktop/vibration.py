from gpiozero import PWMOutputDevice
import time

motor = PWMOutputDevice(18)

for i in range(100):
	motor.value = 1

	time.sleep(1)

	motor.value = 0

	time.sleep(1)
