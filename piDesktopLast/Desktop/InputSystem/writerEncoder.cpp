#include <iostream>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <wiringPi.h>

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

    // ftok to generate unique key
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    double* sAngle = (double*)shmat(shmid, nullptr, 0);
    // shmat to attach to shared memory
    //char* str = (char*)shmat(shmid, (void*)0, 0);
    
    double position = 0.0;

        while(true){//update position
            
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
        }
    // detach from shared memory
    shmdt(sAngle);

    return 0;
}
