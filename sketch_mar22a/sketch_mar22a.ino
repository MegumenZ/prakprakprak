#include <Firebase_ESP_Client.h>
#include <WiFi.h>

#define led_merah 25
#define led_kuning 26
#define led_hijau 27
#define buzzer 14

// Sensor HC-SR04
const int trigPin = 5;
const int echoPin = 4;
long duration;
float distance;

// FIREBASE
#define FIREBASE_HOST "https://project-4-5eb1b-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define FIREBASE_API_KEY "Ze0U2jqvJ4SeCiIap0YX6Rdt1nFxGcoezGXcJGwJ" 

// Akun Firebase Authentication
#define USER_EMAIL "ardanie2004@gmail.com"
#define USER_PASSWORD "Andr331052004"

// Koneksi WiFi
#define WIFI_SSID "Zhafran" 
#define WIFI_PASSWORD "Kembar310504" 

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);
  pinMode(led_merah, OUTPUT);
  pinMode(led_kuning, OUTPUT);
  pinMode(led_hijau, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(trigPin, LOW);

  // Koneksi WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Konfigurasi Firebase
  config.api_key = FIREBASE_API_KEY;
  config.database_url = FIREBASE_HOST;
  
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Tunggu hingga login berhasil
  Serial.println("Logging in...");
  while (Firebase.ready() == false) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nFirebase Ready!");
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Kirim data ke Firebase
  if (Firebase.RTDB.setFloat(&firebaseData, "/WaterDistance", distance)) {
    Serial.println("Data dikirim ke Firebase");
  } else {
    Serial.println("Data gagal dikirim");
    Serial.println(firebaseData.errorReason());
  }

  // Kondisi indikator
  if (distance < 5) {
    digitalWrite(led_merah, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(led_kuning, LOW);
    digitalWrite(led_hijau, LOW);
  } else if (distance >= 5 && distance < 10) {
    digitalWrite(buzzer, LOW);
    digitalWrite(led_kuning, HIGH);
    delay(100);
    digitalWrite(led_kuning, LOW);
    delay(100);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(led_hijau, HIGH);
    digitalWrite(led_kuning, LOW);
    digitalWrite(led_merah, LOW);
  }

  delay(1000);  // Beri jeda sebelum loop berikutnya
}