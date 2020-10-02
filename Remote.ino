/*
  Remote.ino - Handle RF 433 remote device
  Saul bertuccio 5 feb 2017
  Released into the public domain.
*/

#include "Arduino.h"
#include "SoftwareSerial.h"
ADC_MODE(ADC_VCC);
//SoftwareSerial sw;

#include "DeviceHandler.h"
#include "ConnectionManager.h"
#include "Portal.h"

Portal portal;

String ssid, pass;

void setup() {
  
  Serial.begin(115200);
  delay(2000);
  
  Serial.print("ESP8266 chip id: ");
  Serial.println(ESP.getChipId());
  Serial.print("Vitesse du flash ESP8266: ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("Tension d'alimentation: ");
  Serial.println(ESP.getVcc()); 
  Serial.print(" Raison de la dernière réinitialisation: " );
  Serial.println(ESP.getResetReason());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());


  Serial.println("Configurer la connexion wifi");
  ConnectionManager::initConnection();
  Serial.println("Demarrer le web server");
  while (!portal.setup()) {
      Serial.println("Impossible de configurer le serveur HTTP ");
      delay(10000);
  }
}

void loop() {
  portal.handleRequest();

  if (portal.needRestart()) {
    Serial.println(" Redémarrer le périphérique ");
    delay(5000);
    ESP.restart();
  }
}
