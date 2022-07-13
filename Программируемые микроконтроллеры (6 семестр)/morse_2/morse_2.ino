int const_t = 300;
long long int ms;
int button_push = 0;
int button_out = 0;
int flag_push = 0;
int flag_out = 0;
char morse[5] = {-1, -1, -1, -1, -1};
int index = 0;

char symbols()
{
  if (morse[0] == 0 && morse[1] == 1 && morse[2] == -1) return 'a';
  if (morse[0] == 1 && morse[1] == 0 && morse[2] == 0 && morse[3] == 0 && morse[4] == -1) return 'b';
  if (morse[0] == 1 && morse[1] == 0 && morse[2] == 1 && morse[3] == 0 && morse[4] == -1) return 'c';
  if (morse[0] == 1 && morse[1] == 0 && morse[2] == 0 && morse[3] == -1) return 'd';
  if (morse[0] == 0 && morse[1] == -1) return 'e';
  if (morse[0] == 0 && morse[1] == 0 && morse[2] == 1 && morse[3] == 0 && morse[4] == -1) return 'f';
  if (morse[0] == 1 && morse[1] == 1 && morse[2] == 0 && morse[3] == -1) return 'g';
  if (morse[0] == 0 && morse[1] == 0 && morse[2] == 0 && morse[3] == 0 && morse[4] == -1) return 'h';
  if (morse[0] == 0 && morse[1] == -1) return 'i';
  if (morse[0] == 0 && morse[1] == 1 && morse[2] == 1 && morse[3] == 1 && morse[4] == -1) return 'j';
  if (morse[0] == 1 && morse[1] == 0 && morse[2] == 1 && morse[3] == -1) return 'k';
  if (morse[0] == 0 && morse[1] == 1 && morse[2] == 0 && morse[3] == 0 && morse[4] == -1) return 'l';
  if (morse[0] == 1 && morse[1] == 1 && morse[2] == -1) return 'm';
  if (morse[0] == 1 && morse[1] == 0 && morse[2] == -1) return 'n';
  if (morse[0] == 1 && morse[1] == 1 && morse[2] == 1 && morse[3] == -1) return 'o';
  if (morse[0] == 0 && morse[1] == 1 && morse[2] == 1 && morse[3] == 0 && morse[4] == -1) return 'p';
  if (morse[0] == 1 && morse[1] == 1 && morse[2] == 0 && morse[3] == 1 && morse[4] == -1) return 'q';
  if (morse[0] == 0 && morse[1] == 1 && morse[2] == 0 && morse[3] == -1) return 'r';
  if (morse[0] == 0 && morse[1] == 0 && morse[2] == 0 && morse[3] == -1) return 's';
  if (morse[0] == 1 && morse[1] == -1) return 't';
  if (morse[0] == 0 && morse[1] == 0 && morse[2] == 1 && morse[3] == -1) return 'u';
  if (morse[0] == 0 && morse[1] == 0 && morse[2] == 0 && morse[3] == 1 && morse[4] == -1) return 'v';
  if (morse[0] == 0 && morse[1] == 1 && morse[2] == 1 && morse[3] == -1) return 'w';
  if (morse[0] == 1 && morse[1] == 0 && morse[2] == 0 && morse[3] == 1 && morse[4] == -1) return 'x';
  if (morse[0] == 1 && morse[1] == 0 && morse[2] == 1 && morse[3] == 1 && morse[4] == -1) return 'y';
  if (morse[0] == 1 && morse[1] == 1 && morse[2] == 0 && morse[3] == 0 && morse[4] == -1) return 'z';
  return '_';
}

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  //pinMode(3, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(0, knopka_out, FALLING);
  attachInterrupt(1, knopka_pull, RISING);
  Serial.begin(9600);
}

void knopka_pull()
{
  //Serial.println("knopka pull!");
  button_push = millis();
  flag_push = 1;
}

void knopka_out()
{
  //Serial.println("knopka out!");
  button_out = millis();
  flag_out = 1;
}

int type(int a, int b) // a>b - гаранитрованно
{
  if (abs(a - b) <= const_t * 2 - 1) //разница между коротким зажатием: const_t +- const -> [0; 2*const_t)
  {
    return 1;
  }
  else if ((abs(a - b) <= const_t * 4) && ((abs(a - b) >= const_t * 2))) //разница между длинным зажатием: 3*const_t +- const -> [2*const_t;4*const_t]
  {
    return 3;
  }
  else 
  {
    return 7;
  }
}

void loop() {

  if ((button_out - button_push > 0) && (flag_out == 1)) //значит кнопку только что отпустили (тут была только буква какая-то)
  {
    Serial.println("knopka ot_zazhata");
    //flag_push = 0;
    flag_out = 0;
    int result = type(button_out, button_push);
    switch (result)
    {
      case 1:
      {
        morse[index] = 0;
        index++;
        break;
      }
      case 3: 
      {
        morse[index] = 1;
        index++;
        break;
      }
      case 7:
      {
        //произошла какая-то фигня, лампочка гореть 7*const_t не может -> сбрасываем букву????
        //сбросить букву?
        break;
      }
    }
  }
  
  if ((button_push - button_out > 0) && (flag_push == 1)) //значит кнопку только что нажали (тут либо пробел между букв, либо пробел между слов)
  {
    Serial.println("knopka zazhata");
    //flag_out=0;
    flag_push=0;
    int result = type(button_out, button_push);
    switch (result)
    {
      case 1:
      {
        //index++; //тут или не тут делать пробел?
        break;
      }
      case 3:
      {
        
        for(int i = 0; i < 5; i++)
        {
          morse[i] = -1;
        }
        index = 0;
        break;
      }
    }
  }
  
}
