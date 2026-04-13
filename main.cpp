#include <iostream>
#include <string>
#include "SensorDataBase.h"
#include "ImportExport.h"

void printMenu() {
    std::cout << "Agricultura digital\n";
    std::cout << "1. Registrar sensor\n";
    std::cout << "2. Atualizar leitura\n";
    std::cout << "3. Procurar sensor\n";
    std::cout << "4. Remover sensor\n";
    std::cout << "5. Exibir todos os sensores\n";
    std::cout << "6. Importar sensores\n";
    std::cout << "7. Exportar sensores\n";
    std::cout << "8. Simular leituras automáticas\n";
    std::cout << "0. Sair\n";
    std::cout << "Opção: ";
}

void registerSensor(SensorDataBase& db) {
    int id;
    std::string type, location;
    std::cout << "\nRegistrar sensor\n";
    std::cout << "ID          : "; std::cin >> id;
    std::cout << "Tipo        : "; std::cin >> type;
    std::cout << "Localização : "; std::getline(std::cin >> std::ws, location); // le a linha inteira isso
    db.registerSensor(id, type, location);
}

void updateReading(SensorDataBase& db) {
    int id;
    double value;
    std::cout << "\nAtualizar leitura\n";
    std::cout << "ID do sensor : "; std::cin >> id;
    std::cout << "Novo valor   : "; std::cin >> value;
    db.updateReading(id, value);
}

void searchSensor(SensorDataBase& db) {
    int id;
    std::cout << "\nProcurar sendor\n";
    std::cout << "ID do sensor : "; std::cin >> id;

    Sensor* s = db.findById(id);
    if (s != nullptr)
        s->displayInfo();
}

void removeSensor(SensorDataBase& db) {
    int id;
    std::cout << "\nRemover sensor\n";
    std::cout << "ID do sensor : "; std::cin >> id;
    db.removeById(id);
}

void displayAll(SensorDataBase& db) {
    std::cout << "\nTodos os sensores\n";
    db.listAllIds();
}

void importSensors(SensorDataBase& db) {
    std::string inputFile, outputFile;
    std::cout << "\nImportar sensores\n";
    std::cout << "Arquivo de entrada : "; std::cin >> inputFile;
    std::cout << "Arquivo de saída   : "; std::cin >> outputFile;
    ImportExport::importSensors(db, inputFile, outputFile);
}

void exportSensors(const SensorDataBase& db) {
    std::string outputFile;
    std::cout << "\nExportar sensores\n";
    std::cout << "Arquivo de saída   : "; std::cin >> outputFile;
    ImportExport::exportSensors(db, outputFile);
}

void simulateAutomaticReadings(SensorDataBase& db) {
    int cycles;
    double minValue, maxValue;

    std::cout << "\nSimular leituras automáticas\n";
    std::cout << "Número de ciclos : "; std::cin >> cycles;
    std::cout << "Valor mínimo     : "; std::cin >> minValue;
    std::cout << "Valor máximo     : "; std::cin >> maxValue;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "[ERROR] Entrada inválida.\n";
        return;
    }

    db.simulateReadings(cycles, minValue, maxValue);
}

int main() {
    int maxSensors;
    int optimizeMaxSensorOption;

    std::cout << "Agricultura digital\n";
    std::cout << "Número máximo de sensores: ";
    std::cin >> maxSensors;

    while (maxSensors <= 1) {
        std::cout << "[ERROR] Número máximo de sensores deve ser maior que 1.\n";
        std::cin >> maxSensors;
    }

    std::cout << "Gostaria de otimizar sua hash usando o próximo número primo:\n";
    std::cout << "1. Sim (padrão)\n";
    std::cout << "2. Não\n";
    std::cin >> optimizeMaxSensorOption;

    bool optimizeMaxSensor = (optimizeMaxSensorOption != 2);

    SensorDataBase db(maxSensors, optimizeMaxSensor);
    std::cout << "[OK] Sistema criado com capacidade para " << db.getMaxSensors() << " sensores.\n";

    int option;

    do {
        printMenu();
        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "[ERROR] Opção inválida.\n";
            continue;
        }

        switch (option) {
            case 1: registerSensor(db); break;
            case 2: updateReading(db);  break;
            case 3: searchSensor(db);   break;
            case 4: removeSensor(db);   break;
            case 5: displayAll(db);     break;
            case 6: importSensors(db);  break;
            case 7: exportSensors(db);  break;
            case 8: simulateAutomaticReadings(db); break;
            case 0:
                std::cout << "\n[INFO] Saindo do sistema!\n";
                break;
            default:
                std::cout << "[ERROR] Opção inválida.\n";
        }

    } while (option != 0);

    return 0;
}