#include <iostream>
#include <random>

class ISensor {
   public:
    virtual double GetData() = 0;
};

class FahrenheitSensor {
   public:
    FahrenheitSensor()
        : generator_(std::random_device{}()), distribution_(64, 86) {}

    double GetData() {
        return distribution_(generator_);
    }

   private:
    std::mt19937 generator_;
    std::uniform_real_distribution<double> distribution_;
};

class CelsiusSensorAdapter : public ISensor {
   public:
    CelsiusSensorAdapter(FahrenheitSensor sensor)
        : sensor_(std::move(sensor)) {}

    double GetData() override {
        return 5.0 * (sensor_.GetData() - 32.0) / 9.0;
    }

   private:
    FahrenheitSensor sensor_;
};

class IoTSystem {
   public:
    void LogSensorData(ISensor& sensor) {
        std::cout << "Sensor data: " << sensor.GetData() << std::endl;
    }
};

int main() {
    FahrenheitSensor sensor;
    CelsiusSensorAdapter adapter{sensor};
    IoTSystem system;
    system.LogSensorData(adapter);
}
