
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 8, 9, 10};
byte colPins[COLS] = {11, 12, 13, A6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// EEPROM & Phone Numbers
String phone1 = "", phone2 = "";
bool enteredPhone1 = false, enteredPhone2 = false;
const int slot1 = 0;
const int slot2 = 20;

// GPS
TinyGPSPlus gps;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  Serial.begin(9600);       // Debugging
  Serial1.begin(9600);      // GSM TX=D18, RX=D19
  Serial2.begin(9600);      // GPS TX to D17

  loadFromEEPROM();

  lcd.clear();
  if (!enteredPhone1) lcd.print("Enter Number 1:");
  else if (!enteredPhone2) lcd.print("Enter Number 2:");
  else showMainMenu();
}

void loop() {
  while (Serial2.available()) gps.encode(Serial2.read());

  char key = keypad.getKey();
  if (!key) return;

  if (key == 'A') {
    clearEEPROM();
    phone1 = ""; phone2 = "";
    enteredPhone1 = false; enteredPhone2 = false;
    lcd.clear(); lcd.print("RESET Done");
    delay(1000);
    lcd.clear(); lcd.print("Enter Number 1:");
    return;
  }

  if (!enteredPhone1) {
    handlePhoneEntry(key, phone1, enteredPhone1, "Number 1:", slot1);
    if (enteredPhone1) {
      lcd.clear(); lcd.print("Enter Number 2:");
    }
  } else if (!enteredPhone2) {
    handlePhoneEntry(key, phone2, enteredPhone2, "Number 2:", slot2);
    if (enteredPhone2) {
      lcd.clear(); lcd.print("Saved Both!");
      delay(1000); showMainMenu();
    }
  } else {
    if (key == 'B') {
      lcd.clear(); lcd.print("Sending SMS 1");
      sendSMS(phone1);
      delay(2000);

      lcd.clear(); lcd.print("Sending SMS 2");
      sendSMS(phone2);
      delay(2000);

      showMainMenu();
    } else if (key == 'C') {
      lcd.clear(); lcd.print("Calling No.1");
      makeCall(phone1);
      delay(5000); // Ring time
      hangUp();
      showMainMenu();
    } else if (key == 'D') {
      lcd.clear(); lcd.print("Calling No.2");
      makeCall(phone2);
      delay(5000); // Ring time
      hangUp();
      showMainMenu();
    }
  }
}

// Phone number input
void handlePhoneEntry(char key, String &phone, bool &done, const char* label, int eepromStart) {
  if (key == '#') {
    if (phone.length() == 10) {
      saveToEEPROM(phone, eepromStart);
      done = true;
    } else {
      lcd.clear(); lcd.print("Enter 10 digits!");
      delay(1500);
    }
    lcd.clear(); lcd.print(label);
    return;
  }

  if (key == '*') phone = "";
  else if (phone.length() < 10 && isDigit(key)) phone += key;

  lcd.clear();
  lcd.print(label);
  lcd.setCursor(0, 1);
  lcd.print("+91" + phone);
}

// EEPROM
void saveToEEPROM(String num, int addr) {
  for (int i = 0; i < num.length(); i++) EEPROM.write(addr + i, num[i]);
  EEPROM.write(addr + num.length(), '\0');
}

void loadFromEEPROM() {
  char ch;
  phone1 = ""; phone2 = "";

  for (int i = 0; i < 15; i++) {
    ch = EEPROM.read(slot1 + i);
    if (ch == '\0') break;
    phone1 += ch;
  }
  if (phone1.length() == 10) enteredPhone1 = true;

  for (int i = 0; i < 15; i++) {
    ch = EEPROM.read(slot2 + i);
    if (ch == '\0') break;
    phone2 += ch;
  }
  if (phone2.length() == 10) enteredPhone2 = true;
}

void clearEEPROM() {
  for (int i = 0; i < 40; i++) EEPROM.write(i, 0);
}

void showMainMenu() {
  lcd.clear();
  lcd.print("B=SMS C=CALL1");
  lcd.setCursor(0, 1);
  lcd.print("D=CALL2 A=RST");
}

// SMS Sending
bool sendSMS(const String &num) {
  String fullNumber = "+91" + num;
  Serial.println("Sending SMS to: " + fullNumber);

  Serial1.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);

  Serial1.print("AT+CMGS=\"");
  Serial1.print(fullNumber);
  Serial1.println("\"");
  delay(1000);

  String msg = "Help!\n ⚠️ Emergency Alert! I am not safe.\n Location:\n";
  if (gps.location.isValid()) {
    msg += "Lat:";
    msg += String(gps.location.lat(), 6);
    msg += " \nLon:";
    msg += String(gps.location.lng(), 6);
    msg += "\nhttps://maps.google.com/?q=";
    msg += String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  } else {
    msg += "Location Not Found";
  }

  Serial1.print(msg);
  Serial1.write(26);  // Ctrl+Z to send
  delay(3000);

  return true;
}

// Call
bool makeCall(const String &num) {
  String fullNumber = "+91" + num;
  Serial1.print("ATD");
  Serial1.print(fullNumber);
  Serial1.println(";");
  delay(1000);
  return true;
}

// Hang up
void hangUp() {
  Serial1.println("ATH");
  delay(1000);
}
