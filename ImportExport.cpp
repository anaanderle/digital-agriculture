#include "ImportExport.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>

void ImportExport::importSensors(SensorDataBase& db, const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile);
    std::ofstream outFile(outputFile);

    if (!inFile.is_open()) {
        std::cout << "[ERROR] Não foi possível abrir o arquivo de entrada: " << inputFile << "\n";
        return;
    }

    if (!outFile.is_open()) {
        std::cout << "[ERROR] Não foi possível criar o arquivo de saída: " << outputFile << "\n";
        inFile.close();
        return;
    }

    std::string line;
    int successCount = 0;
    int failureCount = 0;

    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        // Parser: id - type - location
        std::istringstream iss(line);
        int id;
        std::string dash1, type, dash2, location;

        if (iss >> id >> dash1 >> type >> dash2) {
            // Pega o resto da linha como location
            std::getline(iss, location);

            // Remove espaços em branco do início
            if (!location.empty() && location[0] == ' ') {
                location = location.substr(1);
            }

            // Tenta registrar o sensor
            int result = db.registerSensor(id, type, location);

            if (result >= 0) {
                outFile << id << " - sucesso\n";
                successCount++;
            } else {
                outFile << id << " - falha\n";
                failureCount++;
            }
        } else {
            std::cout << "[ERROR] Formato inválido na linha: " << line << "\n";
        }
    }

    inFile.close();
    outFile.close();

    std::cout << "[OK] Importação concluída. Arquivo de saída: " << outputFile << "\n";
    std::cout << "[INFO] Sucessos: " << successCount << ", Falhas: " << failureCount << "\n";
}

void ImportExport::exportSensors(const SensorDataBase& db, const std::string& outputFile) {
    std::ofstream outFile(outputFile);

    if (!outFile.is_open()) {
        std::cout << "[ERROR] Não foi possível criar o arquivo de saída: " << outputFile << "\n";
        return;
    }

    Sensor** sensors = db.getSensors();
    int maxSensors = db.getMaxSensors();
    int count = 0;

    for (int i = 0; i < maxSensors; i++) {
        if (sensors[i] != nullptr) {
            Sensor* s = sensors[i];

            outFile << s->getId() << ", "
                    << s->getType() << ", "
                    << s->getLocation() << ", "
                    << s->getLastReading() << ", "
                    << formatDate(s->getLastUpdate()) << ", "
                    << formatHistory(s->getHistory()) << "\n";
            count++;
        }
    }

    outFile.close();

    std::cout << "[OK] Exportação concluída. " << count << " sensores exportados para: " << outputFile << "\n";
}

std::string ImportExport::formatDate(time_t timestamp) {
    if (timestamp == 0) return "(nunca)";

    std::tm* tm_info = std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(tm_info, "%d/%m/%Y %H:%M:%S");
    return oss.str();
}

std::string ImportExport::formatHistory(const std::vector<double>& history) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < history.size(); ++i) {
        oss << history[i];
        if (i + 1 < history.size()) oss << ", ";
    }
    oss << "]";
    return oss.str();
}



