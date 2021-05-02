#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x23, 20, 4);

int led1 = 4;
int led2 = 5;
int led3 = 6;
int led4 = 7;
int vr = A0;
int rom = 184;
int SW = 8;

int epvalue1, epvalue2, epvalue3, epvalue4;

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  
  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(SW, INPUT);

  lcd.begin();
  lcd.backlight();

  Serial.println();
  Serial.println("Connected.");

  lcd.setCursor(2, 1); // กำหนดตำแหน่งเคอร์เซอร์ที่ แถวที่ 0 บรรทัดที่ 0
  lcd.print("SW Control Prog."); //พิมพ์ข้อความ
  lcd.setCursor(5, 2); // กำหนดตำแหน่งเคอร์เซอร์ที่ แถวที่ 2 บรรทัดที่ 1
  lcd.print("Connected."); //พิมพ์ข้อความ "arduinoall.com"

  delay(3000);
  lcd.clear();
  int output = readOutput(analogRead(vr));
  int statsw = digitalRead(SW);

  if ((output >= (rom - 1) && output <= (rom + 1)) || (statsw == 1)) {
    Serial.println("Set LED1...");
    lcd.setCursor(5, 2);
    lcd.print("Set LED1...");
    delay(3000);
    writeIntIntoEEPROM(10, readOutput(analogRead(vr)));
    Serial.println("Set LED2...");
    lcd.setCursor(5, 2);
    lcd.print("Set LED2...");
    delay(3000);
    writeIntIntoEEPROM(20, readOutput(analogRead(vr)));
    Serial.println("Set LED3...");
    lcd.setCursor(5, 2);
    lcd.print("Set LED3...");
    delay(3000);
    writeIntIntoEEPROM(30, readOutput(analogRead(vr)));
    Serial.println("Set LED4...");
    lcd.setCursor(5, 2);
    lcd.print("Set LED4...");
    delay(3000);
    writeIntIntoEEPROM(40, readOutput(analogRead(vr)));
    Serial.println("Save EEPROM....");
    lcd.setCursor(3, 2);
    lcd.print("Save EEPROM....");
    delay(3000);
  }
  Serial.println("Print EEPROM....");
  epvalue1 = readIntFromEEPROM(10);
  epvalue2 = readIntFromEEPROM(20);
  epvalue3 = readIntFromEEPROM(30);
  epvalue4 = readIntFromEEPROM(40);
  Serial.println(epvalue1);
  Serial.println(epvalue2);
  Serial.println(epvalue3);
  Serial.println(epvalue4);
  
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(vr);
  int output = map(val, 0, 1023, 0, 255);

  Serial.print(output);
  Serial.print("\t");
  Serial.print(val);
  Serial.print("\t");
  lcd.setCursor(1, 1);
  lcd.print(output);
  if (val >= 1000) {
    lcd.setCursor(0, 2);
    lcd.print(val);
  } else {  
    lcd.setCursor(0, 2);
    lcd.print(" ");  
    lcd.setCursor(1, 2);
    lcd.print(val);
  }

  if (output >= (epvalue1 - 1) && output <= (epvalue1 + 1)) {
    lcd.setCursor(5, 0);
    lcd.print("LED 001 ON ");
    Serial.print("LED1 ON");
    Serial.print("\n");
    controlLED(1, 0, 0, 0);
  } else if (output >= (epvalue2 - 1) && output <= (epvalue2 + 1)) {
    lcd.setCursor(5, 1);
    lcd.print("LED 002 ON ");
    Serial.print("LED2 ON");
    Serial.print("\n");
    controlLED(0, 1, 0, 0);
  } else if (output >= (epvalue3 - 1) && output <= (epvalue3 + 1)) {
    lcd.setCursor(5, 2);
    lcd.print("LED 003 ON ");
    Serial.print("LED3 ON");
    Serial.print("\n");
    controlLED(0, 0, 1, 0);
  } else if (output >= (epvalue4 - 1) && output <= (epvalue4 + 1)) {
    lcd.setCursor(5, 3);
    lcd.print("LED 004 ON ");
    Serial.print("LED4 ON");
    Serial.print("\n");
    controlLED(0, 0, 0, 1);
  } else {
    lcd.setCursor(5, 0);
    lcd.print("LED 001 OFF");
    lcd.setCursor(5, 1);
    lcd.print("LED 002 OFF");
    lcd.setCursor(5, 2);
    lcd.print("LED 003 OFF");
    lcd.setCursor(5, 3);
    lcd.print("LED 004 OFF");
    Serial.print("LED ALL OFF");
    Serial.print("\n");
    controlLED(0, 0, 0, 0);
  }
  delay(1);
}

void controlLED(int LED1, int LED2, int LED3, int LED4) {
  digitalWrite(led1, LED1);
  digitalWrite(led2, LED2);
  digitalWrite(led3, LED3);
  digitalWrite(led4, LED4);
}

void writeIntIntoEEPROM(int address, int number)
{
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

int readOutput(int val) {
  int output = map(val, 0, 1023, 0, 255);

  Serial.print(output);
  Serial.print("\t");
  Serial.println(val);

  return output;
}
