// SensorDataBase.h
#pragma once

#include "Sensor.h"

class SensorDataBase {
private:
    Sensor** sensors;
    int count;
    int maxSensors;

    int findIndexById(const std::string& id) const;

public:
    SensorDataBase(int maxSensors);
    ~SensorDataBase();

    bool registerSensor(const std::string& id, const std::string& type, const std::string& location);
    Sensor* findById(const std::string& id) const;
    bool removeById(const std::string& id);
    bool updateReading(const std::string& id, double value);
    void listAllIds() const;
};