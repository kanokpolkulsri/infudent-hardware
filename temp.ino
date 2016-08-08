void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    int temp = (25*analogRead(A2) - 2050)/100;
    Serial.println(temp);
    delay(1000);
}
