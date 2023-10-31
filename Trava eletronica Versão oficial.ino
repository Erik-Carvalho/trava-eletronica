#include <SPI.h>                            
#include <MFRC522.h>   
#include <LiquidCrystal_I2C.h>
int lock = 2;
int buzzer= 6;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
                      

#define endereco  0x3F
#define colunas   16
#define linhas    2
#define   SS_PIN    10                       //pino SDA
#define   RST_PIN    9                       //pino RST 
//#define   buzzer     6                      
//#define   lock       5                       //trava eletrônica


MFRC522 mfrc522(SS_PIN, RST_PIN);       



void rfid_func();                            //Função para identificar as tags 
 



void setup() 
{
  lcd.init();
  lcd.backlight();
  lcd.print(" Aproxime a TAG ");
  pinMode(6,    OUTPUT);                
  pinMode(lock,      OUTPUT);
                  
 
  //digitalWrite(buzzer,    LOW);              
  digitalWrite(lock,      LOW);              
  
  Serial.begin(9600);                        
  SPI.begin();                               
  mfrc522.PCD_Init();   //inicia o RFID

 
  Serial.println("Aproxime as tags do leitor...");
  Serial.println();

}



void loop() 
{
rfid_func();                             

}


void rfid_func()    //Função para identificar as tags 
{
      //Verifica novas tags 
      if(!mfrc522.PICC_IsNewCardPresent()) return;
    
      //Seleciona uma das tags
      if(!mfrc522.PICC_ReadCardSerial())   return;
     
      // Mostra o ID da tag na serial
      Serial.print("UID da tag :");
      String conteudo= "";
      byte letra;
      for(byte i = 0; i < mfrc522.uid.size; i++)
      {
         Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
         Serial.print(mfrc522.uid.uidByte[i], HEX);
         conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
         conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();
      Serial.print("Mensagem : ");
      conteudo.toUpperCase();


      //tags para liberar acesso
      if (conteudo.substring(1) == "AE 73 69 67" |
          conteudo.substring(1) == "" |
          conteudo.substring(1) == ""  ) 
      {
        Serial.println("Acesso liberado!");
        Serial.println();
        lcd.clear();
        lcd.print(" Acesso Liberado  ");
        tone(6,440,150);
        digitalWrite(lock,   HIGH);
        delay(2555);
        digitalWrite(lock,    LOW);
        lcd.clear();
        lcd.print(" Aproxime a TAG  ");
      }
  
      else 
      {
        Serial.println("Acesso Negado");
        Serial.println();
        lcd.clear();
        lcd.print(" Acesso Negado  ");
        tone(6,392,500);
        delay(2555);
        lcd.clear();
        lcd.print(" Aproxime a TAG  ");
      } 
}
