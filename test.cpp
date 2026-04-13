#include "SensorDataBase.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <sstream>

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

    std::ostringstream sink;
    std::streambuf* oldCout = std::cout.rdbuf(sink.rdbuf());


    const int NUM_ITEMS = 20000;
    const int NUM_SEARCHES = 1000;
    const int MAX_SENSORS = 10007; // Tamanho da tabela hash menor para forçar coisões
    const double PERCENTAGE = 1; // Porcentagem de sensores a inserir (70%)
    const int ITEMS_TO_INSERT = (int)(MAX_SENSORS * PERCENTAGE); // Quantidade de sensores a inserir

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

    // Inserção convencional e na hash - apenas ITEMS_TO_INSERT (70% de MAX_SENSORS)
    for (int i = 0; i < ITEMS_TO_INSERT; i++) {
        int id = ids[i];
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
        (void)sHash;
        auto endHash = high_resolution_clock::now();
        totalSearchTime += duration<double, milli>(endHash - startHash).count();

        // Busca convencional
        auto startConv = high_resolution_clock::now();
        Sensor* sConv = findConventional(conventionalSensors, id);
        (void)sConv;
        auto endConv = high_resolution_clock::now();
        totalConventionalSearchTime += duration<double, milli>(endConv - startConv).count();
    }

    // Limpeza da memória
    for (auto* s : conventionalSensors) {
        delete s;
    }

    std::cout.rdbuf(oldCout);

    // Resultados
    cout << "=== RESULTADOS DOS TESTES ===\n";
    cout << "Número de itens inseridos: " << ITEMS_TO_INSERT << " (" << (PERCENTAGE * 100) << "% de " << MAX_SENSORS << ")\n";
    cout << "Número de buscas realizadas: " << NUM_SEARCHES << "\n";
    cout << "Tamanho da tabela hash: " << MAX_SENSORS << "\n\n";
    cout << "PERCENTAGE = " << PERCENTAGE << "%\n";

    cout << "INSERÇÃO:\n";
    cout << "  Tempo total (Hash): " << totalInsertionTime << " ms\n";
    cout << "  Tempo médio (Hash): " << (totalInsertionTime / ITEMS_TO_INSERT) << " ms por item\n";
    cout << "  Tempo total (Convencional): " << totalConventionalInsertionTime << " ms\n";
    cout << "  Tempo médio (Convencional): " << (totalConventionalInsertionTime / ITEMS_TO_INSERT) << " ms por item\n";
    cout << "  Relação hash / convencional: " << (totalInsertionTime / totalConventionalInsertionTime) << "\n";
    cout << "  Total de colisões (Hash): " << totalCollisions << "\n";
    cout << "  Média de colisões (Hash): " << (static_cast<double>(totalCollisions) / ITEMS_TO_INSERT) << " por item\n\n";

    cout << "BUSCA:\n";
    cout << "  Tempo total (Hash): " << totalSearchTime << " ms\n";
    cout << "  Tempo médio (Hash): " << (totalSearchTime / NUM_SEARCHES) << " ms por busca\n";
    cout << "  Tempo total (Convencional): " << totalConventionalSearchTime << " ms\n";
    cout << "  Tempo médio (Convencional): " << (totalConventionalSearchTime / NUM_SEARCHES) << " ms por busca\n";
    cout << "  Relação hash / convencional: " << (totalSearchTime / totalConventionalSearchTime) << "\n";
    cout << "=========================================" << std::endl;

    return 0;
}

