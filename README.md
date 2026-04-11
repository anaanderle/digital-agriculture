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
├── ImportExport.h / ImportExport.cpp      # Classe para importação/exportação de sensores
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
- `getSensors()`: Retorna o array de sensores para iteração (usado pela exportação)
- `getMaxSensors()`: Retorna a capacidade máxima da tabela hash
- `getCount()`: Retorna a quantidade de sensores registrados

**Funções de hash:**
- `h1(k)`: Hash primário = `k % maxSensors`
- `h2(k)`: Hash secundário = `1 + (k % (maxSensors - 1))`

---

## 📤 Importação e Exportação de Sensores

### **Classe ImportExport**
Gerencia a importação e exportação de sensores para arquivos de texto.

**Métodos:**
- `importSensors(SensorDataBase& db, inputFile, outputFile)`: Importa sensores de um arquivo TXT
- `exportSensors(const SensorDataBase& db, outputFile)`: Exporta estado completo do banco de dados

### **Importação de Sensores (Opção 6)**

**Formato do arquivo de entrada:**
Cada linha deve conter: `id - type - location`

```
1 - temperatura - cozinha
2 - umidade - quarto
3 - luz - sala
4 - pressao - garagem
```

**Arquivo de resultado:**
Cada linha contém o status da importação: `id - sucesso` ou `id - falha`

```
1 - sucesso
2 - sucesso
3 - sucesso
4 - falha
```

**Recursos:**
- Valida o formato de entrada
- Relata sucesso ou falha para cada sensor
- Detecta IDs duplicados
- Cria arquivo com relatório detalhado

### **Exportação de Sensores (Opção 7)**

**Formato do arquivo de saída:**
Cada linha contém todas as informações de um sensor: `id, type, location, lastReading, lastUpdate, [history]`

```
1, temperatura, cozinha, 23.5, 11/04/2026 20:07:19, [23.5, 24.1, 22.8]
2, umidade, quarto, 65.3, 11/04/2026 20:07:19, [65.3]
3, luz, sala, 500, (nunca), []
```

**Recursos:**
- Exporta TODOS os sensores na ordem do banco de dados
- Inclui histórico completo de leituras
- Formata datas em padrão DD/MM/YYYY HH:MM:SS
- Marca sensores sem leitura como "(nunca)"
- Mostra quantidade de sensores exportados

---

## 🚀 Como Compilar e Rodar

### **1. Compilar o programa principal:**
```bash
g++ -std=c++17 -o main main.cpp Sensor.cpp SensorDataBase.cpp ImportExport.cpp -I.
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
- Importar sensores de um arquivo
- Exportar sensores para um arquivo
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
6. Importar sensores
7. Exportar sensores
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
g++ -std=c++17 -o test test.cpp Sensor.cpp SensorDataBase.cpp ImportExport.cpp -I.
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