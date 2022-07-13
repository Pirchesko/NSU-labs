int analogPin = A0;
int val = 0;
int const_t = 300;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void symbols(char c, char* morse) {
  c = tolower(c);
  switch(c) {
    case 'a': 
    {
      morse[0] = 0;
      morse[1] = 1;
      break;
    }
    case 'b': 
    {
      morse[0] = 1;
      morse[1] = 0;
      morse[2] = 0;
      morse[3] = 0;
      break;
    }
    case 'c': 
    {
      morse[0] = 1;
      morse[1] = 0;
      morse[2] = 1;
      morse[3] = 0;
      break;
    }
    case 'd': 
    {
      morse[0] = 1;
      morse[1] = 0;
      morse[2] = 0;
      break;
    }
    case 'e': 
    {
      morse[0] = 0;
      break;
    }
    case 'f': 
    {
      morse[0] = 0;
      morse[1] = 0;
      morse[2] = 1;
      morse[3] = 0;
      break;
    }
    case 'g': 
    {
      morse[0] = 1;
      morse[1] = 1;
      morse[2] = 0;
      break;
    }
    case 'h': 
    {
      morse[0] = 0;
      morse[1] = 0;
      morse[2] = 0;
      morse[3] = 0;
      break;
    }
    case 'i': 
    {
      morse[0] = 0;
      morse[1] = 0;
      break;
    }
    case 'j': 
    {
      morse[0] = 0;
      morse[1] = 1;
      morse[2] = 1;
      morse[3] = 1;
      break;
    }
    case 'k': 
    {
      morse[0] = 1;
      morse[1] = 0;
      morse[2] = 1;
      break;
    }
    case 'l': 
    {
      morse[0] = 0;
      morse[1] = 1;
      morse[2] = 0;
      morse[3] = 0;
      break;
    }
    case 'm': 
    {
      morse[0] = 1;
      morse[1] = 1;
      break;
    }
    case 'n': 
    {
      morse[0] = 1;
      morse[1] = 0;
      break;
    }
    case 'o': 
    {
      morse[0] = 1;
      morse[1] = 1;
      morse[2] = 1;
      break;
    }
    case 'p': 
    {
      morse[0] = 0;
      morse[1] = 1;
      morse[2] = 1;
      morse[3] = 0;
      break;
    }
    case 'q': 
    {
      morse[0] = 1;
      morse[1] = 1;
      morse[2] = 0;
      morse[3] = 1;
      break;
    }
    case 'r': 
    {
      morse[0] = 0;
      morse[1] = 1;
      morse[2] = 0;
      break;
    }
    case 's': 
    {
      morse[0] = 0;
      morse[1] = 0;
      morse[2] = 0;
      break;
    }
    case 't': 
    {
      morse[0] = 1;
      break;
    }
    case 'u': 
    {
      morse[0] = 0;
      morse[1] = 0;
      morse[2] = 1;
      break;
    }
    case 'v': 
    {
      morse[0] = 0;
      morse[1] = 0;
      morse[2] = 0;
      morse[3] = 1;
      break;
    }
    case 'w': 
    {
      morse[0] = 0;
      morse[1] = 1;
      morse[2] = 1;
      break;
    }
    case 'x': 
    {
      morse[0] = 1;
      morse[1] = 0;
      morse[2] = 0;
      morse[3] = 1;
      break;
    }
    case 'y': 
    {
      morse[0] = 1;
      morse[1] = 0;
      morse[2] = 1;
      morse[3] = 1;
      break;
    }
    case 'z': 
    {
      morse[0] = 1;
      morse[1] = 1;
      morse[2] = 0;
      morse[3] = 0;
      break;
    }
    default: 
    {
      break;
    }
  }
}

void blink_morse(char* morse)
{
  for (int i = 0; morse[i] != -1; i++){
    switch(morse[i]) 
    {
      case 0: 
      {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(const_t);
        digitalWrite(LED_BUILTIN, LOW);
        delay(const_t);
        break;
      }
      case 1: 
      {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(const_t*3);
        digitalWrite(LED_BUILTIN, LOW);
        delay(const_t);
        break;
      }
      default:
      {
        //digitalWrite(LED_BUILTIN, LOW);
        //delay(const_t);
        break;
      }
    }
  }
  delay(const_t*2); //после каждого символа нужно ждать 3 пунтка (1 пункт уже уазан после каждого слова)
}

void loop() 
{
  //val = analogRead(analogPin);
  //Serial.println(val);

  char morse[5] = {-1, -1, -1, -1, -1};
  
  char symbol = Serial.read();
  if (symbol != -1)
  {
    Serial.print(symbol);
    //Serial.print(morse);
    symbols(symbol, morse);
    blink_morse(morse);    
  }
  
}
