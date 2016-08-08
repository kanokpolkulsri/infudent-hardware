#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(3,0);
  lcd.print("EXTERNAL");
  lcd.setCursor(0,1);
  lcd.print("INTERVIEWING");
}
