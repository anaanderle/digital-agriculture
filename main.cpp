#include <iostream>
#include <string>
#include "SensorDataBase.h"

void printMenu() {
    std::cout << "Agricultura digital\n";
    std::cout << "1. Registrar sensor\n";
    std::cout << "2. Atualizar leitura\n";
    std::cout << "3. Procurar sensor\n";
    std::cout << "4. Remover sensor\n";
    std::cout << "5. Exibir todos os sensores\n";
    std::cout << "0. Sair\n";
    std::cout << "Opção: ";
}

void registerSensor(SensorDataBase& db) {
    int id;
    std::string type, location;
    std::cout << "\nRegistrar sensor\n";
    std::cout << "ID          : "; std::cin >> id;
    std::cout << "Tipo        : "; std::cin >> type;
    std::cout << "Localização : "; std::cin >> location;
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
        s->exibirInfo();
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

int main() {
    int maxItems;

    std::cout << "Agricultura digital\n";
    std::cout << "Número máximo de sensores: ";
    std::cin >> maxItems;

    while (maxItems <= 0) {
        std::cout << "[ERROR] Número máximo de sensores deve ser maior que 0.";
        std::cin >> maxItems;
    }

    SensorDataBase db(maxItems);
    std::cout << "[OK] Sistema criado com capacidade para " << maxItems << " sensores.\n";

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
            case 0:
                std::cout << "\n[INFO] Saindo do sistema!\n";
                break;
            default:
                std::cout << "[ERROR] Opção inválida.\n";
        }

    } while (option != 0);

    return 0;
}