
/* 
*  Sketch original escrito por Julian C Braga, HiTech Mecatrônica
*  21/08/2022
*/

// Inclusão de bibliotecas
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Animations.h"
  
#define SCREEN_WIDTH 128 // Largura display, em pixels
#define SCREEN_HEIGHT 64 // Altura display, em pixels
  
// Declaração pino reset para I2C
#define OLED_RESET  -1 // Será o mesmo reset da placa
// Criação objeto display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Altura e largura ANIMACAO (alterar tamanho se necessário)
#define ANIMACAO_HEIGHT   64
#define ANIMACAO_WIDTH  128

String RX_Serial = "Normal";

void setup() {
  // Início comunicação serial com 19200 bps
  Serial.begin(19200);
  
  // SSD1306_SWITCHCAPVCC = gerar tensão para display 3.3V internamente
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C sendo o end I2C
    Serial.println(F("Falha alocação SSD1306"));
    for(;;); // Não processado, loop infinito
  }
  // Limpa display
  display.clearDisplay();
}
  
// Sub-rotina para carregar o display
void PrintDisplay(const unsigned char Bitmap[]) {
  //Limpa o display
  display.clearDisplay();  
  // Preenche a tela com bitmap
  display.drawBitmap((display.width()  - ANIMACAO_WIDTH ) / 2, (display.height() - ANIMACAO_HEIGHT) / 2, Bitmap, ANIMACAO_WIDTH, ANIMACAO_HEIGHT, 1);
  // Exibe no display 
  display.display();
}
void Wave(void){
  PrintDisplay(Y_Wave);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40,0);
  display.print("WAVE");
  display.display();
  delay(500);
  display.clearDisplay();
  display.display();
  delay(100);
}
void Magic(void){
  PrintDisplay(Y_Wave);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(35,0);
  display.print("MAGIC");
  display.display();
  delay(500);
  display.clearDisplay();
  display.display();
  delay(100);
}


void Aberto(void){
  display.clearDisplay();  
  display.fillRoundRect( 9, 16, 46, 48, 12, WHITE);  //drawRoundRect(x, y, largura, altura, raio, cor)
  display.fillRoundRect(73, 16, 46, 48, 12, WHITE);
  display.display();
  delay(1000);
}
void Pisca(void){
  //Meio Aberto
  display.clearDisplay();  
  display.fillRoundRect( 9, 24, 46, 32, 10, WHITE);  //drawRoundRect(x, y, largura, altura, raio, cor)
  display.fillRoundRect(73, 24, 46, 32, 10, WHITE);
  display.display();
  delay(30); //30
  //Meio Fechado
  display.clearDisplay();  
  display.fillRoundRect( 9, 32, 46, 16, 10, WHITE);  //drawRoundRect(x, y, largura, altura, raio, cor)
  display.fillRoundRect(73, 32, 46, 16, 10, WHITE);
  display.display();
  delay(30); //30

  //Fechado
  display.clearDisplay();  
  display.fillRoundRect( 9, 37, 46, 6, 10, WHITE);  //drawRoundRect(x, y, largura, altura, raio, cor)
  display.fillRoundRect(73, 37, 46, 6, 10, WHITE);
  display.display();
  delay(20); //20

}  

/**
 * Função que lê uma string da Serial
 * e retorna-a
 */
String leStringSerial(){
  String RX_SD = "";              
  char caractere;
  while(Serial.available() > 0) {       // Enquanto receber algo pela serial
    caractere = Serial.read();          // Lê byte da serial
    if (caractere != '\n'){             // Ignora caractere de quebra de linha 
      RX_SD.concat(caractere);          // Concatena valores
    }
    delay(10);                          // Aguarda buffer serial ler próximo caractere
  }
  Serial.print("Recebi: ");
  Serial.println(RX_SD);
  return RX_SD;
}
//---------------------------------
void loop() {
  if (Serial.available() > 0){  // Se receber algo pela serial
    RX_Serial = leStringSerial(); // Lê toda string recebida
  }
  if (RX_Serial == "Normal"    ) {Aberto(); Pisca();}                                                // Normal
  if (RX_Serial == "Happy"     ) {PrintDisplay(Y_Happy); delay(1000); Pisca();}                      // Feliz  
  if (RX_Serial == "SuperHappy") {PrintDisplay(Y_Happy); delay(1200); Pisca();}                      // Super Feliz  
  if (RX_Serial == "Sad"       ) {PrintDisplay(Y_Sad); delay(1000); Pisca();}                        // Triste   
  if (RX_Serial == "Victory"   ) {PrintDisplay(Y_Victory); delay(1000);}                            // Vitorioso   
  if (RX_Serial == "Angry"     ) {PrintDisplay(Y_Angry); delay(1000); Pisca();}                      // Com raiva   
  if (RX_Serial == "Sleeping"  ) {PrintDisplay(Y_Sleeping); delay(1000);}                            // Dormindo   
  if (RX_Serial == "Fretful"   ) {PrintDisplay(Y_Angry); delay(1000);Pisca();}                       // Irritado    
  if (RX_Serial == "Love"      ) {PrintDisplay(Y_Love); delay(1000);Pisca();}                        // Apaixonado 
  if (RX_Serial == "Confused"  ) {PrintDisplay(Y_Confused); delay(1000);Pisca();}                    // Confused Confuso   
  if (RX_Serial == "Fart"      ) {PrintDisplay(Y_Fart); delay(1000);Pisca();}                        // Peidano    
  if (RX_Serial == "Wave"      ) {Wave();}                                                           // Onda, Sirene
  if (RX_Serial == "Magic"     ) {Magic();}                                                          // Magico, Sirene
  if (RX_Serial == "Fail"      ) {PrintDisplay(Y_Fail); delay(1000);}                                // Caiu, falhou   

}// Fim do sketch
