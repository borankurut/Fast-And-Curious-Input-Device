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

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT	 5002
#define MAXLINE 1024 

struct VibData{
	bool isActive;
	
	};
class Driver {
public:
    int status;
    float* sSpeed;
    Driver() {
        wiringPiSetup();
        R_EN = 3;
        L_EN = 4;
        RPWM = 23;  // Assuming GPIO 1 for RPWM
        LPWM = 1; // Assuming GPIO 26 for LPWM
        
        status = 0;

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
        
        key_t key = ftok("shmfile", 65);

        // shmget returns an identifier in shmid
        int shmid = shmget(key, 2 * sizeof(float) + sizeof(double), 0666 | IPC_CREAT);

        sSpeed = (float*)shmat(shmid, nullptr, 0);

    }

    void neutral() {
        softPwmWrite(RPWM, 0);  // Stop turning right
        softPwmWrite(LPWM, 0);  // Stop turning left
        status = 0;
    
    }
    
    void instantStop(){
        while(*sSpeed > 0){
            left(5);
        }
        while(*sSpeed<0){
            right(5);
        }
    }

    void right(int speed) {
        softPwmWrite(LPWM, 0);  // Stop turning left
        softPwmWrite(RPWM, speed);  // Set the speed for turning right
        status = 1;
    }

    void left(int speed) {
        softPwmWrite(RPWM, 0);  // Stop turning right
        softPwmWrite(LPWM, speed);  // Set the speed for turning left
        status = -1;
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
    
    struct hostent *he=gethostbyname("EDA.local");
    char *ip=inet_ntoa(*(struct in_addr*)he->h_addr_list[0]);
    
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
	servaddr.sin_addr.s_addr = inet_addr(ip); 
    VibData vibData;
	int n;
	socklen_t len; 
	
	std::cout<<"Connected to the server."<<std::endl; 
 
 
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 2 * sizeof(float) + sizeof(double), 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    float* notImp = (float*)shmat(shmid, nullptr, 0);
    double* sAngle = (double*)(notImp + 1);  // Offset to get the next location in shared memory
 
    double lastAngle = *sAngle;
    
    Driver driver;

    const double offset = 10.0;
    bool off = false;
    int turnPower = 3;
    
    while(true){
        if(*sAngle < offset+20 && *sAngle > -offset-20)
            turnPower = 3;
        else 
            turnPower = 3;
        
        
        sendto(sockfd, (const char *)hello, strlen(hello), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); 
	
	
        n = recvfrom(sockfd, &vibData, sizeof(vibData), 
				MSG_WAITALL, (struct sockaddr *) &servaddr, 
				&len); 
       
        if(vibData.isActive==true)
            driver.vibrateFor(0.5);
            
        if(*sAngle < -offset || *sAngle > offset)
                        off = true;

        if(!off)
                driver.neutral();

        else if(off && *sAngle > 7){
                driver.left(turnPower);
        }

        else if(off && *sAngle < -7){
                driver.right(turnPower);
        }

        if(                     *sAngle < 0 && driver.status == -1
                        ||      *sAngle > 0 && driver.status == 1
                        ||      driver.status == 0)
        { 
            
            off = false;
             
            if(driver.status != 0)
                driver.instantStop();
                
            driver.neutral();
        }  
    }
    driver.neutral();
    driver.cleanup();
    
    
    /*
     * ***************************
     * 
        if(*sAngle < -offset){
            std::cout<<*sAngle << std::endl;
            if(*sAngle < 0)
                driver.right(3);
        }
        else if(*sAngle > offset){
            std::cout<<*sAngle << std::endl;
            if(*sAngle > 0)
                driver.left(3);
        }
        else
            driver.neutral();
        
     * **************************
     * 
      double offset = 10.0;
    double deadband = 0.1;  // Adjust this value based on your system's sensitivity

    // Read the current wheel angle from the encoder
    double currentAngle = readWheelAngle();

    // Calculate the desired motor position based on the offset
    double desiredPosition = offset;

    // Check if the current angle is within the deadband around the desired position
    if (std::abs(currentAngle - desiredPosition) < deadband) {
        stopMotor();
    } else {
        // Determine the direction to move the motor based on the current angle
        if (currentAngle > desiredPosition) {
            moveMotorLeft();
        } else {
            moveMotorRight();
        }
    }
     * 
     * */

    return 0;
}
