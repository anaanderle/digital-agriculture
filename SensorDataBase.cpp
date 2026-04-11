// SensorDataBase.cpp
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

// --- Hash functions ---

int SensorDataBase::h1(unsigned int k) const {
    return k % maxSensors;
}

// M' = maxSensors - 1, garante h2 != 0
int SensorDataBase::h2(unsigned int k) const {
    return 1 + (k % (maxSensors - 1));
}

// h(k, i) = (h1(k) + i * h2(k)) % M
int SensorDataBase::doubleHash(unsigned int k, int i) const {
    std::cout << "HASHHHHHHH" << (h1(k) + i * h2(k)) % maxSensors << "'.\n";
    return (h1(k) + i * h2(k)) % maxSensors;
}

// --- Private helper ---

// Percorre a cadeia de sondagem:
// - para em slot vazio E nao deletado (fim real da cadeia)
// - pula tombstones (deleted == true, sensors == nullptr)
// - retorna o slot se encontrar o ID
int SensorDataBase::findIndexById(int id) const {
    for (int i = 0; i < maxSensors; i++) {
        int slot = doubleHash(id, i);

        if (sensors[slot] == nullptr && !deleted[slot])
            return -1;

        if (sensors[slot] != nullptr && sensors[slot]->getId() == id)
            return slot;

        // deleted[slot] == true: tombstone, continua sondando
    }
    return -1;
}

// --- Public interface ---

bool SensorDataBase::registerSensor(int id, const std::string& type, const std::string& location) {
    if (count >= maxSensors) {
        std::cout << "[ERROR] Database is full. Cannot register sensor '" << id << "'.\n";
        return false;
    }

    int firstTombstone = -1;

    for (int i = 0; i < maxSensors; i++) {
        int slot = doubleHash(id, i);

        // Verifica se o ID já existe
        if (sensors[slot] != nullptr && sensors[slot]->getId() == id) {
            std::cout << "[ERROR] Sensor with ID '" << id << "' already exists.\n";
            return false;
        }

        // Slot vazio e nunca deletado: fim da cadeia
        if (sensors[slot] == nullptr && !deleted[slot]) {
            int target = (firstTombstone != -1) ? firstTombstone : slot;
            sensors[target] = new Sensor(id, type, location);
            deleted[target] = false;
            count++;
            std::cout << "[OK] Sensor '" << id << "' registered at slot " << target << ".\n";
            return true;
        }

        // Primeiro tombstone encontrado: candidato para reuso
        if (deleted[slot] && firstTombstone == -1)
            firstTombstone = slot;
    }

    // Tabela cheia de tombstones mas count < maxSensors
    if (firstTombstone != -1) {
        sensors[firstTombstone] = new Sensor(id, type, location);
        deleted[firstTombstone] = false;
        count++;
        std::cout << "[OK] Sensor '" << id << "' registered at slot " << firstTombstone << " (reused tombstone).\n";
        return true;
    }

    return false;
}

Sensor* SensorDataBase::findById(int id) const {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' not found.\n";
        return nullptr;
    }
    return sensors[index];
}

bool SensorDataBase::removeById(int id) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "[ERROR] Sensor '" << id << "' not found. Nothing removed.\n";
        return false;
    }

    delete sensors[index];
    sensors[index] = nullptr;
    deleted[index] = true;   // tombstone: preserva a cadeia de sondagem
    count--;
    std::cout << "[OK] Sensor '" << id << "' removed from slot " << index << ".\n";
    return true;
}

bool SensorDataBase::updateReading(int id, double value) {
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
            std::cout << "  [slot " << i << "] " << sensors[i]->getId() << "\n";
    }
}