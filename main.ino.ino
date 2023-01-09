#define RFID Serial2
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <WiFiManager.h>
//#include "freertos/FreeRTOS.h"

//set wifi

//mulai telegram

#define BOT_TOKEN ""
#define CHAT_ID "-1001876624474"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
WiFiClientSecure secured_client;

UniversalTelegramBot bot(BOT_TOKEN, secured_client);

String Teks;

void setup()
{
  Serial.begin(9600);
  WiFiManager wm;

  RFID.begin(9600);
 
  pinMode (26, OUTPUT);
  digitalWrite (26, 1);
  Serial.println("Dekatkan kartu RFID anda …");
  Serial.println();
  bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("PRC","12345678"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        wm.resetSettings();
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        configTime(25200, 0, "pool.ntp.org"); // get UTC time via NTP
        time_t now = time(nullptr);
         while (now < 24 * 3600)
          {
               Serial.print(".");
               delay(100);
               now = time(nullptr);
          }
                Serial.println(now);
    }
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  
 
}
boolean state = false;
char c;
void loop()
{
  while (RFID.available() > 0) {
    delay(5);
    c = RFID.read();
    Teks += c;
  }
  if (Teks.length() > 20) Cek();
  Teks = "";
}
void Cek()
{
  time_t mytime = time(NULL);
  char * time_str = ctime(&mytime);
  time_str[strlen(time_str)-1] = '\0';
  printf("Current Time : %s\n", time_str);
  
  Teks = Teks.substring(1, 11);
  const char* jam = time_str;
  String jams = jam;
  String buffer;
  buffer = "ID Kunam : " + Teks + "\n Datang pada : " + jams;
  
 
  Serial.println("ID Burung: " + Teks);
  //printf(buffer);
  bot.sendMessage(CHAT_ID,buffer, "");
  delay(10);
  Serial.println(" ");
  Serial.println("Dekatkan kartu RFID anda …");
 
}
