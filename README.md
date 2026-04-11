# Agricultura Digital - Sistema de Gerenciamento de Sensores

## 📋 Descrição do Projeto

Este projeto implementa um **sistema de gerenciamento de sensores para agricultura digital** utilizando uma **tabela hash com endereçamento duplo** como estrutura de dados principal. O sistema permite registrar, buscar, atualizar e remover sensores agrícolas, mantendo um histórico de leituras de cada sensor.

O projeto demonstra a aplicação prática de estruturas de dados eficientes em um contexto de IoT (Internet das Coisas) para monitoramento de culturas agrícolas.

---

## 🎯 Objetivo

Comparar o desempenho de uma tabela hash com endereçamento duplo versus busca linear em um vetor, para operações de inserção e busca de sensores. O projeto inclui testes automatizados com 20.000 inserções e 1.000 buscas.

---

## 📁 Estrutura do Projeto

```
digital-agriculture/
├── main.cpp                 # Interface interativa com menu de opções
├── Sensor.h / Sensor.cpp    # Classe que representa um sensor
├── SensorDataBase.h / SensorDataBase.cpp  # Tabela hash com endereçamento duplo
└── test.cpp                 # Testes de performance
```

---

## 📚 Classes

### **Sensor**
Representa um sensor agrícola com seus atributos e funcionalidades.

**Atributos:**
- `id`: Identificador único do sensor
- `type`: Tipo de sensor (ex: temperatura, umidade)
- `location`: Localização física do sensor
- `lastReading`: Último valor medido
- `history`: Vetor com histórico de todas as leituras
- `lastUpdate`: Timestamp da última atualização

**Métodos principais:**
- `updateReading(double value)`: Atualiza a leitura do sensor
- `displayInfo()`: Exibe todas as informações do sensor
- `getId(), getType(), getLocation(), etc`: Getters

---

### **SensorDataBase**
Implementa uma **tabela hash com endereçamento duplo** para armazenar sensores.

**Características:**
- Usa **hash duplo** para resolver colisões: `h(k, i) = (h1(k) + i*h2(k)) mod m`
- Mantém array de `Sensor*` para armazenar sensores
- Mantém array `deleted[]` para marcar posições deletadas
- Suporta até `maxSensors` elementos

**Métodos principais:**
- `registerSensor(id, type, location)`: Insere um novo sensor (retorna número de colisões até encontrar slot)
- `findById(id)`: Busca um sensor pelo ID
- `removeById(id)`: Remove um sensor (marca como deletado)
- `updateReading(id, value)`: Atualiza leitura de um sensor
- `listAllIds()`: Lista todos os sensores registrados

**Funções de hash:**
- `h1(k)`: Hash primário = `k % maxSensors`
- `h2(k)`: Hash secundário = `1 + (k % (maxSensors - 1))`

---

## 🚀 Como Compilar e Rodar

### **1. Compilar o programa principal:**
```bash
g++ -std=c++17 -o main main.cpp Sensor.cpp SensorDataBase.cpp -I.
```

### **2. Executar o programa principal:**
```bash
./main
```

Isso abre um menu interativo onde você pode:
- Registrar sensores
- Atualizar leituras
- Procurar sensores
- Remover sensores
- Exibir todos os sensores
- Sair do programa

**Exemplo de uso:**
```
Agricultura digital
Número máximo de sensores: 100
[OK] Sistema criado com capacidade para 100 sensores.

Agricultura digital
1. Registrar sensor
2. Atualizar leitura
3. Procurar sensor
4. Remover sensor
5. Exibir todos os sensores
0. Sair
Opção: 1

Registrar sensor
ID          : 1
Tipo        : Temperatura
Localização : Talhão A
[OK] Sensor '1' registrado na posição 1.
```

---

### **3. Compilar e rodar os testes:**
```bash
g++ -std=c++17 -o test test.cpp Sensor.cpp SensorDataBase.cpp -I.
./test
```

Os testes realizam:
- **20.000 inserções** de sensores
- **1.000 buscas** aleatórias
- Comparam performance entre tabela hash e busca linear em vetor
- Exibem tempo total, tempo médio, e número de colisões

**Exemplo de saída:**
```
Iniciando teste...

=== RESULTADOS DOS TESTES ===
Número de itens inseridos: 20000
Número de buscas realizadas: 1000
Tamanho da tabela hash: 10000

INSERÇÃO:
  Tempo total (Hash): 45.3 ms
  Tempo médio (Hash): 0.00226 ms por item
  Tempo total (Convencional): 2340.5 ms
  Tempo médio (Convencional): 0.117 ms por item
  Total de colisões (Hash): 8932
  Média de colisões (Hash): 0.4466 por item

BUSCA:
  Tempo total (Hash): 2.1 ms
  Tempo médio (Hash): 0.0021 ms por busca
  Tempo total (Convencional): 3456.2 ms
  Tempo médio (Convencional): 3.456 ms por busca
```

---

## 💻 Requisitos

- **Compilador C++**: GCC ou Clang com suporte a C++17
- **Sistema Operacional**: Linux, macOS ou Windows (com WSL/MinGW)