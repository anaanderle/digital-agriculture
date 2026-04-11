#include "SensorDataBase.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace chrono;

// Função para busca convencional (linear no vector)
Sensor* findConventional(const vector<Sensor*>& conventional, int id) {
    for (auto* s : conventional) {
        if (s->getId() == id) {
            return s;
        }
    }
    return nullptr;
}

int main() {
    cout << "Iniciando teste...\n";

    const int NUM_ITEMS = 20000;
    const int NUM_SEARCHES = 1000;
    const int MAX_SENSORS = 10000; // Tamanho da tabela hash menor para forçar colisões

    // Gerar 10.000 IDs únicos aleatórios
    vector<int> ids(NUM_ITEMS);
    iota(ids.begin(), ids.end(), 0); // Preenche com 0 a 9999
    shuffle(ids.begin(), ids.end(), mt19937{random_device{}()}); // Embaralha

    // Estrutura convencional: vector de Sensors
    vector<Sensor*> conventionalSensors;

    // SensorDataBase
    SensorDataBase db(MAX_SENSORS);

    // Variáveis para tempos e colisões
    double totalInsertionTime = 0.0;
    double totalConventionalInsertionTime = 0.0;
    double totalSearchTime = 0.0;
    double totalConventionalSearchTime = 0.0;
    long long totalCollisions = 0;

    // Inserção convencional e na hash
    for (int id : ids) {
        // Inserção convencional
        auto startConv = high_resolution_clock::now();
        conventionalSensors.push_back(new Sensor(id, "TipoFixo", "LocalFixo"));
        auto endConv = high_resolution_clock::now();
        totalConventionalInsertionTime += duration<double, milli>(endConv - startConv).count();

        // Inserção na hash
        auto startHash = high_resolution_clock::now();
        int collisions = db.registerSensor(id, "TipoFixo", "LocalFixo");
        auto endHash = high_resolution_clock::now();
        totalInsertionTime += duration<double, milli>(endHash - startHash).count();

        if (collisions != -1) {
            totalCollisions += collisions; // Número de colisões = tentativas - 1
        }
    }

    // Preparar 1000 IDs aleatórios para busca (embaralhar e pegar os primeiros 1000)
    vector<int> searchIds = ids;
    shuffle(searchIds.begin(), searchIds.end(), mt19937{random_device{}()});
    searchIds.resize(NUM_SEARCHES);

    // Buscas
    for (int id : searchIds) {
        // Busca na hash
        auto startHash = high_resolution_clock::now();
        Sensor* sHash = db.findById(id);
        auto endHash = high_resolution_clock::now();
        totalSearchTime += duration<double, milli>(endHash - startHash).count();

        // Busca convencional
        auto startConv = high_resolution_clock::now();
        Sensor* sConv = findConventional(conventionalSensors, id);
        auto endConv = high_resolution_clock::now();
        totalConventionalSearchTime += duration<double, milli>(endConv - startConv).count();
    }

    // Limpeza da memória
    for (auto* s : conventionalSensors) {
        delete s;
    }

    // Resultados
    cout << "=== RESULTADOS DOS TESTES ===\n";
    cout << "Número de itens inseridos: " << NUM_ITEMS << "\n";
    cout << "Número de buscas realizadas: " << NUM_SEARCHES << "\n";
    cout << "Tamanho da tabela hash: " << MAX_SENSORS << "\n\n";

    cout << "INSERÇÃO:\n";
    cout << "  Tempo total (Hash): " << totalInsertionTime << " ms\n";
    cout << "  Tempo médio (Hash): " << (totalInsertionTime / NUM_ITEMS) << " ms por item\n";
    cout << "  Tempo total (Convencional): " << totalConventionalInsertionTime << " ms\n";
    cout << "  Tempo médio (Convencional): " << (totalConventionalInsertionTime / NUM_ITEMS) << " ms por item\n";
    cout << "  Total de colisões (Hash): " << totalCollisions << "\n";
    cout << "  Média de colisões (Hash): " << (static_cast<double>(totalCollisions) / NUM_ITEMS) << " por item\n\n";

    cout << "BUSCA:\n";
    cout << "  Tempo total (Hash): " << totalSearchTime << " ms\n";
    cout << "  Tempo médio (Hash): " << (totalSearchTime / NUM_SEARCHES) << " ms por busca\n";
    cout << "  Tempo total (Convencional): " << totalConventionalSearchTime << " ms\n";
    cout << "  Tempo médio (Convencional): " << (totalConventionalSearchTime / NUM_SEARCHES) << " ms por busca\n";

    return 0;
}
