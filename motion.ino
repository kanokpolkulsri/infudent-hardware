void setup() {
 Serial.begin(9600);
 pinMode(2, INPUT);
}

void loop() {
 int sw = digitalRead(2);
 Serial.println(sw);
 delay(300);
}

