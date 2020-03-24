#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>

//Definição dos Pinos
#define pinCE  9 // aqui são o pinos do radio
#define pinCSN 8
#define pinIN1 4 // aqui são os pinos do motor da roda direita
#define pinIN2 3
#define pinIN3 6 //esses são os pinos do motor da roda esquerda
#define pinIN4 5

// aqui a gente usa a mesma estrutura do controle, para os potenciometros
struct tipoDadosRF
{
   int pot1 = 512;
   int pot2 = 512;
   boolean botao = false;
};
tipoDadosRF dadosRF;

RF24 radio(pinCE,pinCSN);  //inicia o pinos do radio

//const byte adress[6] = "00001"; //canal transmissor do radio

const uint64_t pipeOut = 0xE14BC8F48LLg; // esse é o canal de conicação dos radios


//Controle do Carro
int pDireita  = 100;
int pEsquerda = 100;


void setup() {
  //Controle do RF
  // aqui é a mesma coisa do joystick
  Serial.begin(9600);
  radio.begin(); //inicia o radio
  radio.setPALevel( RF24_PA_LOW );     //distancia de envio
  radio.setDataRate( RF24_250KBPS );   //velocidade de envio
  radio.setChannel(125);
  radio.openReadingPipe(0, pipeOut); // seta o transmissor
  radio.startListening();   // "escuta" o comando do radio


  //Controle do Carro
  // aqui a gente seta os pinos como saida da ponte H
  pinMode(pinIN1, OUTPUT); 
  pinMode(pinIN2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  Serial.begin(9600); 
  
}


void loop() {
  //Controle do RF
  if (radio.available()) {    // aqui a gente verifica se o carrinho ta recebendo o sinal do controle                               
     radio.read( &dadosRF, sizeof(tipoDadosRF) ); // aqui ele le o sinal que o controle ta mandando
     Serial.print("comunicação estabelecida");
  }     
  

  //Controle do Carro
  
  if (dadosRF.botao) {

     //Aciona o freio caso o botao for apertado
     digitalWrite(pinIN1, HIGH); // pinIN1 e pinIN2 sao do motor direito
     digitalWrite(pinIN2, HIGH);
     digitalWrite(pinIN3, HIGH); //pinIN3 e pinIN4 sao do motor esquerdo
     digitalWrite(pinIN4, HIGH);
       
  } else {
     //Controle da direção 
     if (dadosRF.pot2 < 512) {
        //Esquerda 
        pDireita  = 100;
        pEsquerda = map(dadosRF.pot2, 511, 0, 100, 0);  // mapeia a faixa do potenciometro para velocidade 

        Serial.println("Esquerda");
        delay(200);
        
     } else {
        //Direita
        pDireita  = map(dadosRF.pot2, 512, 1023, 100, 0);
        pEsquerda = 100;

        Serial.println("Direita");
        delay(200);
        
     }
      
     if (dadosRF.pot1 < 512) {
        //Reverso
        int velocidade = map(dadosRF.pot1, 511, 0, 0, 255);
        analogWrite(pinIN1, 0);
        analogWrite(pinIN2, velocidade * pDireita / 100); 
        
        analogWrite(pinIN3, 0);
        analogWrite(pinIN4, velocidade * pEsquerda / 100);

        Serial.println("Reverso");
        delay(200);
         
        } else {
          //Para frente
          int velocidade = map(dadosRF.pot1, 512, 1023, 0, 255);

        analogWrite(pinIN1, velocidade * pDireita / 100);
        analogWrite(pinIN2, 0);
        
        analogWrite(pinIN3, velocidade * pEsquerda / 100);
        analogWrite(pinIN4, 0);                         

        Serial.println("Frente");
        delay(200);
        
     } 
 
     
     delay(100);
     
  }
}
