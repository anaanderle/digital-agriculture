// main.cpp
#include <iostream>
#include <string>
#include "SensorDataBase.h"

void printMenu() {
    std::cout << "\n========================================\n";
    std::cout << "         SENSOR DATABASE SYSTEM\n";
    std::cout << "========================================\n";
    std::cout << "  1. Register sensor\n";
    std::cout << "  2. Update reading\n";
    std::cout << "  3. Search sensor by ID\n";
    std::cout << "  4. Remove sensor\n";
    std::cout << "  5. Display all sensors\n";
    std::cout << "  0. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Option: ";
}

void registerSensor(SensorDataBase& db) {
    int id;
    std::string type, location;
    std::cout << "\n--- Register Sensor ---\n";
    std::cout << "ID       : "; std::cin >> id;
    std::cout << "Type     : "; std::cin >> type;
    std::cout << "Location : "; std::cin >> location;
    db.registerSensor(id, type, location);
}

void updateReading(SensorDataBase& db) {
    int id;
    double value;
    std::cout << "\n--- Update Reading ---\n";
    std::cout << "Sensor ID : "; std::cin >> id;
    std::cout << "New value : "; std::cin >> value;
    db.updateReading(id, value);
}

void searchSensor(SensorDataBase& db) {
    int id;
    std::cout << "\n--- Search Sensor ---\n";
    std::cout << "Sensor ID : "; std::cin >> id;

    Sensor* s = db.findById(id);
    if (s != nullptr)
        s->exibirInfo();
}

void removeSensor(SensorDataBase& db) {
    int id;
    std::cout << "\n--- Remove Sensor ---\n";
    std::cout << "Sensor ID : "; std::cin >> id;
    db.removeById(id);
}

void displayAll(SensorDataBase& db) {
    std::cout << "\n--- All Sensors ---\n";
    db.listAllIds();
}

int main() {
    int maxItems;

    std::cout << "========================================\n";
    std::cout << "       SENSOR DATABASE SYSTEM\n";
    std::cout << "========================================\n";
    std::cout << "Max number of sensors: ";
    std::cin >> maxItems;

    while (maxItems <= 0) {
        std::cout << "[ERROR] Value must be greater than 0. Try again: ";
        std::cin >> maxItems;
    }

    SensorDataBase db(maxItems);
    std::cout << "[OK] Database created with capacity for " << maxItems << " sensors.\n";

    int option;

    do {
        printMenu();
        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "[ERROR] Invalid input. Please enter a number.\n";
            continue;
        }

        switch (option) {
            case 1: registerSensor(db); break;
            case 2: updateReading(db);  break;
            case 3: searchSensor(db);   break;
            case 4: removeSensor(db);   break;
            case 5: displayAll(db);     break;
            case 0:
                std::cout << "\n[INFO] Exiting system. Goodbye!\n";
                break;
            default:
                std::cout << "[ERROR] Invalid option. Please choose between 0 and 5.\n";
        }

    } while (option != 0);

    return 0;
}