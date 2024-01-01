#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PORT	 5002
#define MAXLINE 1024 

struct VibData{
	bool isActive;
	
	};
class Driver {
public:
    Driver() {
        wiringPiSetup();
        R_EN = 3;
        L_EN = 4;
        RPWM = 23;  // Assuming GPIO 1 for RPWM
        LPWM = 1; // Assuming GPIO 26 for LPWM

        pinMode(R_EN, OUTPUT);
        pinMode(L_EN, OUTPUT);
        pinMode(RPWM, OUTPUT);
        pinMode(LPWM, OUTPUT);

        // Initialize PWM for the motors
        softPwmCreate(RPWM, 0, 100);  // Pin, Initial Value, Range (0-100)
        softPwmCreate(LPWM, 0, 100);

        // Start with 0% duty cycle (motor stopped)
        softPwmWrite(RPWM, 0);
        softPwmWrite(LPWM, 0);

        digitalWrite(R_EN, HIGH);
        digitalWrite(L_EN, HIGH);
    }

    void neutral() {
        softPwmWrite(RPWM, 0);  // Stop turning right
        softPwmWrite(LPWM, 0);  // Stop turning left
    }

    void right(int speed) {
        softPwmWrite(LPWM, 0);  // Stop turning left
        softPwmWrite(RPWM, speed);  // Set the speed for turning right
    }

    void left(int speed) {
        softPwmWrite(RPWM, 0);  // Stop turning right
        softPwmWrite(LPWM, speed);  // Set the speed for turning left
    }

    void cleanup() {
        softPwmWrite(RPWM, 0);
        softPwmWrite(LPWM, 0);
        digitalWrite(R_EN, LOW);
        digitalWrite(L_EN, LOW);
    }

    void vibrateFor(double seconds) {
        auto startTime = std::chrono::high_resolution_clock::now();
        while (true) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0;
            if (elapsedTime > seconds) {
                break;
            }

            left(5);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            neutral();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            right(5);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            neutral();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

private:
    int R_EN;
    int L_EN;
    int RPWM;
    int LPWM;
};

int main() {
    
    int sockfd; 
	char buffer[MAXLINE]; 
	const char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 
 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
    
    memset(&servaddr, 0, sizeof(servaddr)); 
	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr("192.168.137.106"); 
    VibData vibData;
	int n;
	socklen_t len; 
	
	std::cout<<"Connected to the server."<<std::endl; 
    
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    double* sAngle = (double*)shmat(shmid, nullptr, 0);
    
    Driver driver;

    const int offset = 10;
    /*
    driver.right(3);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    driver.vibrateFor(0.5);

    driver.right(3);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    driver.vibrateFor(0.5);

    driver.left(3);
    std::this_thread::sleep_for(std::chrono::seconds(3));*/
    
    while(true){
        
        
        sendto(sockfd, (const char *)hello, strlen(hello), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); 
	
	
	n = recvfrom(sockfd, &vibData, sizeof(vibData), 
				MSG_WAITALL, (struct sockaddr *) &servaddr, 
				&len); 
        if(vibData.isActive==true)
            driver.vibrateFor(1.2);
            
        if(*sAngle < -offset){
            if(*sAngle < 0)
                driver.right(3);
        }
        else if(*sAngle > offset){
            if(*sAngle > 0)
                driver.left(3);
        }
        else
            driver.neutral();
    }
    driver.neutral();
    driver.cleanup();
    

    return 0;
}
