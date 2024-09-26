#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin tanımları
const int gasSensorPin = A0;
const int potPin = A1;
const int joyXPin = A3;
const int joyYPin = A2;
const int joySWPin = 8; // SW pinini dijital pine bağladık
const int buzzerPin = 9;
const int greenLEDPin = 10;
const int redLEDPin = 11;

// Menü değişkenleri
int menuIndex = 0;
bool selectPressed = false;

// Eşik değeri
int threshold = 500; // Potansiyometre ile ayarlanabilir

void setup() {
  // Seri haberleşmeyi başlat
  Serial.begin(9600);

  // LCD ekranı başlat
  lcd.begin(16, 2);
  lcd.backlight();

  // Pin modlarını ayarla
  pinMode(gasSensorPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(joyXPin, INPUT);
  pinMode(joyYPin, INPUT);
  pinMode(joySWPin, INPUT_PULLUP); // Joystick düğmesi için pull-up direnç
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  // LCD ekranın ilk satırına bilgi yaz
  lcd.setCursor(0, 0);
  lcd.print("Ayarlar");
  lcd.setCursor(0, 1);
  lcd.print("Bildirimler");

  // Başlangıçta yeşil LED'i yak
  digitalWrite(greenLEDPin, HIGH);
  digitalWrite(redLEDPin, LOW);
}

void loop() {
  // Joystick'ten okuma yap
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);
  int joySWState = digitalRead(joySWPin);

  // Joystick düğmesine basılma durumu
  if (joySWState == LOW && !selectPressed) {
    selectPressed = true;
    if (menuIndex == 0) {
      ayarlarMenusu();
    } else if (menuIndex == 1) {
      bildirimlerMenusu();
    }
  } else if (joySWState == HIGH) {
    selectPressed = false;
  }

  // Joystick Y ekseni hareketi (menüde gezinme)
  if (joyYValue < 400) { // yukarı hareket
    menuIndex = 0;
  } else if (joyYValue > 600) { // aşağı hareket
    menuIndex = 1;
  }

  // Menü ekranını güncelle
  lcd.clear();
  if (menuIndex == 0) {
    lcd.setCursor(0, 0);
    lcd.print("> Ayarlar");
    lcd.setCursor(0, 1);
    lcd.print("  Bildirimler");
  } else if (menuIndex == 1) {
    lcd.setCursor(0, 0);
    lcd.print("  Ayarlar");
    lcd.setCursor(0, 1);
    lcd.print("> Bildirimler");
  }

  // Gaz sensöründen okuma yap
  int gasValue = analogRead(gasSensorPin);

  // Eşik değeri ile karşılaştır
  if (gasValue > threshold) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
  }

  delay(200); // Joystick'in yanlış okumalarını engellemek için bekleme süresi
}

void ayarlarMenusu() {
  while (true) {
    // Potansiyometreden okuma yap
    int potValue = analogRead(potPin);
    threshold = map(potValue, 0, 1023, 100, 1000);

    // LCD ekrana yaz
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Duyarlilik:");
    lcd.setCursor(0, 1);
    lcd.print(threshold);

    // Gaz sensöründen okuma yap ve eşik değeri ile karşılaştır
    int gasValue = analogRead(gasSensorPin);
    if (gasValue > threshold) {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(greenLEDPin, LOW);
      digitalWrite(redLEDPin, HIGH);
    } else {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
    }

    // Joystick düğmesine basılma durumu (geri dön)
    if (digitalRead(joySWPin) == LOW) {
      while (digitalRead(joySWPin) == LOW); // Buton bırakılana kadar bekle
      break;
    }
    delay(200);
  }
}

void bildirimlerMenusu() {
  while (true) {
    // Gaz sensöründen okuma yap
    int gasValue = analogRead(gasSensorPin);

    // LCD ekrana yaz
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gaz Seviyesi:");
    lcd.setCursor(0, 1);
    lcd.print(gasValue);

    // Gaz sensöründen okuma yap ve eşik değeri ile karşılaştır
    if (gasValue > threshold) {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(greenLEDPin, LOW);
      digitalWrite(redLEDPin, HIGH);
    } else {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(greenLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
    }

    // Joystick düğmesine basılma durumu (geri dön)
    if (digitalRead(joySWPin) == LOW) {
      while (digitalRead(joySWPin) == LOW); // Buton bırakılana kadar bekle
      break;
    }
    delay(200);
  }
}
