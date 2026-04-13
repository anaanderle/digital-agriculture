#include "SensorDataBase.h"
#include <iostream>

namespace
{
    bool isPrime(int number)
    {
        if (number < 2) return false;
        if (number % 2 == 0) return number == 2;

        for (int i = 3; i * i <= number; i += 2)
            if (number % i == 0) return false;

        return true;
    }

    int nextPrime(int number)
    {
        if (number <= 2) return 2;
        if (number % 2 == 0) number++;
        while (!isPrime(number)) number += 2;

        return number;
    }
}



SensorDataBase::SensorDataBase(int maxSensors, bool optimizeMaxSensor) : count(0), maxSensors(optimizeMaxSensor ? nextPrime(maxSensors) : maxSensors) {
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
    if (maxSensors <= 1) return 1; // Evita divisão por zero
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

    // variáveis para manter o primeiro slot "tombstone" encontrado durante a sondagem, caso seja necessário reutilizlizar
    int firstDeletedSlot = -1;
    int firstDeletedCollision = -1;

    for (int i = 0; i < maxSensors; i++) {
        int slot = doubleHash(id, i);

        // Slot ocupado: precisa checar duplicidade
        if (sensors[slot] != nullptr) {
            if (sensors[slot]->getId() == id) {
                std::cout << "[ERROR] Sensor com ID '" << id << "' já existe.\n";
                return -1;
            }
            continue;
        }

        // Slot vazio mas já usado antes (tombstone): guarda e continua sondando
        if (deleted[slot]) {
            if (firstDeletedSlot == -1) {
                firstDeletedSlot = slot;
                firstDeletedCollision = i;
            }
            continue;
        }

        // Slot realmente nunca usado -> pode encerrar a busca
        int targetSlot = (firstDeletedSlot != -1) ? firstDeletedSlot : slot;
        int collisions = (firstDeletedSlot != -1) ? firstDeletedCollision : i;

        sensors[targetSlot] = new Sensor(id, type, location);
        deleted[targetSlot] = false;
        count++;
        std::cout << "[OK] Sensor '" << id << "' registrado na posição " << targetSlot << ".\n";
        return collisions;
    }

    // Caso extremo: não achou slot "nunca usado", mas há tombstone reaproveitável
    if (firstDeletedSlot != -1) {
        sensors[firstDeletedSlot] = new Sensor(id, type, location);
        deleted[firstDeletedSlot] = false;
        count++;
        std::cout << "[OK] Sensor '" << id << "' registrado na posição " << firstDeletedSlot << ".\n";
        return firstDeletedCollision;
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
    for (int i = 0; i < maxSensors; i++)
        if (sensors[i] != nullptr)
        {
            std::cout << "----------------------------------------\n";
            std::cout << "[posição " << i << "]\n";
            sensors[i]->displayInfo();
        }
    std::cout << "----------------------------------------\n";
}

Sensor** SensorDataBase::getSensors() const {
    return sensors;
}

int SensorDataBase::getMaxSensors() const {
    return maxSensors;
}

int SensorDataBase::getCount() const {
    return count;
}
