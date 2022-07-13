void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(0, knopka, RISING);
}

void knopka()
{
  Serial.println("knopka!");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly
  digitalWrite(LED_BUILTIN, HIGH);
}
