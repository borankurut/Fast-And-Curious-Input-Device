// Server side implementation of UDP client-server model
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "MCP3008/include/MCP3008.h"
using namespace MCP3008Lib;
using namespace std;

#define PORT    5002
#define MAXLINE 1024

struct SensorData {
    double sw_angle;
    double gas;
    double brake;
};

// Driver code
int main() {
	MCP3008 adc;
	adc.connect();
    
        int sockfd;
        char buffer[MAXLINE];
//      char hello[] = "Hello from server";
        struct sockaddr_in servaddr, cliaddr;

        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(PORT);

        // Bind the socket with the server address
        if ( bind(sockfd, (const struct sockaddr *)&servaddr,
                        sizeof(servaddr)) < 0 )
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }


        socklen_t len;
int n;

        len = sizeof(cliaddr); //len is value/result


      key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 2 * sizeof(float) + sizeof(double), 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    float* sSpeed = (float*)shmat(shmid, nullptr, 0);
    double* sAngle = (double*)(sSpeed + 1);  // Offset to get the next location in shared memory
    
    SensorData sensorData;


        while(true){
			sensorData.sw_angle = *sAngle;
        sensorData.gas= adc.read(0);
        sensorData.brake = adc.read(1);
			
                n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                                        MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                                        &len);
                buffer[n] = '\0';


                printf("Client : %s\n", buffer);
                sendto(sockfd, &sensorData, sizeof(sensorData),
                        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                                len);
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        return 0;
}
