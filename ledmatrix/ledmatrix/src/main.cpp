#include <Arduino.h>
#include <LedControl.h>

int DIN = P2_3;
int CS = P2_4;
int CLK = P2_5;
LedControl lc = LedControl(DIN, CLK, CS, 1);

int Cat[8] = {B10001000, B11111000, B10101000, B01110001, B00100001, B01111001, B01111101, B10111110};
byte Apple[8] = {B00011000, B00001000, B01110110, B11111111, B11111111, B11111111, B01111010, B00110100};

void setup()
{
  lc.shutdown(0, false);
  lc.setIntensity(0, 0);
  lc.clearDisplay(0);
}

void loop()
{
  for (int i = 0; i < 8; i++)
    lc.setRow(0, i, Apple[i]);

  digitalWrite(P1_0, HIGH);
  delay(1000);
  digitalWrite(P1_0, LOW);

  lc.clearDisplay(0);

  for (int i = 0; i < 8; i++)
    lc.setRow(0, i, Cat[i]);

  digitalWrite(P1_0, HIGH);
  delay(1000);
  digitalWrite(P1_0, LOW);

  lc.clearDisplay(0);
}