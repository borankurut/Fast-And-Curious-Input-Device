#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <chrono>
#include <wiringPi.h>
#include <softPwm.h>

#define MOTOR_PIN 1  // Assuming GPIO pin 1 corresponds to pin 18

using boost::asio::ip::tcp;

struct VibData {
    bool isActive;
    int level;
    
};


int main() {
    boost::asio::io_context ioContext;
    tcp::socket socket(ioContext);
    tcp::resolver resolver(ioContext);

    // Replace "RaspberryPi_IP_Address" with the actual IP address of your Raspberry Pi
    auto endpoints = resolver.resolve("192.168.137.62", "5002");

    boost::asio::connect(socket, endpoints);

    std::cout << "Connected to server!" << std::endl;

    // Receive sensor data in a loop
    while (true) {
        VibData vibData;
        // Receive data from the server
        boost::asio::read(socket, boost::asio::buffer(&vibData, sizeof(vibData)));

		if(vibData.isActive == false)
			vibData.level = 0;
		
		softPwmWrite(MOTOR_PIN, vibData.level);
		

        // Add a delay or use a proper synchronization mechanism as needed
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

        return 0;
}
