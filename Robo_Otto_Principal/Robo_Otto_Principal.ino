/*
 * Sketch Exemplo do OTTO
 * Baseado no projeto original "Arduino_Otto_Robot" por Vishal Soni (github.com/vishalsoniindia)
 * Adaptado para Display Oled por Julian C Braga, Hitech Mecatrônica 21/08/2022
 * Customizado e adaptado para múltiplos modos por Fernando Prado 16/09/2025
 */

/*
 * Você pode usar qualquer Sketch do OTTO com o modelo Display OLED Nano  
 * Basta incluir na comunicação serial os comandos para a animação dos olhos 
 * comandos aceitos -> "Normal","H  appy","SuperHappy","Sad","Victory","Angry","Sleeping",
 * "Fretful","Love","Confused","Fart","Wave","Magic","Fail"
 * Lembre-se de desconectar a comunicação serial RX/TX das placas antes de carregar o arduino
 * Movimentos para frente e para traz considerando o eixo do motor do otto virado para parte frontal do robo.
*/

#include <Otto9.h> //-- Otto Library version 9
Otto9 Otto;  //This is Otto!
#include <Servo.h>

// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot

#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define Buzzer  13 
#define Trigger 8 // ultrasonic sensor trigger pin
#define Echo 9 // ultrasonic sensor echo pin

// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (2)
#define PIN_Echo     9  //ECHO pin (3)

// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13 //BUZZER pin (13)

//-- Global Variables ---------------------------------------------------------------------//
int distance; 
bool obstacleDetected = false; 
bool obstacleFlag = true; 
int T = 1000;
int moveId = 0;
int moveSize = 15;
int move = 0;
bool modoObstacleIniciado = false;
bool normalEnviado = false;
//-- Variável Troca de Programa------------------------------------------------------------//
const int botao = 11;
int estadoAtual = 0;
const int totalEstados = 5;

bool ultimoEstadoBotao = HIGH;
unsigned long ultimoDebounce = 0;
const int debounceDelay = 50;

//-- Sensor e movimento--------------------------------------------------------------------//
int estado = HIGH; // primeiro valor esperado do toque
int movement = 0;
boolean izqder = true;

///////////////////////////////////////////////////////////////////
//-- Função para selecionar modo inicial ------------------------//
///////////////////////////////////////////////////////////////////
void selecionarModoInicial() {
  unsigned long tempoInicio = millis();
  //Serial.println("Selecione o modo: pressione o botão para alternar");

  while (millis() - tempoInicio < 5000) {
    if (digitalRead(botao) == LOW) {
      delay(200); 
      while (digitalRead(botao) == LOW);
      estadoAtual = (estadoAtual + 1) % totalEstados;

      //Serial.print("Modo selecionado: ");
      //Serial.println(estadoAtual);

      // ➕ Toca os bips imediatamente ao selecionar
      sinalizarModoSelecionado(estadoAtual);

      tempoInicio = millis(); // reinicia o tempo para dar mais 5s
    }
  }

  //Serial.print("Modo final selecionado: ");
  //Serial.println(estadoAtual);
}

///////////////////////////////////////////////////////////////////
//-- Notas Músicais--------------------- ------------------------//
///////////////////////////////////////////////////////////////////
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_REST 0


void tocarNota(int frequencia, int duracao) {
  if (frequencia > 0) {
    tone(PIN_Buzzer, frequencia, duracao);
  }
  delay(duracao * 1.3); // pausa entre notas
}

///////////////////////////////////////////////////////////////////
//-- Sensor ultrassônico ----------------------------------------//
///////////////////////////////////////////////////////////////////
long ultrasound() {
   long duration, distance;
   digitalWrite(PIN_Trigger, LOW);
   delayMicroseconds(2);
   digitalWrite(PIN_Trigger, HIGH);
   delayMicroseconds(10);
   digitalWrite(PIN_Trigger, LOW);
   duration = pulseIn(PIN_Echo, HIGH);
   distance = duration / 58;
   return distance;
}

///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  pinMode(botao, INPUT_PULLUP);
  pinMode(10, INPUT); // Pino que será usado para receber o sinal High.
 
  while (digitalRead(10) == LOW) {
    delay(100);
  }
  delay(500);
  Serial.begin(19200);
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); 
  Otto.sing(S_connection);
  Otto.home(); 
  delay(500); 

  while (digitalRead(10) == LOW) {
    delay(100);
  }

  selecionarModoInicial();
}
///////////////////////////////////////////////////////////////////
//--Toque para seleção do programa-------------------------------//
///////////////////////////////////////////////////////////////////
void sinalizarModoSelecionado(int modo) {
  noTone(PIN_Buzzer);
  delay(100);

  int freq1 = NOTE_C5;  // tom principal
  int freq2 = NOTE_G5;  // tom secundário para variar

  for (int i = 0; i <= modo; i++) {
    tone(PIN_Buzzer, freq1, 100);
    delay(120);
    tone(PIN_Buzzer, freq2, 80);
    delay(100);
  }

  noTone(PIN_Buzzer);
}
///////////////////////////////////////////////////////////////////
//-- Loop -------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  bool leituraBotao = digitalRead(botao);
  if (leituraBotao != ultimoEstadoBotao) {
    ultimoDebounce = millis();
  }

  if ((millis() - ultimoDebounce) > debounceDelay) {
    if (leituraBotao == LOW && ultimoEstadoBotao == HIGH) {
      estadoAtual = (estadoAtual + 1) % totalEstados;
      ultimoEstadoBotao = leituraBotao;
      delay(500);
      return;
    }
  }

  ultimoEstadoBotao = leituraBotao;

  switch (estadoAtual) {
    case 0:
      modoDemonstracao();
      break;
    case 1:
      obstacle();
      break;
    case 2:
      modoStarWarsShow();
      break;
    case 3:
      modoPirata();
      break;
    case 4:
      modoSmoothCriminal();
      break;
   
  }
  
}

///////////////////////////////////////////////////////////////////
//-- Modo Demonstração ------------------------------------------//
///////////////////////////////////////////////////////////////////
void modoDemonstracao() {
  Serial.write("Happy");    
  Otto.playGesture(OttoHappy);
  Serial.write("SuperHappy");    
  Otto.playGesture(OttoSuperHappy);
  Serial.write("Sad");    
  Otto.playGesture(OttoSad);
  Serial.write("Victory");    
  Otto.playGesture(OttoVictory); 
  Serial.write("Angry");    
  Otto.playGesture(OttoAngry); 
  Serial.write("Sleeping");    
  Otto.playGesture(OttoSleeping);
  Serial.write("Fretful");    
  Otto.playGesture(OttoFretful);
  Serial.write("Love");    
  Otto.playGesture(OttoLove);
  Serial.write("Confused");    
  Otto.playGesture(OttoConfused);        
  Serial.write("Fart");    
  Otto.playGesture(OttoFart);
  Serial.write("Wave");    
  Otto.playGesture(OttoWave);
  Serial.write("Magic");    
  Otto.playGesture(OttoMagic);
  Serial.write("Fail");    
  Otto.playGesture(OttoFail);
  Serial.write("Normal");    
  Otto.home();
  delay(500); // wait for 500 milliseconds to allow Otto to stop
  while(1==1){ 
    obstacleDetector();
    if(obstacleDetected == true && obstacleFlag ==  true){
      Serial.write("Confused");  //Espressão do olho
      Otto.playGesture(OttoConfused);
     obstacleFlag =  false;
    }
    if(obstacleDetected == false && obstacleFlag ==  false){
     move = random(1,27);
     obstacleFlag =  true;
    }
    switch(move){
     case 1:
       Serial.write("Normal");  //Espressão do olho
       Otto.walk(2,1000,1);
       Otto.home();
       move = 0;
       break;
     case 2:
       Serial.write("Normal");  //Espressão do olho
       Otto.walk(2,1000,-1);
       Otto.home();
       move = 0;
       break;
     case 3:
       Serial.write("Normal");  //Espressão do olho
       Otto.turn(2,1000,1);
       Otto.home();
       move = 0;
       break;
     case 4:
       Serial.write("Normal");  //Espressão do olho
       Otto.turn(2,1000,-1);
       Otto.home();
       move = 0;
       break;
       case 5:
       Serial.write("Normal");  //Espressão do olho
       Otto.bend (2,500,1);
       Otto.home();
       move = 0;
       break;
       case 6:
       Serial.write("Normal");  //Espressão do olho
       Otto.bend (2,2000,-1);
       Otto.home();
       move = 0;
       break;
     case 7:
       Serial.write("Normal");  //Espressão do olho
       Otto.shakeLeg (2,1500, 1);
       Otto.home();
       move = 0;
       break;
     case 8:
       Serial.write("Normal");  //Espressão do olho
       Otto.moonwalker(3, 1000, 25,1);
       Otto.home();
       move = 0;
       break;
     case 9:
       Serial.write("Normal");  //Espressão do olho
       Otto.moonwalker(3, 1000, 25,-1);
       Otto.home();
       move = 0;
       break;
     case 10:
       Serial.write("Normal");  //Espressão do olho
       Otto.crusaito(2, 1000, 20,1);
       Otto.home();
       move = 0;
       break;
     case 11:
       Serial.write("Normal");  //Espressão do olho
       Otto.crusaito(2, 1000, 20,-1);
       Otto.home();
       move = 0;
       break;
     case 12:
       Serial.write("Normal");  //Espressão do olho
       Otto.flapping(2, 1000, 20,1);
       Otto.home();
       move = 0;
       break;
     case 13:
       Serial.write("Normal");  //Espressão do olho
       Otto.flapping(2, 1000, 20,-1);
       Otto.home();
       move = 0;
       break;
     case 14:
       Serial.write("Normal");  //Espressão do olho
       Otto.swing(2, 1000, 20);
       Otto.home();
       move = 0;
       break;
       case 15:
       Serial.write("Normal");  //Espressão do olho
       Otto.tiptoeSwing(2, 1000, 20);
       Otto.home();
       move = 0;
       break;
     case 16:
       Serial.write("Normal");  //Espressão do olho
       Otto.jitter(2, 1000, 20); //(small T)
       Otto.home();
       move = 0;
       break;
     case 17:
       Serial.write("Normal");  //Espressão do olho
       Otto.updown(2, 1500, 20);  // 20 = H "HEIGHT of movement"T 
       Otto.home();
       move = 0;
       break;
     case 18:
       Serial.write("Normal");  //Espressão do olho
       Otto.ascendingTurn(2, 1000, 50);
       Otto.home();
       move = 0;
       break;
     case 19:
       Serial.write("Normal");  //Espressão do olho
       Otto.jump(1,500); // It doesn't really jumpl ;P
       Otto.home();
       move = 0;
       break;
     case 20:
       Serial.write("SuperHappy");  //Espressão do olho
       Otto.playGesture(OttoSuperHappy);
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
       break;
     case 21:
       Serial.write("Sad");  //Espressão do olho
       Otto.playGesture(OttoSad);
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
       break;
     case 22:
       Serial.write("Victory");  //Espressão do olho
       Otto.playGesture(OttoVictory); 
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
       break;
     case 23:
       Serial.write("Angry");  //Espressão do olho
       Otto.playGesture(OttoAngry);
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
       break;
     case 24:
       Serial.write("Sleeping");  //Espressão do olho
       Otto.playGesture(OttoSleeping);
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
       break;
     case 25:
       Serial.write("Love");  //Espressão do olho
       Otto.playGesture(OttoLove);
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
       break;
     case 26:
       Serial.write("Fart");  //Espressão do olho
       Otto.playGesture(OttoFart);
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
       break;
     case 27:
       Serial.write("Fail");  //Espressão do olho
       Otto.playGesture(OttoFail);
       Otto.home();
       move = 0;
       Serial.write("Normal");  //Espressão do olho
     break;
     }
   }
}
void obstacleDetector() {
  int distance = Otto.getDistance(); // get the distance from the ultrasonic range finder
  if (distance < 15) obstacleDetected = true; // check to see if this distance is closer than 15cm, true if it is
  else obstacleDetected = false;// false if it is not
}

///////////////////////////////////////////////////////////////////
//-- Modo Obstáculo ---------------------------------------------//
///////////////////////////////////////////////////////////////////

void obstacle() {
  obstacleDetector(); // Atualiza estado do sensor de obstáculos

  // Inicializa modo de obstáculo com animação inicial
  if (!modoObstacleIniciado) {
    //Serial.println("Starting Obstacle Mode!");
    Otto.playGesture(OttoHappy); // Animação inicial para engajar
    delay(100);
    modoObstacleIniciado = true;
    normalEnviado = false;
    movement = 0;
  }

  // Máquina de estados para controle de movimentos
  switch (movement) {
    case 0: // Modo de exploração - movimento rápido e fluido
      if (!normalEnviado) {
        //Serial.println("Exploring...");
        Otto.sing(S_connection); // Som de início de exploração
        normalEnviado = true;
      }
      if (obstacleDetected) {
        //Serial.println("Obstacle Detected!");
        Otto.sing(S_surprise); // Som de surpresa ao detectar obstáculo
        Otto.playGesture(OttoConfused); // Animação de confusão
        Otto.walk(4, 600, 1); // Recua rapidamente (para trás, motor invertido)
        Otto.turn(random(4, 7), 700, random(0, 2) ? 1 : -1); // Giro mais amplo e aleatório
        izqder = random(0, 2); // Aleatoriza direção para próxima iteração
        Otto.home();
        movement = 1; // Transição para modo de análise
      } else {
        Otto.walk(5, 500, -1); // Movimento rápido para frente (motor invertido)
        if (random(10) < 3) { // Adiciona variação ocasional
          Otto.sing(S_happy_short); // Som curto de animação
          Otto.swing(1, 800, 20); // Pequeno balanço para animação
        }
      }
      break;

    case 1: // Modo de análise - verifica obstáculo e decide ação
      delay(150); // Pausa reduzida para maior fluidez
      obstacleDetector();
      if (obstacleDetected) {
        //Serial.println("Persistent Obstacle!");
        Otto.sing(S_sad); // Som de frustração
        Otto.playGesture(OttoSad);
        Otto.walk(3, 700, 1); // Recua com cuidado (para trás, motor invertido)
        Otto.turn(5, 600, izqder ? 1 : -1); // Giro mais longo para desviar
        izqder = !izqder; // Alterna direção para evitar repetição
        Otto.home();
      } else {
        //Serial.println("Path Clear!");
        Otto.sing(S_happy); // Som de alívio
        Otto.playGesture(OttoVictory); // Animação de sucesso
        movement = 2; // Transição para modo de progressão
      }
      break;

    case 2: // Modo de progressão - movimento dinâmico com variações
      obstacleDetector();
      if (obstacleDetected) {
        //Serial.println("New Obstacle!");
        Otto.sing(S_OhOoh2); // Som de alerta
        Otto.playGesture(OttoFretful); // Animação de preocupação
        Otto.walk(3, 600, 1); // Recua rapidamente (para trás, motor invertido)
        Otto.turn(random(4, 6), 600, random(0, 2) ? 1 : -1); // Giro amplo e aleatório
        izqder = random(0, 2); // Aleatoriza direção
      } else {
        //Serial.println("Advancing!");
        Otto.walk(6, 450, -1); // Movimento rápido e contínuo para frente
        if (random(10) < 4) { // Adiciona variação para tornar interessante
          Otto.moonwalker(1, 900, 25, random(0, 2) ? 1 : -1); // Moonwalk ocasional
          Otto.sing(S_cuddly);
        }
      }
      movement = 0; // Volta ao modo de exploração
      break;
  }
}
///////////////////////////////////////////////////////////////////
//-- Modo StarWarsShow ------------------------------------------//
///////////////////////////////////////////////////////////////////
void modoStarWarsShow() {
  Otto.home();
  Serial.write("SuperHappy");
  Otto.playGesture(OttoSuperHappy);

  // Tema Star Wars completo e leve
  int melodia[] = {
    NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5,
    NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_F5, NOTE_C5, NOTE_GS4, NOTE_A4,
    NOTE_F4, NOTE_C5, NOTE_A4,
    NOTE_A5, NOTE_A4, NOTE_A4,
    NOTE_A5, NOTE_GS5, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_FS5,
    NOTE_REST, NOTE_AS4, NOTE_DS5, NOTE_D5, NOTE_CS5,
    NOTE_C5, NOTE_B4, NOTE_C5
  };

  int duracoes[] = {
    350, 350, 350, 700,
    350, 350, 350, 700,
    250, 250, 250,
    350, 350, 350, 700,
    350, 350, 700,
    250, 250, 250,
    250, 250, 250, 250, 250, 500,
    200, 200, 400, 250, 400,
    250, 250, 600
  };

  int n = sizeof(melodia) / sizeof(int);

  for (int i = 0; i < n; i++) {
    int nota = melodia[i];
    int duracao = duracoes[i];

    if (nota > 0) {
      tone(PIN_Buzzer, nota, duracao);
    }

    // Sincroniza com movimentos
    if (i % 6 == 0) {
      Otto.turn(1, 300, 1);
    } else if (i % 6 == 3) {
      Otto.turn(1, 300, -1);
    } else if (i % 5 == 0) {
      Otto.shakeLeg(1, 300, 1);
    } else if (i % 4 == 0) {
      Otto.swing(1, 300, 10);
    }

    delay(duracao * 1.3);
    noTone(PIN_Buzzer);
  }

  // Encerramento dramático
  Serial.write("Victory");
  Otto.playGesture(OttoVictory);
  Otto.home();
  Otto.sing(S_happy);
  Serial.write("Normal");
}

void modoPirata() {
  Serial.write("Angry");
  Otto.playGesture(OttoAngry);
  Otto.home();
  delay(300);


  // 🎵 Música Piratas do Caribe (versão longa)
  int melodia[] = {
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4,
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4,
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4,
    NOTE_C5, NOTE_A4, NOTE_C5, NOTE_E5,
    NOTE_D5, NOTE_C5, NOTE_B4,
    NOTE_A4, NOTE_E4, NOTE_G4, NOTE_A4,
    NOTE_B4, NOTE_C5, NOTE_D5,
    NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5,
    NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_A4,

    // BLOCO EXTRA
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4,
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4,
    NOTE_A4, NOTE_B4, NOTE_C5,
    NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4
  };

  int duracoes[] = {
    200, 200, 400, 200,
    200, 400, 200, 200,
    200, 200, 400, 200,
    400, 200, 200, 200,
    200, 200, 400,
    200, 200, 200, 400,
    200, 200, 400,
    200, 200, 200, 200,
    400, 200, 200, 600,

    // DURAÇÕES BLOCO EXTRA
    200, 200, 400, 200,
    200, 400, 200, 200,
    400, 200, 400, 200,
    400, 200, 400,
    200, 200, 400, 600
  };

  int totalNotas = sizeof(melodia) / sizeof(int);

  // 🎶 Primeiro trecho sem movimento
  int notasIntro = 12;
  for (int i = 0; i < notasIntro; i++) {
    tone(PIN_Buzzer, melodia[i], duracoes[i]);
    delay(duracoes[i]);
    noTone(PIN_Buzzer);
    delay(20);
  }

  delay(150);

  // 🎶 Resto com movimentos leves
  for (int i = notasIntro; i < totalNotas; i++) {
    tone(PIN_Buzzer, melodia[i], duracoes[i]);
    delay(duracoes[i]);
    noTone(PIN_Buzzer);
    delay(20);

    if ((i + 1) % 4 == 0) {
      Otto.shakeLeg(1, 80, RIGHT);
      delay(80);
    }
  }

  // Encerramento
  Serial.write("Love");
  Otto.playGesture(OttoLove);
  Otto.sing(S_connection);
  Otto.home();
  delay(300);
  Serial.write("Normal");
}

  // Adicione após modoDesvioDramatico no primeiro código

///////////////////////////////////////////////////////////////////
//-- Modo Smooth Criminal ---------------------------------------//
///////////////////////////////////////////////////////////////////
void modoSmoothCriminal() {
  Otto.home();
  Serial.write("SuperHappy");
  Otto.playGesture(OttoSuperHappy);
  delay(500);

  // Melodia simplificada de Smooth Criminal
  int melodia[] = {
    NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5,
    NOTE_A4, NOTE_G4, NOTE_A4, NOTE_E5,
    NOTE_F5, NOTE_C5, NOTE_A4, NOTE_G4,
    NOTE_A4, NOTE_C5, NOTE_A4
  };

  int duracoes[] = {
    250, 250, 500, 500,
    250, 250, 500, 500,
    250, 250, 500, 500,
    250, 500, 500
  };

  int totalNotas = sizeof(melodia) / sizeof(int);

  // Primeira parte: introdução com movimentos suaves
  for (int i = 0; i < totalNotas; i++) {
    tocarNota(melodia[i], duracoes[i]);
    if (i % 4 == 0) {
      Otto.swing(1, 500, 20);
    } else if (i % 4 == 2) {
      Otto.tiptoeSwing(1, 500, 20);
    }
  }
  Otto.home();
  Serial.println("Normal");

  // Sequência de dança principal
  Otto.moonwalker(4, 1000, 25, 1);  // moonWalkLeft
  Serial.println("Happy");
  Otto.moonwalker(4, 1000, 25, -1); // moonWalkRight
  Otto.moonwalker(4, 1000, 25, 1);
  Otto.moonwalker(4, 1000, 25, -1);
  Otto.home();

  Otto.crusaito(1, 4000, 20, 1);
  Serial.println("SuperHappy");
  Otto.crusaito(1, 3500, 20, -1);
  Otto.home();

  for (int i = 0; i < 8; i++) {
    Otto.flapping(1, 250, 20, 1);
    delay(750);
  }
  Serial.println("Normal");

  Otto.moonwalker(4, 1000, 25, -1);
  Otto.moonwalker(4, 1000, 25, 1);
  Otto.moonwalker(4, 1000, 25, -1);
  Otto.moonwalker(4, 1000, 25, 1);
  Otto.home();

  // Sequência "drunk"
  for (int i = 0; i < 4; i++) {
    Otto.jitter(1, 2000, 20);
    Serial.println("Confused");
    Otto.home();
  }

  // Chutes
  Otto.shakeLeg(1, 1000, 1);  // kickRight
  Serial.println("Angry");
  Otto.shakeLeg(1, 1000, -1); // kickLeft
  Otto.home();

  // Finalização com "noGravity" e "goingUp"
  Otto.updown(2, 1000, 20);
  Serial.println("Victory");
  Otto.updown(1, 500, 20);
  Otto.ascendingTurn(2, 1000, 40);
  Otto.home();

  Otto.crusaito(1, 1000, 20, 1);
  Otto.crusaito(1, 4000, 20, -1);
  Otto.crusaito(1, 1000, 20, 1);
  Otto.crusaito(1, 4000, 20, -1);
  Otto.crusaito(1, 1000, 20, 1);
  Otto.crusaito(1, 1500, 20, -1);
  Otto.home();

  // Final com flapping e música
  for (int i = 0; i < totalNotas; i++) {
    tocarNota(melodia[i], duracoes[i]);
    Otto.flapping(1, 500, 20, 1);
    delay(500);
  }

  // Encerramento
  Serial.println("Victory");
  Otto.playGesture(OttoVictory);
  Otto.sing(S_happy);
  Otto.home();
  Serial.println("Normal");
}