#ifndef RESTSERVER_H
#define RESTSERVER_H


#include <stdio.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "resources\PhysicsResource.h"

class RestServer{

public:
  const int HTTP_REST_PORT = 80;
  const int WIFI_RETRY_DELAY = 500;
  const int MAX_WIFI_INIT_RETRY = 50;
  const char* wifi_ssid = "YOUR_SSID";
  const char* wifi_passwd = "YOUR_PASSWD";
  const ESP8266WebServer *http_rest_server= new ESP8266WebServer(HTTP_REST_PORT);
private:
  PhysicsResource* physicsResourcePtr;
  int init_wifi() {
    int retries = 0;
    Serial.println("Connecting to WiFi AP..........");
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_passwd);
    // check the status of WiFi connection to be WL_CONNECTED
    while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY)) {
      retries++;
      delay(WIFI_RETRY_DELAY);
      Serial.print("#");
    }
    return WiFi.status(); // return the WiFi connection status
  }

  void greet(){
    http_rest_server->send(200, "text/html","Welcome to the ESP8266 REST Web Server");
  }
  void config_rest_server_routing() {
    *http_rest_server->on("/", HTTP_GET, greet);
  }

public:

  RestServer() {
    Serial.begin(115200);

    if (init_wifi() == WL_CONNECTED) {
      Serial.print("Connetted to ");
      Serial.print(wifi_ssid);
      Serial.print("--- IP: ");
      Serial.println(WiFi.localIP());
    }
    else {
      Serial.print("Error connecting to: ");
      Serial.println(wifi_ssid);
    }
    config_rest_server_routing();
    http_rest_server->begin();
    Serial.println("HTTP REST Server Started");
  }

  void handleClient(){
    http_rest_server->handleClient();
  }

  const ESP8266WebServer* getServer(){
    return http_rest_server;
  }

};
#endif
