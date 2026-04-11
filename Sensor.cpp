#include "Sensor.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Sensor::Sensor(int id, const std::string& type, const std::string& location)
    : id(id), type(type), location(location),
      lastReading(0.0), lastUpdate(0) {}

void Sensor::updateReading(double newValue) {
    lastReading = newValue;
    lastUpdate = std::time(nullptr);
    history.push_back(newValue);
}

void Sensor::displayInfo() const {
    std::cout << "Sensor\n";
    std::cout << "ID             : " << id << "\n";
    std::cout << "Tipo           : " << type << "\n";
    std::cout << "Localização    : " << location << "\n";
    std::cout << "Última leitura : " << lastReading << "\n";

    if (lastUpdate != 0)
        std::cout << "Atualizado em : " << formatDate(lastUpdate) << "\n";
    else
        std::cout << "Atualizado em : (nunca)\n";

    std::cout << "Histórico   : [";
    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << history[i];
        if (i + 1 < history.size()) std::cout << ", ";
    }
    std::cout << "]\n";
}

std::string Sensor::formatDate(time_t timestamp) const {
    std::tm* tm_info = std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(tm_info, "%d/%m/%Y %H:%M:%S");
    return oss.str();
}

int         Sensor::getId()           const { return id; }
std::string Sensor::getType()         const { return type; }
std::string Sensor::getLocation()  const { return location; }
double      Sensor::getLastReading() const { return lastReading; }
time_t      Sensor::getLastUpdate() const { return lastUpdate; }
const std::vector<double>& Sensor::getHistory() const { return history; }
