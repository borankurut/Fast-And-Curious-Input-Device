#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Define a struct to hold sensor data
struct SensorData {
    double pot0Value;
    double pot1Value;
    double position;
};

// Function to convert byte array to SensorData struct
SensorData deserializeSensorData(const std::string& data) {
    SensorData sensorData;
    std::memcpy(&sensorData, data.data(), sizeof(SensorData));
    return sensorData;
}

int main() {
    asio::io_context ioContext;
    tcp::socket socket(ioContext);

    try {
        // Connect to the server
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("192.168.137.227"), 2222));

        while (true) {
            // Receive sensor data from the server
            char buffer[sizeof(SensorData)];
            boost::asio::read(socket, boost::asio::buffer(buffer, sizeof(SensorData)));

            // Convert the byte array to the struct
            SensorData sensorData = deserializeSensorData(buffer);

            // Display received sensor data
            std::cout << "Pot0: " << sensorData.pot0Value << ", Pot1: " << sensorData.pot1Value
                      << ", Position: " << sensorData.position << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
