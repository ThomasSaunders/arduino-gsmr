#include <LiquidCrystal_I2C.h>
#include <Wire.h>

byte Brick[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

byte Line[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
};

byte Blank[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

String headcode;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void newPage(bool skipHeadcode = false) {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("GSM-R GB");
  
  if (!skipHeadcode) { 
    lcd.setCursor(16, 0);
    lcd.print(headcode);
  }
}

void setup() {
  analogReference(EXTERNAL);
  
  headcode = "";

  Serial.begin(9600);
  
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.createChar(0, Brick);
  lcd.createChar(1, Line);
  lcd.createChar(8, Blank);

  for (int ia = 0; ia <= 3; ia++) {
    lcd.setCursor(0, ia);
    
    for (int ib = 0; ib <= 20; ib++) {
      lcd.write(0);
    }
  }

  delay(1500);

  newPage(true);

  lcd.setCursor(0, 1);
  lcd.print("Powering up/testing");

  lcd.setCursor(0, 2);
  lcd.print("NR3.4.1");

  if(!Serial) {
    lcd.clear();
    
    lcd.setCursor(1, 1);
    lcd.print("Test NOT OK");

    lcd.setCursor(1, 2);
    lcd.print("!Serial");
  }

  delay(1500);

  newPage(true);

}

void loop() {
  if (Serial.available() > 0) {
    String serialInput = Serial.readStringUntil('\n');
    Serial.println(serialInput);

    if (serialInput == "register") {
      newPage();
      Serial.println("Register page");

      registerRadio();
      newPage();
    }
    else if (serialInput == "shutdown") {
      lcd.noDisplay();
      lcd.clear();
    }

    // Serial.println(analogRead(buttonInputPin));

    // delay(250);  
  }
}



void registerRadio() {
  String signalNumber = "";
  char serialInput;

  bool headcodeComplete = false;
  bool signalComplete = false;

  int headcodePosition = 0;
  int signalPosition = 0;

  lcd.setCursor(0, 1);
  lcd.print("Registration code");

  while(!headcodeComplete) {
    if(Serial.available() > 0) {
      serialInput = Serial.readStringUntil('\n')[0];    

      Serial.println(serialInput);

      if (headcodePosition == 0 || headcodePosition == 2 || headcodePosition == 3 && isDigit(serialInput)) {
        headcode += serialInput;

        headcodePosition++;
      } else if (isAlpha(serialInput)){
        headcode += serialInput;

        headcodePosition++;
      }

      if (headcodePosition == 4) {
        headcodeComplete = true;
      }

      headcode.toUpperCase();

      lcd.setCursor(0, 2);
      lcd.print(headcode);   
    } else {
      lcd.setCursor(headcodePosition, 2);
      lcd.write(1);
      
      delay(500);

      lcd.write(8);
    }
  }

  while (!signalComplete) {
    while (Serial.available() <= 0) 
    {}

    serialInput = Serial.readStringUntil('\n')[0];

    Serial.println(serialInput);

    if (isDigit(serialInput)) {
      signalNumber += serialInput;

      signalPosition++;
    }

    if (signalPosition == 3) {
      signalComplete = true;
    }

    lcd.setCursor(5, 2);
    lcd.print(signalNumber);
  }

  while (Serial.available() <= 0) 
  {}
  
  newPage(true);
  delay(757); 

  lcd.setCursor(0, 1);
  lcd.print("Registering");

  lcd.setCursor(0, 2);
  lcd.print(headcode + " " + signalNumber + " Lead Driver");

  delay(2000);

  lcd.clear();
  newPage(true);

  delay(767);

  lcd.setCursor(0, 1);
  lcd.print("Wait/check head code");

  lcd.setCursor(0, 2);
  lcd.print(headcode + " " + signalNumber);

  delay(2000);
}

