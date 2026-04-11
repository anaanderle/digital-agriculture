#pragma once

#include "SensorDataBase.h"
#include <string>

class ImportExport {
public:
    static void importSensors(SensorDataBase& db, const std::string& inputFile, const std::string& outputFile);

    static void exportSensors(const SensorDataBase& db, const std::string& outputFile);

private:
    static std::string formatDate(time_t timestamp);
    static std::string formatHistory(const std::vector<double>& history);
};

