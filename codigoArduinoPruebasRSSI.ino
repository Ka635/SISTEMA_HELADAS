#include <SPI.h>
#include <LoRa.h>

// Pines del módulo LoRa
#define PIN_nss 10
#define PIN_rst 9
#define PIN_dio0 2

// Configuración LoRa
const int sf = 7;          // Spreading Factor (7-12)
const long bw = 125E3;     // Bandwidth (Hz)
const int cr = 4;          // Coding Rate
const int expectedNode = 1; // Dirección esperada del nodo transmisor

void setup() {
  // Inicialización del monitor serial
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Nodo Gateway - Iniciando");

  // Configuración de pines del módulo LoRa
  LoRa.setPins(PIN_nss, PIN_rst, PIN_dio0);

  // Inicialización del módulo LoRa
  if (!LoRa.begin(915E6)) {
    Serial.println("Error: Falló la conexión con el módulo LoRa. Reiniciando...");
    while (1);
  }

  // Configuración de parámetros LoRa
  LoRa.setSpreadingFactor(sf);
  LoRa.setSignalBandwidth(bw);
  LoRa.setCodingRate4(cr);
  LoRa.enableCrc();
  LoRa.setSyncWord(0xF3);

  Serial.println("Módulo LoRa inicializado correctamente");
  Serial.print("Spreading Factor (SF): ");
  Serial.println(sf);
  Serial.print("Bandwidth (BW): ");
  Serial.println(bw);
  Serial.print("Coding Rate (CR): 5/");
  Serial.println(cr);

}

void loop() {
  // Comprobar si hay un paquete disponible
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // Leer la dirección del nodo transmisor
    int senderNode = LoRa.read();
    if (senderNode != expectedNode) {
      Serial.println("Paquete recibido de un nodo no autorizado.");
      return;
    }

    // Leer el resto del paquete
    String data = "";
    while (LoRa.available()) {
      data += (char)LoRa.read();
    }

    // Imprimir el paquete recibido
    Serial.print("Datos recibidos: ");
    Serial.println(data);

    // Procesar los datos
    procesarDatos(data);
  }
}

void procesarDatos(String data) {
  // Variables para almacenar los datos procesados
  float temperatura = 0.0;
  float humedad = 0.0;
  float velocidadKmh = 0.0;
  float indiceUV = 0.0;

  // Buscar y extraer cada valor del mensaje recibido
  int tempIndex = data.indexOf("T:");
  int humIndex = data.indexOf("H:");
  int windIndex = data.indexOf("W:");
  int uvIndex = data.indexOf("UV:");

  if (tempIndex == -1 || humIndex == -1 || windIndex == -1 || uvIndex == -1) {
    Serial.println("Error: Formato de datos no válido.");
    return;
  }

  // Extraer los valores usando substring
  temperatura = data.substring(tempIndex + 2, humIndex).toFloat();
  humedad = data.substring(humIndex + 2, windIndex).toFloat();
  velocidadKmh = data.substring(windIndex + 2, uvIndex).toFloat();
  indiceUV = data.substring(uvIndex + 3).toFloat();

  Serial.println(LoRa.packetRssi());

  // Imprimir los datos procesados
  Serial.println("Datos Procesados:");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  Serial.print("Velocidad del viento: ");
  Serial.print(velocidadKmh);
  Serial.println(" km/h");

  Serial.print("Índice UV: ");
  Serial.println(indiceUV);
}
