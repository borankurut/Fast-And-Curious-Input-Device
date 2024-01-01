#include <wiringPi.h>
#include <iostream>

#define A_PIN 0  // Assuming GPIO pin 0 corresponds to pin 11 in BOARD mode
#define B_PIN 2  // Assuming GPIO pin 2 corresponds to pin 13 in BOARD mode

int main() {
    wiringPiSetup();
    pinMode(A_PIN, INPUT);
    pinMode(B_PIN, INPUT);

    int position = 0;
    int lastStateA = digitalRead(A_PIN);
    int lastStateB = digitalRead(B_PIN);

    while (true) {
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

        std::cout << (static_cast<double>(position) / 4096) * 360 << std::endl;

    }

    return 0;
}
