/*
  many, many examples were combined to do this

  https://circuits4you.com/2018/11/21/esp32-access-point-station-ap-html-web-server-arduino/
   was particularily informative
 Thanks to Rui Santos and his crew for all their work as well.

*/


const int Flight = 18;

int PWM_Speed = 250; // initial speed PWM duty cycle   

// wired connections            

#define MOTORRight_PWM 26 //  Speed
#define MOTORRight_DIR 25 //  Direction
#define MOTORLeft_PWM 32 //  Speed
#define MOTORLeft_DIR 33 //  Direction

#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0

const int freq = 5000;
const int ledChannelLeft_PWM = 10;
const int ledChannelLeft_DIR = 11;
const int ledChannelRight_PWM = 12;
const int ledChannelRight_DIR = 13;
const int resolution = 8;

#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

const char* ssid = "Your Place's";               // different name and password?
const char* password = "********";                    // password length minimum 8 char

WebServer server(80);

void setup(void) {


  Serial.begin(115200);

  pinMode(Flight, OUTPUT);

  ledcSetup(ledChannelLeft_PWM, freq, resolution);
  ledcSetup(ledChannelLeft_DIR, freq, resolution);
  ledcSetup(ledChannelRight_PWM, freq, resolution);
  ledcSetup(ledChannelRight_DIR, freq, resolution);
  
  pinMode( MOTORLeft_DIR, OUTPUT );
  pinMode( MOTORLeft_PWM, OUTPUT );
  pinMode( MOTORRight_DIR, OUTPUT );
  pinMode( MOTORRight_PWM, OUTPUT );
  
  ledcAttachPin(MOTORLeft_PWM, ledChannelLeft_PWM);
  ledcAttachPin(MOTORLeft_DIR, ledChannelLeft_DIR);
  ledcAttachPin(MOTORRight_PWM, ledChannelRight_PWM);
  ledcAttachPin(MOTORRight_DIR, ledChannelRight_DIR);
  
  ledcWrite( ledChannelLeft_DIR, dir_Forward );
  ledcWrite( ledChannelLeft_PWM, PWM_STOP );
  ledcWrite( ledChannelRight_DIR, dir_Forward );
  ledcWrite( ledChannelRight_PWM, PWM_STOP );
  
  digitalWrite(Flight, LOW); //Flashlight will turn off

  Serial.println();
  Serial.println("Booting Sketch...");

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // WiFi.setTxPower(WIFI_POWER_8_5dBm);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();


 
  server.on("/", handle_OnConnect);                      //  commands are sent from html page index.h
 // server.on("/A", handle_Go_Lefter);                  //  what they do is described below
  server.on("/B", handle_Forward);
 // server.on("/C", handle_Go_Righter);
  server.on("/D", handle_Left);
  server.on("/E", handle_Right);
//  server.on("/F", handle_SW);
  server.on("/G", handle_Back);
//  server.on("/H", handle_SE);
  server.on("/I", handle_Spin_Left);
  server.on("/J", handle_Spin_Right);
  server.on("/off", handle_Stop);
  server.on("/FO", handle_Flashlight_Off);
//  server.on("/Flon", handle_Flashlight_On);
  server.on("/Pos", handle_Speed_Slider_Input);
  server.onNotFound(handle_NotFound);

  server.begin();                  //Start server
//  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();
}


void handle_NotFound () {
}


void handle_OnConnect() {
  String s = MAIN_page; //Read HTML contents
//  Serial.println("connected:");
  server.send(200, "text/html", s); //Send web page

}

void handle_Speed_Slider_Input() {
if(server.arg("value") ){
  PWM_Speed = server.arg("value").toInt();   //
  Serial.print("PWM_Speed = ");
  Serial.println(PWM_Speed);
  }

server.send(200, "text/plain", "ok");
}

void handle_Forward() {
  digitalWrite(Flight, HIGH); // Flash light will turn on
 
  ledcWrite( ledChannelLeft_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLeft_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRight_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRight_PWM, PWM_Speed ); // PWM speed = fast
  Serial.println("forward");
  server.send(200, "text/plain", "ok");
}


void handle_Back() {
  digitalWrite(Flight, LOW);
 
  ledcWrite( ledChannelLeft_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLeft_PWM, 255 - PWM_Speed ); // PWM speed = Slow  ledChannel_PWM, 180 - PWM_Speed
  ledcWrite( ledChannelRight_DIR, dir_Back  ); 
  ledcWrite( ledChannelRight_PWM, 255 - PWM_Speed ); 
  Serial.println("back");
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
 
  digitalWrite(Flight, LOW);
  ledcWrite( ledChannelLeft_DIR, dir_Forward);
  ledcWrite( ledChannelLeft_PWM, PWM_STOP );
  ledcWrite( ledChannelRight_DIR, dir_Forward ); 
  ledcWrite( ledChannelRight_PWM, PWM_STOP );  
  Serial.println("STOP");
  server.send(200, "text/plain", "ok");
}

void handle_Right() {
 
  ledcWrite( ledChannelLeft_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelLeft_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRight_DIR, dir_Back ); 
  ledcWrite( ledChannelRight_PWM, 255 - PWM_Speed); 
  Serial.println("Right");
  server.send(200, "text/plain", "ok");
}

void handle_Left() {
  ledcWrite( ledChannelLeft_DIR, dir_Back ); // direction = forward or back
  ledcWrite( ledChannelLeft_PWM, 255 - PWM_Speed); // PWM speed = fast or slow
  ledcWrite( ledChannelRight_DIR, dir_Forward ); 
  ledcWrite( ledChannelRight_PWM, PWM_Speed ); 
  Serial.println("Left");
  server.send(200, "text/plain", "ok");
}
void handle_Spin_Left() {
  ledcWrite( ledChannelRight_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelRight_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelLeft_DIR, dir_Back ); 
  ledcWrite( ledChannelLeft_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}
void handle_Spin_Right() {
  ledcWrite( ledChannelLeft_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelLeft_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRight_DIR, dir_Back ); 
  ledcWrite( ledChannelRight_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_Flashlight_On() {

  digitalWrite(Flight, LOW);
  server.send(200, "text/plain", "ok");
  Serial.println("on on on on on ");
}

void handle_Flashlight_Off() {

  digitalWrite(Flight, LOW);
  server.send(200, "text/plain", "ok");
  Serial.println("off off off off off ");

}
