#include <iostream>
#include <gpiod.h>
#include <chrono>
#include <thread>

int main() {
    // Define the GPIO chip name (replace with the appropriate name)
    const char* chipname = "gpiochip0";

    // Define the GPIO line offsets for A and B channels
    const int A_PIN_OFFSET = 11;  // Replace with your actual GPIO line offsets
    const int B_PIN_OFFSET = 12;

    // Create a gpiod_chip structure and open it
    struct gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip" << std::endl;
        return 1;
    }

    // Request A and B lines for input
    struct gpiod_line* line_A = gpiod_chip_get_line(chip, A_PIN_OFFSET);
    struct gpiod_line* line_B = gpiod_chip_get_line(chip, B_PIN_OFFSET);
    if (!line_A || !line_B) {
        std::cerr << "Failed to request GPIO lines" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Configure lines for input
    if (gpiod_line_request_input(line_A, "A_PIN") || gpiod_line_request_input(line_B, "B_PIN")) {
        std::cerr << "Failed to configure GPIO lines for input" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    int position = 0;
    int lastStateA = gpiod_line_get_value(line_A);
    int lastStateB = gpiod_line_get_value(line_B);

    while (true) {
        int currentStateA = gpiod_line_get_value(line_A);
        int currentStateB = gpiod_line_get_value(line_B);

        if (currentStateA == lastStateA && currentStateB == lastStateB) {
            continue;
        }

        if (lastStateA == 0) {
            if (currentStateB == 0) {
                position -= 1;
            } else {
                position += 1;
            }
        } else {
            if (currentStateB == 1) {
                position -= 1;
            } else {
                position += 1;
            }
        }

        lastStateA = currentStateA;
        lastStateB = currentStateB;
        std::cout << "Position: " << position << std::endl;

        // Sleep briefly to avoid rapid position changes due to contact bounce
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Clean up and close the GPIO lines and chip
    gpiod_line_release(line_A);
    gpiod_line_release(line_B);
    gpiod_chip_close(chip);

    return 0;
}
