#pragma once

#include <string>
#include <vector>

class Sensor {
    int id;
    std::string tipo;
    std::string localizacao;
    double ultimaLeitura;
    std::vector<double> historico;
    time_t ultimaAtualizacao;

    std::string formatarData(time_t timestamp) const;

public:
    Sensor(int id, const std::string& tipo, const std::string& localizacao);

    void atualizarLeitura(double novoValor);
    void exibirInfo() const;

    int getId()            const;
    std::string getTipo()          const;
    std::string getLocalizacao()   const;
    double      getUltimaLeitura() const;
    time_t      getUltimaAtualizacao() const;
    const std::vector<double>& getHistorico() const;
};