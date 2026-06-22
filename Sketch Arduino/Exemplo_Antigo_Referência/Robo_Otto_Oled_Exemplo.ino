/*
 * Sketch Exemplo do OTTO
 * Adaptado para Display Oled por Julian C Braga, Hitech Mecatrônica
 * 21/08/2022
*/

/*
 * Você pode usar qualque Sketch do OTTO com o modeulo Display OLED Nano  
 * Basta incluir na comunicação serial os comandos para a animação dos olhos 
 * comandos aceitos -> "Normal","Happy","SuperHappy","Sad","Victory","Angry","Sleeping",
 * "Fretful","Love","Confused","Fart","Wave","Magic","Fail"
 * Lembre-se de desconectar a comunicação serial RX/TX das placas antes de carregar o arduino
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

//-- Variável Troca de Programa------------------------------------------------------------//
const int botao = 11;
int estadoAtual = 0;
const int totalEstados = 2;

bool ultimoEstadoBotao = HIGH;
unsigned long ultimoDebounce = 0;
const int debounceDelay = 50;

//-- Sensor e movimento--------------------------------------------------------------------//
const int sensorPin = A0;
int estado = HIGH; // primeiro valor esperado do toque
int movement = 0;
boolean izqder = true;

///////////////////////////////////////////////////////////////////
//-- Função para selecionar modo inicial ------------------------//
///////////////////////////////////////////////////////////////////
void selecionarModoInicial() {
  unsigned long tempoInicio = millis();
  Serial.println("Selecione o modo: pressione o botão para alternar");

  while (millis() - tempoInicio < 5000) {
    if (digitalRead(botao) == LOW) {
      delay(200); 
      while (digitalRead(botao) == LOW);
      estadoAtual = (estadoAtual + 1) % totalEstados;
      Serial.print("Modo selecionado: ");
      Serial.println(estadoAtual);
      tempoInicio = millis();
    }
  }

  Serial.print("Modo final selecionado: ");
  Serial.println(estadoAtual);
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
      modoDemonstracao();
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
  Serial.write("Normal"); 
  if (digitalRead(sensorPin) == estado)
  {
    if (estado == HIGH) {
      estado = LOW;
    } else {
      estado = HIGH;
    }
    Otto.sing(S_buttonPushed);
    movement = movement + 1;
    delay(500);
  }

  if (movement == 1)
  {
    if(obstacleDetected){
      Serial.println("OBJETO DETECTADO"); 
      Otto.sing(S_OhOoh);

      if (izqder == true)
      {
        Otto.walk(2,1000,1); 
        Otto.turn(2,1000,1); 
        izqder = false;
      }
      else
      {
        Otto.walk(2,1000,1); 
        Otto.turn(2,1000,-1); 
        izqder = true;
      }

      delay(50); 
      obstacleDetector();
    } 
    else
    {
      Otto.walk(1,500,-1); 
      delay(50);
      obstacleDetector();
    }
  }

  if (movement == 2)
  {
    if(obstacleDetected){ 
      Serial.println("Object detected");  
      Otto.walk(2,1000,-1); 
      Otto.home(); 
      obstacleDetector();
    }        
    else { 
      obstacleDetector(); 
    } 
  }

  if (movement == 3)
  {
    Otto.jitter(10,500,40); 
    Otto.home();
    Otto.moonwalker(2,1000,30,1);
    Otto.home();
    Otto.ascendingTurn(2,500,50); 
    Otto.home();
    Otto.tiptoeSwing(2,1000,30); 
    Otto.home();
    Otto.flapping(2,500,40,1);
    Otto.home();
    Otto.crusaito(2,3000,40,1);
    Otto.home();
    Otto.shakeLeg(2,1000,1);  
    Otto.home();
    Otto.sing(S_disconnection);
    movement = 0;
  }
}
