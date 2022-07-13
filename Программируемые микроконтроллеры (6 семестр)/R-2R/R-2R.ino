int a[32];
unsigned char x = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  a[0] = 0 + 8;
  a[1] = 1 + 8;
  a[2] = 3 + 8;
  a[3] = 4 + 8;
  a[4] = 5 + 8;
  a[5] = 6 + 8;
  a[6] = 6 + 8;
  a[7] = 7 + 8;
  a[8] = 7 + 8;
  a[9] = 7 + 8;
  a[10] = 6 + 8;
  a[11] = 6 + 8;
  a[12] = 5 + 8;
  a[13] = 4 + 8;
  a[14] = 3 + 8;
  a[15] = 1 + 8;
  a[16] = 0 + 8;
  a[17] = -1 + 8;
  a[18] = -3 + 8;
  a[19] = -4 + 8;
  a[20] = -5 + 8;
  a[21] = -6 + 8;
  a[22] = -7 + 8;
  a[23] = -7 + 8;
  a[24] = -8 + 8;
  a[25] = -7 + 8;
  a[26] = -7 + 8;
  a[27] = -6 + 8;
  a[28] = -5 + 8;
  a[29] = -4 + 8;
  a[30] = -3 + 8;
  a[31] = -1 + 8;
}



void sinus()
{
  //Serial.println(PI);
  for (int i = 0; i < 32; i++)
  {
    Serial.print(i);
    Serial.print(" ");
    Serial.println(7.5 * sin(((double)PI / 16.0) * (double) i));
  }
  delay(100000);
}  

unsigned long previousTime = micros(); // or millis()
int val = 0;
long timeInterval = 1000/32 * (val + 1);

void loop() {
  //sinus();

  //unsigned long currentTime = micros();
  //if (currentTime - previousTime > timeInterval) {
    x++;
    x %= 32;
    digitalWrite(2, a[x] & 1);
    digitalWrite(3, a[x] & 2);
    digitalWrite(4, a[x] & 4);
    digitalWrite(5, a[x] & 8);
    //previousTime = currentTime;
  //}

  //val = analogRead(A0);
  //timeInterval = 1000/32 * ((val + 1)/10);
  //Serial.println(val);
  //timeInterval = 1;
  
  /*for (int i = 0; i < 16; i++) 
  {
    int c = i;
    int j = 0;
    int a[4];
    while (j < 4) 
    {
      if (c % 2 == 0) 
      {
        digitalWrite(j+2, LOW);
        a[j] = 0;
      }
      else 
      {
        digitalWrite(j+2, HIGH);
        a[j] = 1;
      }
      c = c / 2;
      j = j + 1;
    }
    for (int k = 4; k > 0; k--) 
    {
      Serial.print(a[k]);
    }
    Serial.print(" ");
    Serial.println(i);
    delay(100);
    Serial.println(micros());
  }*/
}
