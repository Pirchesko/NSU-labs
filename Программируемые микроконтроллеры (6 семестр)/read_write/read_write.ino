void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  char symbol = Serial.read();
  if (symbol != -1)
  {
    Serial.print(symbol);
  }
}
