# Robô Otto DIY com Display OLED

Este repositório contém o código, esquemáticos e arquivos de montagem do meu robô Otto DIY adaptado com Display OLED (SSD1306). Customizei o projeto original de hardware livre para incluir novos modos de operação, músicas e coreografias dinâmicas controladas por botão.

---

## O que o robô faz
* **Controle de Movimento:** Usa 4 servomotores para mover pernas e pés.
* **Display OLED:** Exibe expressões faciais animadas que piscam e reagem.
* **Sensor Ultrassônico:** Usado para navegação autônoma e desvio de obstáculos.
* **Buzzer:** Reproduz efeitos sonoros e melodias.
* **Múltiplos Modos:** Permite selecionar 5 comportamentos diferentes usando um botão físico.

---

## Conexões de Hardware (Mapeamento dos Pinos no Arduino Nano)

### Servomotores (Pernas e Pés)
* **Perna Esquerda (YL):** Pino `2`
* **Perna Direita (YR):** Pino `3`
* **Pé Esquerdo (RL):** Pino `4`
* **Pé Direita (RR):** Pino `5`

### Sensores, Botões e Atuadores
* **Sensor Ultrassônico:** Trigger no pino `8` | Echo no pino `9`
* **Buzzer Sonoro:** Pino `13`
* **Botão de Modo:** Pino `11`
* **Boot Enable (Sinal de Partida):** Pino `10` *(O robô só calibra e inicia quando o pino 10 recebe sinal nível ALTO/HIGH).*

---

## Modos de Operação (Botão no Pino 11)
Ao ligar o robô, você tem 5 segundos para apertar o botão e escolher o comportamento. O buzzer emite bips indicando qual modo foi selecionado:

* **Modo 0 (0 bips) - Demonstração:** Mostra todas as expressões, gestos e desvio básico de obstáculos de forma sequencial.
* **Modo 1 (1 bip) - Desvio Inteligente:** Navegação autônoma usando o sensor ultrassônico, com sons de surpresa e confusão.
* **Modo 2 (2 bips) - Star Wars:** Toca a marcha imperial sincronizada com passos de giros e balanços.
* **Modo 3 (3 bips) - Piratas do Caribe:** Toca a trilha clássica com passos leves e vibrações nos pés do robô.
* **Modo 4 (4 bips) - Michael Jackson (Smooth Criminal):** Coreografia com passos de Moonwalk, Crusaito e Flapping.

---

## Estrutura do Repositório
* `Robo_Otto_Principal/`: Código principal do Arduino Nano.
* `Robo_Otto_Olhos/`: Código de controle do Display OLED e biblioteca de animações.
* `docs/`: Manual de montagem em PDF, diagramas elétricos e links de referência.
* `Sketch Arduino/`: Bibliotecas necessárias e esquema elétrico (`Otto_robot_circuit.png`).
* `Otto.lbrn2`: Arquivo para corte a laser do chassi (LightBurn).

---

## Como configurar e gravar

1. Copie a pasta `Otto_Robot_Lib` (dentro de `Sketch Arduino/`) para a pasta de bibliotecas da sua Arduino IDE (geralmente em `Documentos/Arduino/libraries/`).
2. **Gravar Placa Principal:** Abra o arquivo `Robo_Otto_Principal/Robo_Otto_Principal.ino`. Selecione a placa **Arduino Nano**, processador **ATmega328P** (ou *Old Bootloader*) e faça o upload.
   * *Atenção: desconecte os cabos RX/TX antes de fazer o upload para evitar conflitos na gravação.*
3. **Gravar Placa do Display:** Abra o arquivo `Robo_Otto_Olhos/Robo_Otto_Olhos.ino` e faça o upload na placa do display.

---

## Créditos e Referências
Este projeto foi construído utilizando as seguintes bases da comunidade de hardware livre:
* **[Otto DIY](https://www.ottodiy.com/)** (Camilo Parra Palacio): Projeto mecânico original.
* **[Vishal Soni India](https://github.com/vishalsoniindia)**: Repositório base para controle e esquemáticos.
* **Julian C. Braga (HiTech Mecatrônica)**: Adaptação física inicial para o Display OLED SSD1306.
* **Minhas contribuições:** Adição do botão físico para alternar modos, lógica de máquina de estados, novas melodias (Star Wars, Piratas do Caribe, Michael Jackson) e coreografias sincronizadas.
