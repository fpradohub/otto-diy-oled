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
//---------------------------------------------------------
// Face should be in front of you
/*
                 --------------- 
                |     O   O     |
                |---------------|
Left leg =>     |               |    <= Right leg
                 --------------- 
                    ||     ||
Left foot =>     -----   ------     <= Right foot
                 |-----   ------|
*/
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (2)
#define PIN_Echo     9  //ECHO pin (3)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13 //BUZZER pin (13)
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
int distance; // variable to store distance read from ultrasonic range finder module
bool obstacleDetected = false; // logic state for when object detected is at the distance we set
bool obstacleFlag = true; 
//-- Movement parameters
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=15;         //Asociated with the height of some movements
int move = 0;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(19200);   // A comunicação deve estar setada para 19200 para a comunicação com o display oled
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  //Otto wake up!
  Otto.sing(S_connection);// Otto makes a sound
  Otto.home(); // Otto moves to its ready position
  delay(500); // wait for 500 milliseconds to allow Otto to stop
}

void loop() {
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
//   Serial.print(distance);
//   Serial.println("cm");
 if(distance<15){
          obstacleDetected = true;
//          Serial.println ("Obstacle Detected! Avoid collision");
        }else{
          obstacleDetected = false;
//          Serial.println ("No Obstacle detected! - Keep on walking");
        }
}
