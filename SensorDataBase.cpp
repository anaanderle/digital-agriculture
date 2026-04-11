#include "SensorDataBase.h"
#include <iostream>

SensorDataBase::SensorDataBase(int maxSensors) : count(0), maxSensors(maxSensors) {
    sensors = new Sensor*[maxSensors];
    deleted = new bool[maxSensors];

    for (int i = 0; i < maxSensors; i++) {
        sensors[i] = nullptr;
        deleted[i] = false;
    }
}

SensorDataBase::~SensorDataBase() {
    for (int i = 0; i < maxSensors; i++)
        delete sensors[i];

    delete[] sensors;
    delete[] deleted;
}

int SensorDataBase::h1(unsigned int k) const {
    return k % maxSensors;
}

int SensorDataBase::h2(unsigned int k) const {
    return 1 + (k % (maxSensors - 1));
}

int SensorDataBase::doubleHash(unsigned int k, int i) const {
    return (h1(k) + i * h2(k)) % maxSensors;
}

int SensorDataBase::findIndexById(int id) const {
    for (int i = 0; i < maxSensors; i++) {
        int slot = doubleHash(id, i);

        // se a posição é null e nunca foi deletado, nunca foi inserido
        if (sensors[slot] == nullptr && !deleted[slot])
            return -1;

        // se a posição não é null e o id bate, encontrado
        if (sensors[slot] != nullptr && sensors[slot]->getId() == id)
            return slot;

        // continua se
        // a posição é null mas foi deletada (precisa averiguar próximas sondagens)
        // a posição não é null mas o id não bate (precisa averiguar próximas sondagens)
    }
    return -1;
}

int SensorDataBase::registerSensor(int id, const std::string& type, const std::string& location) {
    if (count >= maxSensors) {
        std::cout << "[ERROR] Capacidade máxima de sensores atingida '" << id << "'.\n";
        return -1;
    }

    for (int i = 0; i < maxSensors; i++) {
        int slot = doubleHash(id, i);

        if (sensors[slot] != nullptr && sensors[slot]->getId() == id) {
            std::cout << "[ERROR] Sensor com ID '" << id << "' já existe.\n";
            return -1;
        }

        // posição vazia
        if (sensors[slot] == nullptr) {
            sensors[slot] = new Sensor(id, type, location);
            deleted[slot] = false;
            count++;
            std::cout << "[OK] Sensor '" << id << "' registrado na posição " << slot << ".\n";
            return i;
        }

        // continua se posição não é null
    }

    return -1;
}

Sensor* SensorDataBase::findById(int id) const {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' não encontrado.\n";
        return nullptr;
    }
    return sensors[index];
}

bool SensorDataBase::removeById(int id) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' não encontrado.\n";
        return false;
    }

    delete sensors[index];
    sensors[index] = nullptr;
    deleted[index] = true;
    count--;
    std::cout << "[OK] Sensor '" << id << "' removido da posição " << index << ".\n";
    return true;
}

bool SensorDataBase::updateReading(int id, double value) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' não encontrado.\n";
        return false;
    }

    sensors[index]->updateReading(value);
    std::cout << "[OK] Sensor '" << id << "' atualizado com valor " << value << ".\n";
    return true;
}

void SensorDataBase::listAllIds() const {
    if (count == 0) {
        std::cout << "[INFO] Nenhum sensor registrado.\n";
        return;
    }

    std::cout << "[INFO] Sensores registrados (" << count << "/" << maxSensors << "):\n";
    for (int i = 0; i < maxSensors; i++) {
        if (sensors[i] != nullptr)
            std::cout << "  [posição " << i << "] " << sensors[i]->getId() << "\n";
    }
}