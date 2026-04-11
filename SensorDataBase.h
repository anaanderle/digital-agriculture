// SensorDataBase.h
#pragma once

#include "Sensor.h"

class SensorDataBase {
private:
    Sensor** sensors;
    bool*    deleted;
    int      count;
    int      maxSensors;

    int h1(unsigned int k) const;
    int h2(unsigned int k) const;
    int doubleHash(unsigned int k, int i) const;
    int findIndexById(int id) const;

public:
    SensorDataBase(int maxSensors);
    ~SensorDataBase();

    bool     registerSensor(int id, const std::string& type, const std::string& location);
    Sensor*  findById(int id) const;
    bool     removeById(int id);
    bool     updateReading(int id, double value);
    void     listAllIds() const;
};