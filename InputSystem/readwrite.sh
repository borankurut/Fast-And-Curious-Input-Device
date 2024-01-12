# Compile writer.cpp
g++ -o writer writerEncoder.cpp -lrt -lpthread -lwiringPi

# Compile reader.cpp
g++ -o reader readerEncoderUdp.cpp MCP3008/src/MCP3008.cpp -lrt -lpthread -llgpio -lboost_system

# Open terminals and run the compiled programs
lxterminal --title="Writer Terminal" --command="./writer"
lxterminal --title="Reader Terminal" --command="./reader"
