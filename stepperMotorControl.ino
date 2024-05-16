#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "EOLO - FRITZ!Box 7530 VJ";     // SSID della tua rete WiFi
const char* password = "14417380252310105223"; // Password della tua rete WiFi

const int stepPin = 32;
const int dirPin = 35; 
const int enPin = 33;
bool motorEnabled = false;
bool motorDirection = false; // false per il movimento in un verso, true per il movimento nell'altro verso

WebServer server(80);

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  
  Serial.begin(115200);
  
  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connettendo alla rete ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connesso alla rete WiFi");
  
  // Stampare l'indirizzo IP
  Serial.print("Indirizzo IP assegnato: ");
  Serial.println(WiFi.localIP());

  // Imposta le pagine web
  server.on("/", handleRoot);
  server.on("/changeDirection", handleChangeDirection);
  server.on("/toggleMotor", handleToggleMotor);
  
  // Avvia il server
  server.begin();
  Serial.println("Server avviato");
}

void loop() {
  server.handleClient(); // Gestione delle richieste HTTP
}

// Funzione per gestire la richiesta della radice "/"
void handleRoot() {
  String page = "<!DOCTYPE html><html><head><title>Controllo Motore</title></head><body>";
  page += "<h1>Controllo Motore</h1>";
  page += "<form action='/changeDirection' method='post'><button type='submit'>Cambia Direzione</button></form>";
  page += "<form action='/toggleMotor' method='post'><button type='submit'>" + String(motorEnabled ? "Disabilita" : "Abilita") + " Motore</button></form>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

// Funzione per gestire la richiesta di cambiamento di direzione
void handleChangeDirection() {
  motorDirection = !motorDirection; // Inverti la direzione
  digitalWrite(dirPin, motorDirection ? HIGH : LOW); // Imposta il pin di direzione in base alla nuova direzione
  server.sendHeader("Location", "/");
  server.send(303); // Redirect alla pagina principale
}

// Funzione per gestire la richiesta di abilitare/disabilitare il motore
void handleToggleMotor() {
  motorEnabled = !motorEnabled; // Inverti lo stato del motore
  digitalWrite(enPin, motorEnabled ? LOW : HIGH); // Abilita o disabilita il motore
  server.sendHeader("Location", "/");
  server.send(303); // Redirect alla pagina principale
}