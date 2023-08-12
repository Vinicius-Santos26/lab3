#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO

/////Json
#include <ArduinoJson.h>
const int TAMANHO = 16; // define o tamanho do buffer para o json
const int echoPin = 6;  // PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin = 5;

///// Sensor DTH
#include "DHT.h"
#define DHTPIN 7 // define o pino usado no arduino
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);                // declara a objeto da classe
Ultrasonic ultrasonic(trigPin, echoPin); // INICIALIZANDO OS PINOS DO ARDUINO
int distancia;                           // VARIÁVEL DO TIPO INTEIRO
String result;                           // VARIÁVEL DO TIPO STRING

////// Outras declarações
#define led 13 // define led conectado no pino 13

void setup()
{
  // inicialia c sensor
  dht.begin();

  // inicializa comunicação serial
  Serial.begin(9600);
  pinMode(echoPin, INPUT); // DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT);
  // configura pinos de saida do arduinos
  pinMode(led, OUTPUT);
}

void loop()
{
  StaticJsonDocument<TAMANHO> json; // Aloca buffer para objeto json

  // Faz a leitura da temperatura
  float temp = dht.readTemperature();
  // faz a leitura da humidade
  float umi = dht.readHumidity();
  // faz leitura distancia
  hcsr04();

  // formato de escrita do json
  json["temperatura"] = temp;
  json["umidade"] = umi;
  json["distancia"] = distancia;

  serializeJson(json, Serial);

  if (Serial.available() > 0)
  {
    deserializeJson(json, Serial);
    if (json.containsKey("alerta"))
    {
      
      digitalWrite(led, HIGH);
      delay(5000);
      digitalWrite(led, LOW);    
    }
  }

  Serial.println();

  // delay
  delay(500);
}

void hcsr04()
{
  digitalWrite(trigPin, LOW);  // SETA O PINO 6 COM UM PULSO BAIXO "LOW"
  delayMicroseconds(2);        // INTERVALO DE 2 MICROSSEGUNDOS
  digitalWrite(trigPin, HIGH); // SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10);       // INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(trigPin, LOW);  // SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE

  // FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  // RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  // NA VARIAVEL "distancia"
  distancia = (ultrasonic.Ranging(CM)); // VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  result = String(distancia);           // VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
  delay(500);                           // INTERVALO DE 500 MILISSEGUNDOS
}
