#include <wiringPi.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define A_PIN 0  // Assuming GPIO pin 0 corresponds to pin 11 in BOARD mode
#define B_PIN 2  // Assuming GPIO pin 2 corresponds to pin 13 in BOARD mode

double *sPosition = mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

int main(){
    int lastStateA;
int lastStateB;
 double position = 0.0;

if (sem_init(sem, 1, 1) == -1) {
        perror("Error in sem_init");
        exit(EXIT_FAILURE);
    }

if (sPosition == MAP_FAILED) {
        perror("Error in mmap");
        exit(EXIT_FAILURE);
    }
    wiringPiSetup();
    pinMode(A_PIN, INPUT);
    pinMode(B_PIN, INPUT);
    
    lastStateA = digitalRead(A_PIN);
    lastStateB = digitalRead(B_PIN);
    
    int currentStateA = digitalRead(A_PIN);
    int currentStateB = digitalRead(B_PIN);

while(true){
     currentStateA = digitalRead(A_PIN);
     currentStateB = digitalRead(B_PIN);
    
    if (lastStateA == LOW) {
        if (currentStateB == LOW) {
            position -= 1.0;
        } else {
            position += 1.0;
        }
    } else {
        if (currentStateB == HIGH) {
            position -= 1.0;
        } else {
            position += 1.0;
        }
  
    }
    
        lastStateA = currentStateA;
        lastStateB = currentStateB;

        sem_wait(sem);
        *sPosition = (position / 4096) * 360;
        sem_post(sem);
    
    }
    
    
    // Destroy the semaphore
        if (sem_destroy(sem) == -1) {
            perror("Error in sem_destroy");
            exit(EXIT_FAILURE);
        }

    
}


