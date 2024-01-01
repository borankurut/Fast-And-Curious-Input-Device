
#include <iostream>
#include <boost/asio.hpp>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
double *sPosition;
sem_t *sem;

using boost::asio::ip::tcp;

struct SensorData {
    double sw_angle;
    double gas;
    double brake;
};




int main() {
    boost::asio::io_context ioContext;
	tcp::acceptor acceptor(ioContext, tcp::endpoint(boost::asio::ip::make_address("192.168.137.227"), 5001));

    std::cout << "Server listening on port 8080..." << std::endl;

    tcp::socket socket(ioContext);
    acceptor.accept(socket);

    std::cout << "Client connected!" << std::endl;

    // Send sensor data in a loop
    while (true) {
		
        SensorData sensorData;
	
	std::cout<< sensorData.sw_angle << "\n";
	sem_wait(sem);
	sensorData.sw_angle = *sPosition;
	sem_post(sem);
	sensorData.gas=0;
	sensorData.brake = 0;
	
	
        // Code to populate sensorData with actual sensor values

        // Send data to the client
        boost::asio::write(socket, boost::asio::buffer(&sensorData, sizeof(sensorData)));

    }

    return 0;
}
