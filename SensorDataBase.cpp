// SensorDataBase.cpp
#include "SensorDataBase.h"
#include <iostream>

SensorDataBase::SensorDataBase(int maxSensors) : count(0), maxSensors(maxSensors) {
    sensors = new Sensor*[maxSensors];
    for (int i = 0; i < maxSensors; i++)
        sensors[i] = nullptr;
}

SensorDataBase::~SensorDataBase() {
    for (int i = 0; i < maxSensors; i++) {
        delete sensors[i];
        sensors[i] = nullptr;
    }
    delete[] sensors;
}

int SensorDataBase::findIndexById(const std::string& id) const {
    for (int i = 0; i < maxSensors; i++) {
        if (sensors[i] != nullptr && sensors[i]->getId() == id)
            return i;
    }
    return -1;
}

bool SensorDataBase::registerSensor(const std::string& id, const std::string& type, const std::string& location) {
    if (count >= maxSensors) {
        std::cout << "[ERROR] Database is full. Cannot register sensor '" << id << "'.\n";
        return false;
    }
    if (findIndexById(id) != -1) {
        std::cout << "[ERROR] Sensor with ID '" << id << "' already exists.\n";
        return false;
    }

    for (int i = 0; i < maxSensors; i++) {
        if (sensors[i] == nullptr) {
            sensors[i] = new Sensor(id, type, location);
            count++;
            std::cout << "[OK] Sensor '" << id << "' registered successfully.\n";
            return true;
        }
    }
    return false;
}

Sensor* SensorDataBase::findById(const std::string& id) const {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' not found.\n";
        return nullptr;
    }
    return sensors[index];
}

bool SensorDataBase::removeById(const std::string& id) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' not found. Nothing removed.\n";
        return false;
    }

    delete sensors[index];
    sensors[index] = nullptr;
    count--;
    std::cout << "[OK] Sensor '" << id << "' removed successfully.\n";
    return true;
}

bool SensorDataBase::updateReading(const std::string& id, double value) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' not found. Reading not updated.\n";
        return false;
    }

    sensors[index]->atualizarLeitura(value);
    std::cout << "[OK] Sensor '" << id << "' updated with value " << value << ".\n";
    return true;
}

void SensorDataBase::listAllIds() const {
    if (count == 0) {
        std::cout << "[INFO] No sensors registered.\n";
        return;
    }

    std::cout << "[INFO] Registered sensors (" << count << "/" << maxSensors << "):\n";
    for (int i = 0; i < maxSensors; i++) {
        if (sensors[i] != nullptr)
            std::cout << "  - " << sensors[i]->getId() << "\n";
    }
}