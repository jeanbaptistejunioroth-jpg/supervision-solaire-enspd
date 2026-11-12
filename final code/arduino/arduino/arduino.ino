
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define A0_OFFSET      0.25
#define A0_FACTEUR     5.88
#define AC_CALIBRATION 0.85
#define OFFSET_CENTRE  280

// NOUVELLES CONSTANTES COURANT
#define PIN_COURANT    A2
#define OFFSET_COURANT 2.44 // Ta valeur mesurée au multimètre
#define SENSIBILITE    0.066 // 66mV/A pour le modèle 30A (valeur standard ACS712)

const int pinRelais1 = 7, pinRelais2 = 6, pinRelais3 = 5;
int forceRelais[3] = {0, 0, 0}; 

float vbatLisse = 0.0;
float vacLisse = 0.0;
float courantLisse = 0.0; // Variable pour lissage courant

String cmdBuffer = "";
unsigned long t_envoi = 0;

void setup() {
  Serial1.begin(115200); 
  lcd.init(); lcd.backlight();
  pinMode(pinRelais1, OUTPUT);
  pinMode(pinRelais2, OUTPUT);
  pinMode(pinRelais3, OUTPUT);
  digitalWrite(pinRelais1, HIGH);
  digitalWrite(pinRelais2, HIGH);
  digitalWrite(pinRelais3, HIGH);
}

void loop() {
  lireCommandesESP32();

  // 1. Lecture avec Lissage
  float rawVbat = lireTensionDC();
  float rawVac = lireTensionAC();
  float rawCourant = lireCourant();
  
  if (vbatLisse == 0) vbatLisse = rawVbat;
  vbatLisse = (vbatLisse * 0.95) + (rawVbat * 0.05);
  vacLisse = (vacLisse * 0.95) + (rawVac * 0.05);
  courantLisse = (courantLisse * 0.90) + (rawCourant * 0.10); // Lissage spécifique courant

  int soc = map(constrain((int)(vbatLisse * 100), 1120, 1220), 1120, 1220, 0, 100);

  gererRelais(soc);

  // 2. Affichage LCD
  lcd.setCursor(0, 0); lcd.print("SOC: "); lcd.print(soc); lcd.print("%  I: "); lcd.print(abs(courantLisse), 1); lcd.print("A ");
  lcd.setCursor(0, 1); lcd.print("Bat DC: "); lcd.print(vbatLisse, 1); lcd.print(" V   ");
  lcd.setCursor(0, 2); lcd.print("Ond AC: "); lcd.print((int)vacLisse); lcd.print(" V   ");
  lcd.setCursor(0, 3);
  lcd.print("E:"); lcd.print(digitalRead(pinRelais1) == LOW ? "ON " : "OF ");
  lcd.print("P:"); lcd.print(digitalRead(pinRelais2) == LOW ? "ON " : "OF ");
  lcd.print("C:"); lcd.print(digitalRead(pinRelais3) == LOW ? "ON " : "OF ");

  // 3. Transmission ESP32 (Trame avec courant ajouté à la fin)
  if (millis() - t_envoi >= 800) {
    t_envoi = millis();
    Serial1.print("<");
    Serial1.print(soc);               Serial1.print(";");
    Serial1.print(vbatLisse, 1);       Serial1.print(";");
    Serial1.print((int)vacLisse);      Serial1.print(";");
    Serial1.print(digitalRead(pinRelais1) == LOW ? 1 : 0); Serial1.print(";");
    Serial1.print(digitalRead(pinRelais2) == LOW ? 1 : 0); Serial1.print(";");
    Serial1.print(digitalRead(pinRelais3) == LOW ? 1 : 0); Serial1.print(";");
    Serial1.print(abs(courantLisse), 2);   // Valeur Courant ajoutée
    Serial1.println(">");
  }
}

// NOUVELLE FONCTION COURANT
float lireCourant() {
  float tensionAnalog = 0;
  for (int i = 0; i < 50; i++) {
    tensionAnalog += analogRead(PIN_COURANT) * (5.0 / 1023.0);
    delay(2);
  }
  float tensionMoyenne = tensionAnalog / 50.0;
  // Calcul : (Tension mesurée - Offset) / Sensibilité
  return (tensionMoyenne - OFFSET_COURANT) / SENSIBILITE;
}

// [Garder les autres fonctions (lireCommandesESP32, gererRelais, etc.) inchangées]
void lireCommandesESP32() {
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      cmdBuffer.trim();
      if (cmdBuffer.startsWith("R1:")) forceRelais[0] = (cmdBuffer.substring(3).toInt() == 1) ? 1 : 2;
      if (cmdBuffer.startsWith("R2:")) forceRelais[1] = (cmdBuffer.substring(3).toInt() == 1) ? 1 : 2;
      if (cmdBuffer.startsWith("R3:")) forceRelais[2] = (cmdBuffer.substring(3).toInt() == 1) ? 1 : 2;
      if (cmdBuffer.startsWith("AUTO")) { forceRelais[0]=0; forceRelais[1]=0; forceRelais[2]=0; }
      cmdBuffer = "";
    }
    else if (c != '\r') { cmdBuffer += c; if (cmdBuffer.length() > 20) cmdBuffer = ""; }
  }
}

void gererRelais(int soc) {
  if (soc < 65) forceRelais[2] = 0; 
  digitalWrite(pinRelais3, (forceRelais[2] == 0) ? (soc > 70 ? LOW : HIGH) : (forceRelais[2] == 1 ? LOW : HIGH));
  if (soc < 35) forceRelais[1] = 0;
  digitalWrite(pinRelais2, (forceRelais[1] == 0) ? (soc > 40 ? LOW : HIGH) : (forceRelais[1] == 1 ? LOW : HIGH));
  if (soc < 5) forceRelais[0] = 2;
  digitalWrite(pinRelais1, (forceRelais[0] == 0) ? (soc > 10 ? LOW : HIGH) : (forceRelais[0] == 1 ? LOW : HIGH));
}

float lireTensionDC() {
  float somme = 0;
  for (int i = 0; i < 100; i++) somme += analogRead(A0);
  float tension = ((somme / 100.0) * (5.0 / 1023.0) - A0_OFFSET) * A0_FACTEUR;
  return (tension < 0) ? 0 : tension;
}

float lireTensionAC() {
  unsigned long sommeCarres = 0;
  int n = 0;
  unsigned long debut = millis();
  while (millis() - debut < 200) {
    int v = analogRead(A1) - OFFSET_CENTRE;
    sommeCarres += (long)v * v;
    n++;
  }
  return sqrt((float)sommeCarres / n) * AC_CALIBRATION;
}