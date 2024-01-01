#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <softPwm.h>
#include <wiringPi.h>

#define MOTOR_PIN 1  // Assuming GPIO pin 1 corresponds to pin 18

struct VibData {
    bool isActive;
    int level;
    
};

int main() {
	
	wiringPiSetup();
    softPwmCreate(MOTOR_PIN, 0, 100);  // Initialize PWM pin with range 0-100
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Set up server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5002); // Use the same port as the server
    serverAddr.sin_addr.s_addr = inet_addr("192.168.137.62"); // Replace with Raspberry Pi's IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to server");
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Receive sensor data in a loop
    while (true) {
        VibData vibData;
        // Receive data from the server
        recv(clientSocket, &vibData, sizeof(vibData), 0);

		if(vibData.isActive == false)
			vibData.level = 0;
		
		softPwmWrite(MOTOR_PIN, vibData.level);

        // Process and use sensorData as needed

        // Add a delay or use a proper synchronization mechanism as needed
        usleep(1000000); // 1 second delay
    }

    // Close socket
    close(clientSocket);

    return 0;
}
