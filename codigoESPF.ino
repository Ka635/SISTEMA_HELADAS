#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>
#include <DHT_U.h>

//*********************************************LoRA****************************************************
#define PIN_nss 5
#define PIN_rst 14
#define PIN_dio0 2

// Configuraciones LoRa
const int sf = 7;            // Spreading Factor (7-12)
const int bw = 125000;       // Bandwidth (Hz)
const int cr = 4;            // Coding Rate
const int nodeAddress = 1;   // Dirección del nodo LoRa

//*******************************************DHT11*****************************************************
#define PIN_DHT 4           // Pin conectado al sensor DHT11
#define tipo_DHT DHT11       // Tipo de sensor
DHT dht(PIN_DHT, tipo_DHT);  // Instancia del sensor DHT

//***************************************GUVA S12SD****************************************************
#define PIN_sensorUV 35       // Pin conectado al sensor UV

//***************************************ANEMÓMETRO****************************************************
#define PIN_anemometro 34    // Pin conectado al anemómetro

//***************************************SETUP*********************************************************

void setup() {
  // Inicialización del monitor serial
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Nodo Sensor - Iniciando");

  // Inicialización del sensor DHT11
  dht.begin();

  // Configuración de pines del módulo LoRa
  LoRa.setPins(PIN_nss, PIN_rst, PIN_dio0);

  // Inicialización del módulo LoRa
  if (!LoRa.begin(915E6)) {
    Serial.println("Error: Falló la conexión con el módulo LoRa. Reiniciando...");
    delay(5000);
    ESP.restart();
  }

  // Configuración de parámetros LoRa
  LoRa.setSpreadingFactor(sf); // Factor de dispersión (7-12)
  LoRa.setSignalBandwidth(bw); // Ancho de banda (125-250-500 kHz)
  LoRa.setCodingRate4(cr);     // Coding Rate
  LoRa.enableCrc();            // Habilita la comprobación de redundancia cíclica (CRC)
  LoRa.setSyncWord(0xF3);      // Sincroniza con los elementos de la red

  // Impresión de parámetros configurados DE LoRa
  Serial.println("Módulo LoRa inicializado correctamente");
  Serial.print("Spreading Factor (SF): ");
  Serial.println(sf);
  Serial.print("Bandwidth (BW): ");
  Serial.println(bw);
  Serial.print("Coding Rate (CR): 5/");
  Serial.println(cr);
}

//***************************************LOOP**********************************************************
void loop() {
  
  
  //Datos Prueba
  float humedad = 55;
  float temperatura = 19;
  float velocidadKmh = 20;
  float indiceUV = 4;

  // Leer datos de los sensores
  /*float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Comprobación de datos del sensor de Humedad y Temperatura
  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("Error al leer del sensor DHT!");
    return;
  }

  // Lectura y calibración del anemómetro
  float ValorAnalogicoA = analogRead(PIN_anemometro);
  float voltaje_anemometro = (ValorAnalogicoA * 3.3) / 4095;
  float velocidadKmh = (voltaje_anemometro * 25) * 3.6;

  // Lectura y calibración del sensor UV
  int valorAnalogicoUV = analogRead(PIN_sensorUV);
  float voltajeUV = (valorAnalogicoUV * 3.3) / 4095;
  float indiceUV = voltajeUV / 0.1;*/

  // Preparar los datos en formato char[]
  char data[128];
  snprintf(data, sizeof(data), "T:%.2f H:%.2f W:%.2f UV:%.2f", temperatura, humedad, velocidadKmh, indiceUV);

  // Imprimir datos en el monitor serial
  Serial.println("Datos a enviar:");
  Serial.println(data);

  // Enviar datos mediante LoRa
  LoRa.beginPacket();         // Iniciar el paquete
  LoRa.write(nodeAddress);    // Dirección del nodo
  LoRa.print(data);           // Agregar datos
  LoRa.endPacket();           // Finalizar el paquete

  Serial.println("Datos enviados con éxito.");
  delay(30000); // Enviar datos cada 10 segundos
}

