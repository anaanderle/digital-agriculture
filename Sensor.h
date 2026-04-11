#pragma once

#include <string>
#include <vector>

class Sensor {
    int id;
    std::string type;
    std::string location;
    double lastReading;
    std::vector<double> history;
    time_t lastUpdate;

    std::string formatDate(time_t timestamp) const;

public:
    Sensor(int id, const std::string& type, const std::string& location);

    void updateReading(double newValue);
    void displayInfo() const;

    int getId()            const;
    std::string getType()          const;
    std::string getLocation()   const;
    double      getLastReading() const;
    time_t      getLastUpdate() const;
    const std::vector<double>& getHistory() const;
};