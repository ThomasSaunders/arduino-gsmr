#ifndef util_hh
#define util_hh

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

static String headcode;

void newPage(bool skipHeadcode = false);

static LiquidCrystal_I2C lcd(0x27, 20, 4);

#endif