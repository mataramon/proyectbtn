
#include "WiFi.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// #define LED_BUILTIN 2
#define PUSH_BUTON 15

const char* ssid = "HUAWEINova3";
const char* password = "Abcde12345";

// Documento JSON 
StaticJsonDocument<250> doc;

HTTPClient myHTTP;

// Dominio Servidor
//const char* serverName = "https://dev19722.service-now.com/api/now/table/x_24052_panico_boton2";
const char* serverName = "https://dev19722.service-now.com/api/now/table/u_alert";

void setup() {

  //int WiFiNetCount = 0;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSH_BUTON, INPUT);


  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

 /*
  Serial.println("Escaneando redes inalambricas...");
  WiFiNetCount = WiFi.scanNetworks();
  Serial.println("");

  if (WiFiNetCount == 0) {
    Serial.println("ERROR: No hay redes inalambricas disponibles.");
  }
  else {
    Serial.print(WiFiNetCount);
    Serial.println(" redes inalambricas encontradas:");
    for (int i = 0; i < WiFiNetCount; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        delay(10);
    }
  }
  */
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  WiFi.setSleep(false);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("\nConfiguracion terminada...");
}

void loop() {
  int PushButtonState = digitalRead(PUSH_BUTON);
  
  if(PushButtonState == HIGH) {
    Serial.println("¡Boton presionado!");
    digitalWrite(LED_BUILTIN, HIGH);
    
    myHTTP.begin(serverName);

    myHTTP.addHeader("Accept", "application/json");
    myHTTP.addHeader("Content-Type", "application/json");
    myHTTP.setAuthorization("ramon","1234");
    //int httpResponseCode = myHTTP.POST("{\"u_nombre\":\"AlmacenEx1\",\"u_contacto\":\"Ramon Mata\",\"u_telefono_1\":\"3314106787\",\"u_telefono_2\":\"3338101922\",\"u_observacion\":\"¿Alarma!\"}");
    int  httpResponseCode = myHTTP.POST("{\"u_button\":\"b09022952f10d0d4a352bab62799b67e\"}");

    if(httpResponseCode > 0) {
    
      String response = myHTTP.getString();  //Get the response to the request
  
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);
    }
    else {
      Serial.println("ERROR: POST no enviado");
      Serial.println(httpResponseCode);
    }

  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
