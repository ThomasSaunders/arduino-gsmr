#include "register_radio.hh"
#include "util.hh"

// Define custom characters for the LCD
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

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  // Register custom characters
  lcd.createChar(0, Brick);
  lcd.createChar(1, Line);
  lcd.createChar(8, Blank);

  // Draw bricks across full display for startup
  // FIXME - Fix bricks not displaying. Issue #5
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

    if (serialInput == "r") {
      newPage();
      Serial.println("Register page");

      registerRadio();
      
      newPage();
    }
    else if (serialInput == "s") {
      lcd.noDisplay();
      lcd.clear();
    }

    // TODO - Convert system to function via button press rather than serial input. Breadboard button would most likely be best for this.

    // Serial.println(analogRead(buttonInputPin));

    // delay(250);  
  }
}