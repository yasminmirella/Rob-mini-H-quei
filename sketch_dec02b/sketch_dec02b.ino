//Programa : Modulo Arduino Bluetooth HC-05 - Programacao
//Autor : FILIPEFLOP
 
//Carrega a biblioteca SoftwareSerial
#include <SoftwareSerial.h>
    
//Define os pinos para a serial   
SoftwareSerial mySerial(10, 11); // RX, TX  
String command = ""; // Stores response of bluetooth device  
            // which simply allows n between each  
            // response.  
    
void setup()   
{  
  //Inicia a serial  
  Serial.begin(115200);  
  Serial.println("Digite os comandos AT :");  
  //Inicia a serial configurada nas portas 10 e 11
  mySerial.begin(38400);  
}  
    
void loop()  
{  
  // Read device output if available.  
  if (mySerial.available()) 
  {  
     while(mySerial.available()) 
     { // While there is more to be read, keep reading.  
       command += (char)mySerial.read();  
     }  
   Serial.println(command);  
   command = ""; // No repeats  
  }  
   
  // Read user input if available.  
  if (Serial.available())
  {  
    delay(10); // The DELAY!  
    mySerial.write(Serial.read());  
  }  
}
