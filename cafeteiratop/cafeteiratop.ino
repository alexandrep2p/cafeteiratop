//bilbiotecas do lcd e do rfid
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  

// Inicializa o display no endereco 0x3F
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

char st[20];
int tempoPreparo = 15;

void setup()
{
  Serial.begin(9600);  // Inicia a serial
  SPI.begin();    // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");//mostra a mensagem no painel serial
  Serial.println();
  lcd.begin (16,2);//seta o tamanho do lcd
  mensageminicial();//chama o metodo no final do codigo pra mostrar a mensagem inicial no lcd
}
 
void loop()
{
 // procura se ha tag ou cartao proximo do sensor
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // seleciona o cartao
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  conteudo.toUpperCase();//bota a ID em caixa alta
  
  if (conteudo.substring(1) == "50 52 1D 33") //verifica a ID 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" OLA ALEXANDRE");
    lcd.setCursor(0,1);
    lcd.print("VAMOS TOMAR CAFE");
    delay(3000);
    autorizado();
  }else{ //se a ID nao tiver cadastrada
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" NAO TE CONHECO ");
    lcd.setCursor(0,1);
    lcd.print("  SOME DAQUI !! ");
    delay(3000);
    mensageminicial();
  }
  
}

void mensageminicial()
{
  //mensagem inicial do quero cafeee !!!
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("APROXIME SUA TAG");
  lcd.setCursor(0,1);
  lcd.print("QUERO CAFEEE !!! "); 
}

void autorizado()
{
  //mensagem na liberacao do rele
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PREPARANDO TUDO");
  lcd.setCursor(0,1);
  lcd.print("AGUARDE...");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CERTIFIQUE-SE");
  lcd.setCursor(0,1);
  lcd.print("QUE TEM AGUA E");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("QUE TEM AGUA E");
  lcd.setCursor(0,1);
  lcd.print("PO NA CAFETEIRA");
  delay(5000);//ESPERA 5 SEGUNDOS ANTES DE ENTRAR NA CONTAGEM REGRESSIVA
  //
  do{
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("TEMPO RESTANTE");
    lcd.setCursor(3,1);
    lcd.print(tempoPreparo);
    lcd.setCursor(6,1);
    lcd.print("MINUTOS");
    tempoPreparo--;
    delay(60000); // delay de 1 min
  }while(tempoPreparo>0);
  tempoPreparo = 15;//volta o tempo para 15min
  
  //fecha o rele e volta para a mensagem inicial
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  TERMINAMOS!  ");
  lcd.setCursor(0,1);
  lcd.print(" ATE A PROXIMA ");
  delay(2000);
  lcd.clear();
  mensageminicial();  
}
