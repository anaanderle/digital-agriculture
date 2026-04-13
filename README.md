# Agricultura Digital - Sistema de Gerenciamento de Sensores

## Autores

Ana Clara Anderle

Carolina Silva dos Santos

Guilherme Lenzi

Taimisson de Carvalho Schardosim

## Descrição do Projeto

Este projeto implementa um **sistema de gerenciamento de sensores para agricultura digital** utilizando uma **tabela hash com endereçamento duplo** como estrutura de dados principal. O sistema permite registrar, buscar, atualizar e remover sensores agrícolas, mantendo um histórico de leituras de cada sensor.

O projeto demonstra a aplicação prática de estruturas de dados eficientes em um contexto de IoT (Internet das Coisas) para monitoramento de culturas agrícolas.

---

## Objetivo

Comparar o desempenho de uma tabela hash com endereçamento duplo versus busca linear em um vetor, para operações de inserção e busca de sensores. O projeto inclui testes automatizados com 20.000 inserções e 1.000 buscas.

---

## Estrutura do Projeto

```
digital-agriculture/
├── main.cpp                 # Interface interativa com menu de opções
├── Sensor.h / Sensor.cpp    # Classe que representa um sensor
├── SensorDataBase.h / SensorDataBase.cpp  # Tabela hash com endereçamento duplo
├── ImportExport.h / ImportExport.cpp      # Classe para importação/exportação de sensores
└── test.cpp                 # Testes de performance
```

---

## Classes

### **Sensor**
Representa um sensor agrícola com seus atributos e funcionalidades.

**Atributos:**
- `id`: Identificador único do sensor
- `type`: Tipo de sensor (ex.: temperatura, umidade)
- `location`: Localização física do sensor
- `lastReading`: Último valor medido
- `history`: Vetor com histórico de todas as leituras
- `lastUpdate`: Timestamp da última atualização

**Métodos principais:**
- `updateReading(double value)`: Atualiza a leitura do sensor
- `displayInfo()`: Exibe todas as informações do sensor
- `getId(), getType(), getLocation(), etc.`: Getters

---

### **SensorDataBase**
Implementa uma **tabela hash com endereçamento duplo** para armazenar sensores.

**Características:**
- Usa **hash duplo** para resolver colisões: `h(k, i) = (h1(k) + i*h2(k)) mod m`
- Mantém array de `Sensor*` para armazenar sensores
- Mantém array `deleted[]` para marcar posições deletadas
- Ajusta o tamanho da tabela para número primo

**Métodos principais:**
- `registerSensor(id, type, location)`: Insere um novo sensor (retorna número de colisões até encontrar slot)
- `findById(id)`: Busca um sensor pelo ID
- `removeById(id)`: Remove um sensor (marca como deletado)
- `updateReading(id, value)`: Atualiza leitura de um sensor
- `listAllIds()`: Lista todos os sensores registrados
- `simulateReadings(cycles, min, max)`: Gera leituras automáticas
- `getSensors()`: Retorna o array de sensores para iteração (usado pela exportação)
- `getMaxSensors()`: Retorna a capacidade máxima da tabela hash
- `getCount()`: Retorna a quantidade de sensores registrados

**Funções de hash:**
- `h1(k)`: Hash primário = `k % maxSensors`
- `h2(k)`: Hash secundário = `1 + (k % (maxSensors - 1))`

---

## Importação e Exportação de Sensores

### **Classe ImportExport**
Gerencia a importação e exportação de sensores para arquivos de texto.

**Métodos:**
- `importSensors(SensorDataBase& db, inputFile, outputFile)`: Importa sensores de um arquivo TXT
- `exportSensors(const SensorDataBase& db, outputFile)`: Exporta estado completo do banco de dados

### **Importação de Sensores (Opção 6)**

**Formato do arquivo de entrada:**
Cada linha deve conter: `id - type - location`

```txt
1 - temperatura - cozinha
2 - umidade - quarto
3 - luz - sala
4 - pressao - garagem
```

**Arquivo de resultado:**
Cada linha contém o status da importação: `id - sucesso` ou `id - falha`

```txt
1 - sucesso
2 - sucesso
3 - sucesso
4 - falha
```

### **Exportação de Sensores (Opção 7)**

**Formato do arquivo de saída:**
Cada linha contém todas as informações de um sensor: `id, type, location, lastReading, lastUpdate, [history]`

```txt
1, temperatura, cozinha, 23.5, 11/04/2026 20:07:19, [23.5, 24.1, 22.8]
2, umidade, quarto, 65.3, 11/04/2026 20:07:19, [65.3]
3, luz, sala, 500, (nunca), []
```

---

## Como Compilar e Rodar

### **1. Compilar o programa principal**
```bash
g++ -std=c++17 -o main main.cpp Sensor.cpp SensorDataBase.cpp ImportExport.cpp -I.
```

### **2. Executar o programa principal**
```bash
./main
```

Isso abre um menu interativo onde você pode:
- Registrar sensores
- Atualizar leituras
- Procurar sensores
- Remover sensores
- Exibir todos os sensores
- Importar sensores de um arquivo
- Exportar sensores para um arquivo
- Simular leituras automáticas
- Sair do programa

**Exemplo de uso:**
```txt
Agricultura digital
Número máximo de sensores: 100
[OK] Sistema criado com capacidade para 101 sensores.

Agricultura digital
1. Registrar sensor
2. Atualizar leitura
3. Procurar sensor
4. Remover sensor
5. Exibir todos os sensores
6. Importar sensores
7. Exportar sensores
8. Simular leituras automáticas
0. Sair
Opção: 1
```

---

### **3. Compilar e rodar os testes**
```bash
g++ -std=c++17 -o test test.cpp Sensor.cpp SensorDataBase.cpp ImportExport.cpp -I.
./test
```

Os testes realizam:
- até **100% de ocupação da tabela** (`MAX_SENSORS = 10007`)
- **1.000 buscas** aleatórias
- comparação entre tabela hash e busca linear em vetor
- medição de tempo total, tempo médio e número de colisões

**Exemplo de saída:**
```txt
Iniciando teste...
=== RESULTADOS DOS TESTES ===
Numero de itens inseridos: 10007 (100% de 10007)
Numero de buscas realizadas: 1000
Tamanho da tabela hash: 10007

PERCENTAGE = 100%
INSERCAO:
  Tempo total (Hash): 2.7423 ms
  Tempo medio (Hash): 0.000274038 ms por item
  Tempo total (Convencional): 1.2223 ms
  Tempo medio (Convencional): 0.000122144 ms por item
  Relacao hash / convencional: 2.24356
  Total de colisoes (Hash): 80959
  Media de colisoes (Hash): 8.09024 por item

BUSCA:
  Tempo total (Hash): 19.9325 ms
  Tempo medio (Hash): 0.0199325 ms por busca
  Tempo total (Convencional): 16.135 ms
  Tempo medio (Convencional): 0.016135 ms por busca
  Relacao hash / convencional: 1.23536
=========================================
```

---

## Requisitos

- **Compilador C++**: GCC ou Clang com suporte a C++17
- **Sistema Operacional**: Linux, macOS ou Windows (com WSL/MinGW)
