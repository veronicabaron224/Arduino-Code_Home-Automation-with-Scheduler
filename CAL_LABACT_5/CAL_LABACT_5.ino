#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>

#define Password_Length 8

const int relayPin = 12;
char Data[Password_Length];
char Master[Password_Length] = "123A456";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(relayPin, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
  digitalWrite(relayPin, LOW);

  customKey = customKeypad.getKey();
  if (customKey) {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
  }

  if (data_count == Password_Length - 1) {
    delay(500); 
    lcd.clear();
    if (checkPassword(Data)) {
      lcd.print("Correct");
      Serial.println(Data);
      delay(500);
    } 
    else {
      lcd.print("Incorrect");
      delay(500);
    }
    lcd.clear();
    clearData();
  }

  controlRelay();
}

bool checkPassword(char* input) {
  return strcmp(input, Master) == 0;
}

void controlRelay() {
  if (Serial.available() > 0) {
    char winforms_a = Serial.read();
    if (winforms_a == 'A') {
      digitalWrite(relayPin, HIGH);
    }   
    while (Serial.read() != 'B') {
      delay(500);
    }
    char winforms_b = Serial.read();
    if (winforms_b == 'B') {
      digitalWrite(relayPin, LOW);
    }
  }
}

void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
}