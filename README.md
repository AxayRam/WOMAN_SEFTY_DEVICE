# WOMAN_SEFTY_DEVICE

# 🛡️ Women Safety Device using Arduino Mega (GSM + GPS + Keypad)

This project is a real-time Women Safety Device built using an **Arduino Mega**, **SIM800L GSM module**, **NEO-6M GPS**, **16x2 I2C LCD**, and a **4x4 Keypad**. The system is designed to help women in emergencies by quickly sending their location to two pre-stored phone numbers and making phone calls.

---

## 🚨 Key Features

- 🔢 Enter two emergency phone numbers using a 4x4 keypad
- 💾 Save numbers permanently using internal EEPROM (non-volatile)
- 📍 Send **SMS with real-time GPS coordinates and Google Maps link**
- 📞 Make direct calls to saved numbers (Call 1 or Call 2)
- 🔁 Reset numbers anytime with a single button
- 🖥️ LCD displays user instructions and system status

---

## 📦 Components Used

| Component          | Description                           |
|--------------------|---------------------------------------|
| Arduino Mega 2560  | Main microcontroller board            |
| SIM800L GSM Module | For SMS and Calling                   |
| NEO-6M GPS Module  | For Latitude and Longitude tracking   |
| 16x2 I2C LCD       | For displaying messages               |
| 4x4 Matrix Keypad  | For inputting numbers and actions     |
| EEPROM (internal)  | For storing phone numbers             |
| Battery (9V / Li-ion) | Power supply for GSM and Arduino  |

---

## 🔌 Connections Overview

| Module             | Arduino Mega Pins                                  |
|--------------------|----------------------------------------------------|
| LCD (I2C)          | SDA → 20, SCL → 21                                 |
| GSM Module         | TX → D18 (RX1), RX → D19 (TX1) via voltage divider |
| GPS Module         | TX → D17 (RX2)                                     |
| Keypad Rows        | R1=7, R2=8, R3=9, R4=10                            |
| Keypad Cols        | C1=11, C2=12, C3=13, C4=A6                         |
| Buttons (optional) | Connected to A6–A8 with pull-down                  | 

---

## 📱 How It Works

1. On startup, the user is prompted to **enter two 10-digit phone numbers** via keypad.
2. Once saved, the numbers are stored in EEPROM permanently.
3. LCD shows main menu:

B=SMS C=CALL1
D=CALL2 A=RST


4. Functions:
- **Press 'B'** → Sends SMS to both numbers with GPS location
- **Press 'C'** → Calls Number 1
- **Press 'D'** → Calls Number 2
- **Press 'A'** → Resets phone numbers and starts over

---

## ✉️ Emergency SMS Format

If GPS is available:

⚠️ Emergency Alert!

I am in danger and need help immediately.
Please check my location and come quickly.

📍 Location:
Lat:23.034567 Lon:72.564789
https://maps.google.com/?q=23.034567,72.564789

This is serious. Please act fast.


If GPS fails:

⚠️ Emergency!
Unable to fetch GPS location. Please call me urgently.


---

## 🛠️ File Structure


