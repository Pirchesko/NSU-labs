void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int checkpoint;
  Serial.println(sizeof(checkpoint));
  //Serial.println("sdfsdf");
  
  digitalWrite(LED_BUILTIN, HIGH);
  
  checkpoint = millis();
  Serial.println(checkpoint);
  
  while (millis() < checkpoint + 1000)
  {
    //delay(1);
  }

  digitalWrite(LED_BUILTIN, LOW);

  checkpoint = millis();
  Serial.println(checkpoint);
  
  while (millis() < checkpoint + 1000)
  {
    //delay(1);
  }
}
