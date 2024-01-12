#include <iostream>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <wiringPi.h>
#include <chrono>
#include <thread>

using namespace std;

#define A_PIN 0  // Assuming GPIO pin 0 corresponds to pin 11 in BOARD mode
#define B_PIN 2  // Assuming GPIO pin 2 corresponds to pin 13 in BOARD mode

int main()
{
    wiringPiSetup();
    pinMode(A_PIN, INPUT);
    pinMode(B_PIN, INPUT);
    
    int lastStateA = digitalRead(A_PIN);
    int lastStateB = digitalRead(B_PIN);
    
      key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 2 * sizeof(float) + sizeof(double), 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    float* sSpeed = (float*)shmat(shmid, nullptr, 0);
    double* sAngle = (double*)(sSpeed + 1);  // Offset to get the next location in shared memory

    // Example: Set values for speed and angle
    *sSpeed = 0.0f;
    *sAngle = 0.0f;
    
    double position = 0.0;
    double last_angle = *sAngle;
    
    auto startTime = std::chrono::high_resolution_clock::now();

        while(true){//update position
            
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0;
            
            if(elapsedTime >= 0.05){
                *sSpeed = *sAngle - last_angle;
                startTime = currentTime;
                last_angle = *sAngle;
            }
            
            int currentStateA = digitalRead(A_PIN);
            int currentStateB = digitalRead(B_PIN);

            if (currentStateA == lastStateA && currentStateB == lastStateB) {
                continue;
            }

            if (lastStateA == LOW) {
                if (currentStateB == LOW) {
                    position -= 1;
                } else {
                    position += 1;
                }
            } else {
                if (currentStateB == HIGH) {
                    position -= 1;
                } else {
                    position += 1;
                }
            }
    
    
            lastStateA = currentStateA;
            lastStateB = currentStateB;
            *sAngle = (position / 4096) * 360;
            
            
            cout << "Write Data : ";
            //cin.getline(str, 1024);
            //logic
            //str=position
            cout << "pos:" << *sAngle << endl;
            //cout << "speed:" << *sSpeed << endl;
            
        }
    // detach from shared memory
    shmdt(sAngle);

    return 0;
}
