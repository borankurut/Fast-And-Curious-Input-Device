// Client side implementation of UDP client-server model 
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

#define MOTOR_PIN 1
#define PORT	 5002
#define MAXLINE 1024 
struct VibData{
	bool isActive;
	int level;
	
	};
	
int main() { 
	wiringPiSetup();
    softPwmCreate(MOTOR_PIN, 0, 100);
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
	servaddr.sin_addr.s_addr = inet_addr("192.168.137.62"); 
	VibData vibData;
	int n;
	socklen_t len; 
	
	std::cout<<"Connected to the server."<<std::endl; 
	
	while(true)
	{
		
		
	sendto(sockfd, (const char *)hello, strlen(hello), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); 
	
	
	n = recvfrom(sockfd, &vibData, sizeof(vibData), 
				MSG_WAITALL, (struct sockaddr *) &servaddr, 
				&len); 
	if (!vibData.isActive)
            vibData.level = 0;

        softPwmWrite(MOTOR_PIN, vibData.level);

	std::cout<<"isActive: "<< vibData.isActive<<"\tlevel: "<<vibData.level<<std::endl; 

        usleep(1000000); 
	}
	
	close(sockfd); 
	return 0; 
}
