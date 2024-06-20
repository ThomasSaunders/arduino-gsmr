#include "util.hh"

void newPage(bool skipHeadcode) {
  lcd.clear();
  lcd.setBacklight(100);
  
  lcd.setCursor(0, 0);
  lcd.print("GSM-R GB");
  
  if (!skipHeadcode) { 
    lcd.setCursor(16, 0);
    lcd.print(headcode);
  }
}