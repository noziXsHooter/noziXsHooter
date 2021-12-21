#define BLYNK_TEMPLATE_ID "**********"
#define BLYNK_DEVICE_NAME "**********"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG


//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <DHT.h>



#define DHTPIN D4  // PINO DO DHT
#define DHTTYPE DHT22 // TIPO DO DHT
//#define 

int gas = A0;
int pin8 = D0;
int valorsensor = 0;
int buzzer = D5;
int ledligado = D1;
int ledalarme = D8;
int lampada;
int estadolampada;
int pinestadoesp;

DHT dht (DHTPIN, DHTTYPE); //SETA NA LIB


// VARIAVEIS DO  APP



BLYNK_WRITE(V8)
{
  
  int pinestadoalarme = A0;
  Blynk.virtualWrite(V8, pinestadoalarme);
  
}
BLYNK_WRITE(V1)
{
  int pinestadoesp = D1;
  //digitalWrite(D1,pinestadoesp);
  //Blynk.virtualWrite(V1, HIGH);
}
BLYNK_WRITE(V7)
{
  int valortemp = dht.readTemperature();
  Blynk.virtualWrite(V7, HIGH);
}
BLYNK_WRITE(V6)
{
  int valorhmd = dht.readHumidity();
  Blynk.virtualWrite(V6, HIGH);
}


void setup()
{
  //SENSOR
   
  pinMode(pin8, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledligado, OUTPUT);
  pinMode(ledalarme, OUTPUT);
  pinMode(lampada, OUTPUT);
  //digitalWrite(D5, LOW);
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();                     //INICIA BLYNK
  dht.begin();                             // INICIA DHT
}

void loop() {

                                                        //    DHT LOOP

    int t = dht.readTemperature();
    int h = dht.readHumidity();

   Serial.println(("Temperatura: ") + String(t) + (" Celsius"));
   Serial.println(("Humidade: ") + String(h)+ ("%"));

   Blynk.virtualWrite(V7, t);
   Blynk.virtualWrite(V6, h);



                                                  // SENSOR GAS , LEDS E BUZZER

 
            if (WiFi.status() == WL_CONNECTED){     // SÓ LIGA O LED SE A CONEXAO DO WIFI FOI BEM SUCEDIDA
                analogWrite(ledligado, HIGH);
                Blynk.virtualWrite(V1, HIGH);

  
            }else {
                analogWrite(ledligado, LOW);
                Blynk.virtualWrite(V1, LOW);
                
}


delay(1000);


                valorsensor = analogRead(gas);
                String nivelgas = "                NIVEL DE GAS CARBONICO:   "; 
                Serial.println(nivelgas + valorsensor);  

 
          if (valorsensor<380 && valorsensor>30)
          {

               digitalWrite(buzzer, LOW);
               digitalWrite(ledalarme, LOW);
               digitalWrite(pin8, LOW);
               Serial.println("                                                    SEGURO!");
               Blynk.virtualWrite(V8, LOW);
               delay(1000);
                
          }
          if (valorsensor<30) 
          {
    
                Blynk.virtualWrite(V1, LOW);  

                
          } 
          if (valorsensor>380 || int(t)>40&&(t)<150)         //CORREÇAO ERRO VARIAVEL "t" e "h" = 2147483647 //  NAO ACIONA O ALERTA POR ALTA TEMP FALSO POSITIVO 
          {
          
                digitalWrite(pin8, HIGH);
                digitalWrite(ledalarme, HIGH);
                Serial.println("                                                    PERIGO DETECTADO!");
                digitalWrite(buzzer, HIGH);
                Blynk.virtualWrite(V8, HIGH);
                Blynk.logEvent("alert_message");            //NOTIFICAÇAO NO APP
                delay(10000);
           }
delay(10000);


  BlynkEdgent.run();
}
