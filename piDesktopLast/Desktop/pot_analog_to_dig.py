from gpiozero import MCP3008
import time

pot0 = MCP3008(0)
pot1 = MCP3008(1)


while True:
    
    print("first: " + str(pot0.value))
    print("second: " + str(pot1.value))
    print()
    print()
    """
    print(pot1.value)
    print(pot2.value)
    print(pot3.value)
    print(pot4.value)
    print(pot5.value)
    print(pot6.value)
    print(pot7.value)
    """
    time.sleep(0.3)
