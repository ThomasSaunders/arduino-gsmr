#include "register_radio.hh"
#include "util.hh"

void registerRadio() {
  String signalNumber = "";
  String headcode = "";

  char serialInput;

  bool headcodeComplete = false;
  bool signalComplete = false;

  int headcodePosition = 0;
  int signalPosition = 0;

  lcd.init();
  lcd.setBacklight(100);
  
  newPage();
  
  lcd.setCursor(0, 1);
  lcd.print("Registration code");

  while(!headcodeComplete) {
    if(Serial.available() > 0) {
      serialInput = Serial.readStringUntil('\n')[0];    

      Serial.println(serialInput);

      if (isAlpha(serialInput) && headcodePosition == 1) {
        headcode += serialInput;

        headcodePosition++;
      } else if (isDigit(serialInput) && headcodePosition != 1) {
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

      // FIXME - Cursor doesn't blink. Issue #6
      lcd.setCursor(headcodePosition, 2);
      lcd.write(1);
      
      delay(500);

      lcd.write(8);
    }
  }

  // TODO - Use same Serial logic as headcode.
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

    // TODO - Add blinking line.
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
