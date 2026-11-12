#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "index.h"

#include "secrets.h"  // identifiants Wi-Fi / Telegram (voir secrets.example.h)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int dernierSeuil = 100; 
WebServer server(80);

String soc="0", vbat="0", vac="0", r1="0", r2="0", r3="0", current="0";
String modeR1="auto", modeR2="auto", modeR3="auto"; // AJOUTÉ : Variable pour les modes
String buffer = ""; 
bool trame_en_cours = false;
bool modeMaintenance = false;

float histSOC[24] = {50, 52, 55, 58, 60, 65, 70, 75, 80, 85, 90, 92, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 45};
float histPOW[24] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 12};
String journalLogs = "[{\"time\":\"14:00\",\"type\":\"info\",\"msg\":\"Système démarré\"}]";

void setup() {
  Serial.begin(115200);
  client.setInsecure();
  pinMode(4, INPUT_PULLUP);
  Serial2.begin(115200, SERIAL_8N1, 4, 2);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  server.on("/", []() { server.send_P(200, "text/html", HTML_PAGE); });

  server.on("/DATA", []() {
    float p = vbat.toFloat() * current.toFloat();
    String autonomie = String(soc.toInt() / 10) + "h";
    String kwh = String(p / 1000.0, 2); 
    String downtime = (soc.toInt() < 10) ? "1.2h" : "0h"; 

    String json = "{\"soc\":\""+soc+"\",\"vbat\":\""+vbat+"\",\"vac\":\""+vac+"\",\"r1\":\""+r1+"\",\"r2\":\""+r2+"\",\"r3\":\""+r3+"\",\"current\":\""+current+"\",\"power\":\""+String(p, 1)+"\"";
    // AJOUTÉ : envoi des modes actuels au JSON
    json += ",\"modeR1\":\""+modeR1+"\",\"modeR2\":\""+modeR2+"\",\"modeR3\":\""+modeR3+"\"";
    json += ",\"autonomy\":\"" + autonomie + "\",\"daily_kwh\":\"" + kwh + "\",\"downtime\":\"" + downtime + "\"";
    json += ",\"history_soc\":[";
    for(int i=0; i<24; i++) { json += String(histSOC[i]) + (i<23?",":""); }
    json += "],\"history_pow\":[";
    for(int i=0; i<24; i++) { json += String(histPOW[i]) + (i<23?",":""); }
    json += "],\"logs\":" + journalLogs + "}";
    server.send(200, "application/json", json);
  });

  server.on("/CMD", []() {
    if (server.hasArg("R1")) { r1=server.arg("R1"); Serial2.print("R1:"); Serial2.println(r1); }
    if (server.hasArg("R2")) { r2=server.arg("R2"); Serial2.print("R2:"); Serial2.println(r2); }
    if (server.hasArg("R3")) { r3=server.arg("R3"); Serial2.print("R3:"); Serial2.println(r3); }
    // AJOUTÉ : réception des changements de mode
    if (server.hasArg("MODE_R1")) modeR1 = server.arg("MODE_R1");
    if (server.hasArg("MODE_R2")) modeR2 = server.arg("MODE_R2");
    if (server.hasArg("MODE_R3")) modeR3 = server.arg("MODE_R3");
    if (server.hasArg("MAINT")) modeMaintenance = (server.arg("MAINT") == "1");
    server.sendHeader("Location", "/");
    server.send(303);
  });
  server.begin();
}

void loop() {
  server.handleClient();
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '<') { buffer = ""; trame_en_cours = true; }
    else if (c == '>' && trame_en_cours) {
      trame_en_cours = false;
      int p1=buffer.indexOf(';'), p2=buffer.indexOf(';', p1+1), p3=buffer.indexOf(';', p2+1), 
          p4=buffer.indexOf(';', p3+1), p5=buffer.indexOf(';', p4+1), p6=buffer.indexOf(';', p5+1);
      if (p6 != -1) {
        soc=buffer.substring(0,p1); vbat=buffer.substring(p1+1,p2); vac=buffer.substring(p2+1,p3);
        r1=buffer.substring(p3+1,p4); r2=buffer.substring(p4+1,p5); r3=buffer.substring(p5+1,p6);
        current=buffer.substring(p6+1);
        if (!modeMaintenance) { 
           int s = soc.toInt();
           if (s <= 70 && dernierSeuil > 70) { bot.sendMessage(CHAT_ID, "⚠️ Alerte Niveau 1 : Batterie à " + soc + "% - Climatisation coupée.", ""); dernierSeuil = 70; }
           else if (s <= 40 && dernierSeuil > 40) { bot.sendMessage(CHAT_ID, "⚠️ Alerte Niveau 2 : Batterie à " + soc + "% - Prises coupées.", ""); dernierSeuil = 40; }
           else if (s <= 10 && dernierSeuil > 10) { bot.sendMessage(CHAT_ID, "🚨 Alerte Niveau 3 : Batterie à " + soc + "% - Éclairage coupé - Critique !", ""); dernierSeuil = 10; }
           else if (s > 75) { dernierSeuil = 100; }
        }
      }
    }
    else if (trame_en_cours) { buffer += c; if(buffer.length()>60) trame_en_cours=false; }
  }
}