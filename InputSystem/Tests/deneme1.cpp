#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wiringPi.h>

#define A_PIN 0  // Replace with your actual GPIO pin numbers
#define B_PIN 2

using boost::asio::ip::tcp;

struct SensorData {
    double position;
    // Add other sensor data members as needed
};

int main() {
    // Create shared memory for sensor data
    key_t key = ftok("/tmp", 'S');
    int shmid = shmget(key, sizeof(SensorData), IPC_CREAT | 0666);
    SensorData* sharedData = static_cast<SensorData*>(shmat(shmid, nullptr, 0));

    // Fork a new process
    pid_t pid = fork();

    if (pid == 0) {
        // Child process: Handle GPIO and rotary encoder logic
        wiringPiSetup();
        pinMode(A_PIN, INPUT);
        pinMode(B_PIN, INPUT);

        int position = 0;
        int last_state_A = digitalRead(A_PIN);
        int last_state_B = digitalRead(B_PIN);

        while (true) {
            int current_state_A = digitalRead(A_PIN);
            int current_state_B = digitalRead(B_PIN);

            if (current_state_A == last_state_A && current_state_B == last_state_B) {
                continue;
            }

            if (last_state_A == 0) {
                if (current_state_B == 0) {
                    position -= 1;
                } else {
                    position += 1;
                }
            } else {
                if (current_state_B == 1) {
                    position -= 1;
                } else {
                    position += 1;
                }
            }

            last_state_A = current_state_A;
            last_state_B = current_state_B;

            // Update shared memory with position
            sharedData->position = static_cast<double>(position);

            // Add a delay or use a proper synchronization mechanism as needed
            std::cout<<position<<std::endl;
        }
    } else if (pid > 0) {
        // Parent process: Handle Boost ASIO server
        boost::asio::io_context ioContext;
        tcp::acceptor acceptor(ioContext, tcp::endpoint(boost::asio::ip::make_address("192.168.137.227"), 5001));

        std::cout << "Server listening on port 8080..." << std::endl;

        tcp::socket socket(ioContext);
        acceptor.accept(socket);

        std::cout << "Client connected!" << std::endl;

        // Send sensor data in a loop
        while (true) {
            // Read shared data
            SensorData sensorData = *sharedData;

            // Send data to the client
            boost::asio::write(socket, boost::asio::buffer(&sensorData, sizeof(sensorData)));

            // Add a delay or use a proper synchronization mechanism as needed
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } else {
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    }

    // Clean up shared memory
    shmdt(sharedData);
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
