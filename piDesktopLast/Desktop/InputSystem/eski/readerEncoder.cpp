#include <iostream>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <boost/asio.hpp>
#include "MCP3008/include/MCP3008.h"
using namespace MCP3008Lib;
using namespace std;
using boost::asio::ip::tcp;
struct SensorData {
    double sw_angle;
    double gas;
    double brake;
};


int main(){
    MCP3008 adc;
	adc.connect();
    
    boost::asio::io_context ioContext;
	tcp::acceptor acceptor(ioContext, tcp::endpoint(boost::asio::ip::make_address("192.168.137.78"), 5001));

    std::cout << "Server listening on port 8080..." << std::endl;

    tcp::socket socket(ioContext);
    acceptor.accept(socket);

    std::cout << "Client connected!" << std::endl;
    
    // ftok to generate unique key
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    double* sAngle = (double*)shmat(shmid, nullptr, 0);
    SensorData sensorData;
    while(true){
        
        sensorData.sw_angle = *sAngle;
        sensorData.gas= adc.read(0);
        sensorData.brake = adc.read(1);
        
            cout << "pos:" << *sAngle << endl; //sended data read
        
        boost::asio::write(socket, boost::asio::buffer(&sensorData, sizeof(sensorData)));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //cout << "Data read from memory:" << *sAngle << endl;
    }
    // detach from shared memory
    shmdt(sAngle);

    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
