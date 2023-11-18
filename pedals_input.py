from gpiozero import MCP3008
import time

# potantiometers initialization
pot0 = MCP3008(0) # mcp channel 0
pot1 = MCP3008(1) # mcp channel 1


while True:
    
    print("first: " + str(pot0.value))
    print("second: " + str(pot1.value))
    print()

    time.sleep(0.3)

