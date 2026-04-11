// Sensor.cpp
#include "Sensor.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Sensor::Sensor(const std::string& id, const std::string& tipo, const std::string& localizacao)
    : id(id), tipo(tipo), localizacao(localizacao),
      ultimaLeitura(0.0), ultimaAtualizacao(0) {}

void Sensor::atualizarLeitura(double novoValor) {
    ultimaLeitura = novoValor;
    ultimaAtualizacao = std::time(nullptr);
    historico.push_back(novoValor);
}

void Sensor::exibirInfo() const {
    std::cout << "=== Sensor ===\n";
    std::cout << "ID          : " << id << "\n";
    std::cout << "Tipo        : " << tipo << "\n";
    std::cout << "Localização : " << localizacao << "\n";
    std::cout << "Última leitura: " << ultimaLeitura << "\n";

    if (ultimaAtualizacao != 0)
        std::cout << "Atualizado em : " << formatarData(ultimaAtualizacao) << "\n";
    else
        std::cout << "Atualizado em : (nunca)\n";

    std::cout << "Histórico   : [";
    for (size_t i = 0; i < historico.size(); ++i) {
        std::cout << historico[i];
        if (i + 1 < historico.size()) std::cout << ", ";
    }
    std::cout << "]\n";
}

std::string Sensor::formatarData(time_t timestamp) const {
    std::tm* tm_info = std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(tm_info, "%d/%m/%Y %H:%M:%S");
    return oss.str();
}

std::string Sensor::getId()           const { return id; }
std::string Sensor::getTipo()         const { return tipo; }
std::string Sensor::getLocalizacao()  const { return localizacao; }
double      Sensor::getUltimaLeitura() const { return ultimaLeitura; }
time_t      Sensor::getUltimaAtualizacao() const { return ultimaAtualizacao; }
const std::vector<double>& Sensor::getHistorico() const { return historico; }