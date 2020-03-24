 #include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>

//Definição dos Pinos
#define pinCE    7    // pinos para iniciar o radio
#define pinCSN   8    
#define pinPot1  A1  //Frente e Reverso
#define pinPot2  A2  //Direito e Esquerdo
#define pinBotao A3 // caso use o botão

//usa uma estruturas para poder usar os potenciometros do joystick
struct tipoDadosRF
{
   int pot1 = 512;
   int pot2 = 512;
   boolean botao = false;
};
tipoDadosRF dadosRF;

RF24 radio(pinCE,pinCSN); // aqui a gente "diz" quais são os pinos do radio

const uint64_t pipeOut = 0xE14BC8F48LL; // esse é o canal de conicação dos radios

//const byte adress[6] = "00001"; // canal trasnmissor do radio

//Controle do Joystick 
boolean botaoAnt = HIGH;
boolean botao    = HIGH;


void setup() {
  //Controle do RF
  Serial.begin(9600);
  radio.begin(); // aqui inicia o radio
  radio.setPALevel( RF24_PA_LOW );     //distancia de envio
  radio.setDataRate( RF24_250KBPS );   //velociadade de envio
  radio.openWritingPipe(pipeOut);  // envia o camando do radio

  //Controle do Joystick
  pinMode(pinBotao, INPUT_PULLUP); //seta o botao como saida 

  delay(100);
}


void loop() {
  //Controle do RF
  radio.write( &dadosRF, sizeof(tipoDadosRF) ); // envia o comando para o carrinho


  //Controle do Joystick
  dadosRF.pot1 = analogRead(pinPot1); // armazena a dados do joystick na estrutura dos dados
  dadosRF.pot2 = analogRead(pinPot2);
    
  botao = digitalRead(pinBotao);
  if (botao && (botao != botaoAnt)) {
     dadosRF.botao = !dadosRF.botao;
  }
  botaoAnt = botao;
  
  Serial.println("Y: ");
  Serial.print(dadosRF.pot1);
  Serial.print("  X: ");
  Serial.println(dadosRF.pot2);
  Serial.println("Botao: ");
  Serial.println(dadosRF.botao);
  
  delay(200);

}
