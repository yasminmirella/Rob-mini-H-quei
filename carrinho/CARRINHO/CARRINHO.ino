//Programa : Modulo Arduino Bluetooth HC-05 - Recepcao
//Autor : FILIPEFLOP
 
//Armazena o caracter recebido
char buf;
 
void setup()
{
  //Define o pino 13 como saida
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
 
void loop()
{
  while(Serial.available() > 0)
  {
    buf = Serial.read();
    //Caso seja recebido o caracter L, acende o led
    if (buf == 'L')
    {
      digitalWrite(13, HIGH);
    }
    //Caso seja recebido o caracter D, apaga o led
     if (buf == 'D')
    {
      digitalWrite(13, LOW);
    }
  }
}
