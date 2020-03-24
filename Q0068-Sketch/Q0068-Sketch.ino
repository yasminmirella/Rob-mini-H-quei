#include <RF24.h>

#include <Servo.h>
#include <TM1637Display.h>

//*************** Controle do RF ***********************
#define radioID 1   //Informar "0" para um dispositivo e "1" para o outro dispositivo

struct estruturaDadosRF
{
   boolean ligando = false;   //Esta variavel será usada para solicitar os dados do outro aparelho. Será útil quando o aparelho solicitante esta sendo ligado, para manter os valores do aparelho que já esta ligado.
   boolean botao1 = false;
   boolean botao2 = false;
   boolean botao3 = false;
   int potenciometro1 = 0;
   int potenciometro2 = 0;
};
typedef struct estruturaDadosRF tipoDadosRF;
tipoDadosRF dadosRF;
tipoDadosRF dadosRecebidos;

boolean transmitido = true;
boolean alterado = false;

RF24 radio(7,8);

byte enderecos[][6] = {"1node","2node"};



//*************** Controle do Projeto LOCAL ************
boolean botao1Ant = HIGH;
boolean botao1    = HIGH;
boolean botao2Ant = HIGH;
boolean botao2    = HIGH;
boolean botao3Ant = HIGH;
boolean botao3    = HIGH;
int pot1Ant = 0;
int pot1    = 0;
int pot2Ant = 0;
int pot2    = 0;

int angulo1 = 0;
int angulo2 = 0;
Servo servo1;
TM1637Display display1(9,6);


void setup() {
  //*************** Controle do RF ***********************
  radio.begin();
  
  #if radioID == 0
      radio.openWritingPipe(enderecos[0]);
      radio.openReadingPipe(1, enderecos[1]);
  #else
      radio.openWritingPipe(enderecos[1]);
      radio.openReadingPipe(1, enderecos[0]);
  #endif

  //Solicita os dados do outro aparelho, se ele estiver ligado. Tenta a comunicação por 2 segundos.
  dadosRF.ligando = true;
  radio.stopListening();                                   
  long tempoInicio = millis();
  while ( !radio.write( &dadosRF, sizeof(tipoDadosRF) ) ) {
     if ((millis() - tempoInicio) > 2000) {
        break;
     }   
  }
  dadosRF.ligando = false; 
  radio.startListening();  


  //*************** Controle do Projeto LOCAL ************
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP); 
  pinMode(4, INPUT_PULLUP); 
  servo1.attach(5);
  display1.setBrightness(0x0f);
}


void loop() {
  //*************** Controle do RF ***********************
  // se houve alteração dos dados, envia para o outro radio 
  if (alterado || !transmitido) {
     radio.stopListening();                                   
     transmitido = radio.write( &dadosRF, sizeof(tipoDadosRF) );
     radio.startListening();  
     alterado = false;
  }

  //verifica se esta recebendo mensagem       
  if (radio.available()) {                                   
     radio.read( &dadosRecebidos, sizeof(tipoDadosRF) ); 

     //verifica se houve solicitação de envio dos dados
     if (dadosRecebidos.ligando) {
        alterado = true;
     } else {
        dadosRF = dadosRecebidos;
     }
  }



  //*************** Controle do Projeto LOCAL ************
  #if radioID == 1  
    pot1 = analogRead(A4);
    if (pot1 != pot1Ant) {
       dadosRF.potenciometro1 = pot1;
       alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
    }
    pot1Ant = pot1;
  
    pot2 = analogRead(A5);
    if (pot2 != pot2Ant) {
       dadosRF.potenciometro2 = pot2;
       alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
    }
    pot2Ant = pot2;
  #endif
    
  botao1 = digitalRead(2);
  if (botao1 && (botao1 != botao1Ant)) {
     dadosRF.botao1 = !dadosRF.botao1;
     alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
  }
  botao1Ant = botao1;

  botao2 = digitalRead(3);
  if (botao2 && (botao2 != botao2Ant)) {
     dadosRF.botao2 = !dadosRF.botao2;
     alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
  }
  botao2Ant = botao2;

  botao3 = digitalRead(4);
  if (botao3 && (botao3 != botao3Ant)) {
     dadosRF.botao3 = !dadosRF.botao3;
     alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
  }
  botao3Ant = botao3;


  angulo1 = map(dadosRF.potenciometro1, 0, 1023, 0, 180);  
  servo1.write(angulo1);

  angulo2 = map(dadosRF.potenciometro2, 0, 1023, 0, 180);  
  display1.showNumberDec(angulo2);

  digitalWrite(A1, dadosRF.botao1);
  digitalWrite(A2, dadosRF.botao2);
  digitalWrite(A3, dadosRF.botao3);

  delay(10);
}
