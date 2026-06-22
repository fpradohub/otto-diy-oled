# 🤖 Robô Otto DIY com Display OLED

Este repositório contém o projeto de um robô **Otto DIY** adaptado com **Display OLED**, desenvolvido e customizado para incluir múltiplos modos de operação, músicas e coreografias dinâmicas.

O projeto inclui o código-fonte Arduino (.ino), manual de montagem em PDF, diagramas de circuito, bibliotecas necessárias e arquivos de corte a laser para o chassi.

---

## 📋 Índice
- [Características do Projeto](#-características-do-projeto)
- [Conexões de Hardware (Pinos)](#-conexões-de-hardware-pinos)
- [Instruções Importantes de Operação](#-instruções-importantes-de-operação)
- [Modos de Operação](#-modos-de-operação)
- [Animações dos Olhos (Display OLED)](#-animações-dos-olhos-display-oled)
- [Estrutura do Repositório](#-estrutura-do-repositório)
- [Como Configurar e Gravar](#-como-configurar-e-gravar)
- [Manual de Montagem e Vídeos](#-manual-de-montagem-e-vídeos)
- [Licença](#-licença)

---

## ⚡ Características do Projeto
*   **Controle de Movimento:** 4 servomotores controlando pernas e pés.
*   **Feedback Visual:** Display OLED Nano que exibe diferentes expressões faciais animadas.
*   **Sensor Ultrassônico:** Para navegação autônoma e desvio de obstáculos.
*   **Feedback Sonoro:** Buzzer para reprodução de efeitos e melodias famosas.
*   **Múltiplos Modos:** Seleção de 5 modos diferentes de comportamento através de um botão táctil.

---

## 🔌 Conexões de Hardware (Pinos)

Abaixo está o mapeamento dos pinos utilizados na placa Arduino Nano:

### 👣 Servomotores (Pernas e Pés)
*   **Perna Esquerda (YL):** Pino `2`
*   **Perna Direita (YR):** Pino `3`
*   **Pé Esquerdo (RL):** Pino `4`
*   **Pé Direita (RR):** Pino `5`

### 📡 Sensores e Atuadores
*   **Ultrassônico TRIGGER:** Pino `8`
*   **Ultrassônico ECHO:** Pino `9`
*   **Buzzer Sonoro:** Pino `13`
*   **Botão de Seleção (Modo):** Pino `11`
*   **Sinal de Partida (Boot Enable):** Pino `10` *(Veja as notas de inicialização abaixo)*

---

## ⚠️ Instruções Importantes de Operação

### 🚀 Sequência de Inicialização (Boot)
O código contém uma verificação de segurança no `setup()`. O robô só iniciará a calibração e a seleção de modos **depois que o pino `10` receber um sinal nível ALTO (HIGH)**.
```cpp
while (digitalRead(10) == LOW) {
  delay(100);
}
```
Certifique-se de que a sua eletrônica está configurada para enviar o sinal HIGH neste pino para que o robô seja ativado.

### 📥 Gravação do Arduino (Upload)
> [!WARNING]
> Sempre **desconecte os cabos de comunicação serial (RX/TX)** do Arduino antes de fazer o upload do sketch pelo Arduino IDE, para evitar conflitos na gravação.

### 📐 Calibração dos Motores
Os movimentos de frente e para trás consideram o eixo do motor do Otto virado para a **parte frontal** do robô.

---

## 🕹️ Modos de Operação

Ao ligar o robô, você tem **5 segundos** para pressionar o botão (Pino `11`) e selecionar o modo desejado. O Buzzer emitirá bips indicando qual modo foi selecionado:

*   **Modo 0: Modo Demonstração (0 bips)**
    *   Exibe sequencialmente todas as expressões nos olhos, executa gestos e ativa o desvio de obstáculo básico com movimentos aleatórios.
*   **Modo 1: Modo Desvio de Obstáculo (1 bip)**
    *   Navegação inteligente utilizando o sensor ultrassônico, com sons de surpresa, confusão e reações divertidas ao encontrar barreiras.
*   **Modo 2: Tema Star Wars (2 bips)**
    *   Toca a melodia clássica de Star Wars sincronizada com uma coreografia de giros e balanços.
*   **Modo 3: Modo Pirata (3 bips)**
    *   Toca a famosa trilha sonora de Piratas do Caribe com passos leves e vibrações nos pés do robô.
*   **Modo 4: Smooth Criminal (4 bips)**
    *   Coreografia especial inspirada em Michael Jackson (Smooth Criminal), incluindo passos de Moonwalk, Crusaito, Swing e Flapping.

---

## 👁️ Animações dos Olhos (Display OLED)

As expressões oculares são controladas através de comandos enviados pela Comunicação Serial (Baud Rate: `19200`). Os seguintes comandos de texto são aceitos pelo display:
*   `Normal` | `Happy` | `SuperHappy` | `Sad` | `Victory` | `Angry`
*   `Sleeping` | `Fretful` | `Love` | `Confused` | `Fart` | `Wave`
*   `Magic` | `Fail`

---

## 📁 Estrutura do Repositório

```
Otto/
├── Robo_Otto_Principal/
│   └── Robo_Otto_Principal.ino       # Código da placa principal do robô (Arduino Nano)
├── Robo_Otto_Olhos/
│   ├── Robo_Otto_Olhos.ino           # Código do controlador do Display OLED (olhos)
│   └── Animations.h                  # Definições de animações dos olhos
├── docs/
│   ├── Montagem OTT0 OLED JCB.pdf    # Manual em PDF com passo a passo ilustrado
│   ├── conexoes_robo.txt             # Mapeamento rápido de pinos
│   └── video_montagem.txt            # Link do vídeo de referência no YouTube
├── Sketch Arduino/                   # Bibliotecas originais, diagramas e esquemáticos
│   ├── Otto_Robot_Lib/               # Biblioteca oficial Otto9
│   ├── Otto_robot_circuit.png        # Esquema elétrico das ligações
│   └── Exemplo_Antigo_Referência/    # Código de exemplo antigo para referência
├── Otto.lbrn2                        # Arquivo de corte a laser (LightBurn)
├── .gitignore                        # Regras para ignorar arquivos desnecessários no git
├── LICENSE                           # Licença MIT
└── README.md                         # Este arquivo de documentação
```

---

## 🛠️ Como Configurar e Gravar

1.  Baixe e instale a versão mais recente do [Arduino IDE](https://www.arduino.cc/en/software).
2.  Copie a pasta `Otto_Robot_Lib` (localizada dentro de `Sketch Arduino/`) para a pasta de bibliotecas do seu Arduino (geralmente em `Documentos/Arduino/libraries/`).
3.  **Gravar Placa Principal:**
    *   Abra o arquivo `Robo_Otto_Principal/Robo_Otto_Principal.ino` no Arduino IDE.
    *   Selecione a placa **Arduino Nano**, o processador correspondente (ex: ATmega328P ou ATmega328P Old Bootloader) e a porta COM correspondente.
    *   Desconecte os pinos RX/TX da placa antes de carregar o código para evitar conflitos na gravação.
    *   Clique em **Carregar (Upload)**.
4.  **Gravar Placa do Display (Olhos):**
    *   Abra o arquivo `Robo_Otto_Olhos/Robo_Otto_Olhos.ino` no Arduino IDE.
    *   Carregue o código na placa correspondente do display OLED seguindo o mesmo processo de upload.

---

## 📚 Manual de Montagem e Vídeos

*   O passo a passo detalhado de montagem mecânica e estrutura 3D está detalhado no manual: [Montagem OTT0 OLED JCB.pdf](docs/Montagem%20OTT0%20OLED%20JCB.pdf).
*   Vídeo complementar de montagem: [Assistir no YouTube](https://www.youtube.com/watch?v=geUmuX8xXqc&t=333s).

---

## 📄 Licença

Este projeto está licenciado sob a Licença MIT. Consulte o arquivo [LICENSE](LICENSE) para obter mais informações.
