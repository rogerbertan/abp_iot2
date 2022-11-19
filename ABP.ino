/**--------------------------------------------------------
  Hardware: WeMos D1 ESP8266 R1
  Framework: Arduino IDE 2.0.2
  Curso: Engenharia de Computação
  Disciplina: IoT II
  -------------------------------------------------------- */
/*  Relação entre pinos da WeMos D1 R1 e GPIOs do ESP8266
  Pinos-WeMos   Função          Pino-ESP-8266
    D0       RX                       GPIO3
    D1       TX                       GPIO1
    D2       IO                       GPIO16  
    D3(D15)  IO, SCL                  GPIO5
    D4(D14)  IO, SDA                  GPIO4
    D5(D13)  IO, SCK                  GPIO14
    D6(D12)  IO, MISO                 GPIO12   
    D7(D11)  IO, MOSI                 GPIO13    
    D8       IO, Pull-up              GPIO0    //Utilizado na gravação. Sug: Usar como INPUT.
    D9       IO, Pull-up, BUILTIN_LED GPIO2    //Utilizado na gravação. Sug: Usar como INPUT.  
    D10      IO, Pull-down, SS        GPIO15  
    A0       Analog Input             A0
*/

/* Inclusão de Bibliotecas */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

/* Definições e Constantes */
#define LED_R           D6
#define LED_G           D7
#define LED_B           D8

#define DHTPIN          D3
#define DHTTYPE         DHT11

/* Protótipos de Funções */


/* Variáveis Globais */
const char* ssid = "Antonio";
const char* password = "julia5050";
unsigned long lastTime = 0;      //Variável de auxílio para medição/sincronização do tempo.
unsigned long timerDelay = 5000; //Timer de 5 segundos entre requisições.

//Seu nome de domínio com caminho de URL ou endereço IP com caminho
String serverThingspeak = "http://api.thingspeak.com/update?api_key=EL177GJPZIAE9HMQ";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  delay(1000);
  Serial.print("\nConectando na rede ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi conectado. Endereço de IP: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);
  
  dht.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    float t = dht.readTemperature();
    if (isnan(t)) {
      Serial.println("Falha ao ler o sensor");
    } 
    else {
    Serial.print("\nTemperatura: ");
    Serial.print(t);
    Serial.println(" *C");
    }
    // Verifica o status da conexão
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      String serverPath = serverThingspeak + "&field1=" + t;
      
      http.begin(client, serverPath.c_str());
      
    // Realiza a requisição GET e armazena a resposta.
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
      }
      else {
        Serial.print("Código de Erro: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Desconectado");
    }
    lastTime = millis();
  }
}