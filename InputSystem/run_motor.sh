# Compile motor.cpp
g++ -o motor_out motor.cpp -lwiringPi

lxterminal --title="Motor Terminal" --command="./motor_out"
