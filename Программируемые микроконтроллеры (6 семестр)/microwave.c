int time = 0;
int prev_millis = 0;
int check_finish_time = 2;
int open_close; //1 - open, 0 - close
int interval_print = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(2, INPUT);
    pinMode(4, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, INPUT);
    attachInterrupt(0, press_button, RISING);
}

void press_button()
{
    time += 3000;
    check_finish_time = 0;
    Serial.println("Press button +3 seconds");
}

void loop()
{
    //Открыта или закрыта дверь
    open_close = digitalRead(13);
    if (open_close == 1)
    {
        digitalWrite(4, HIGH);
        //Serial.println("Door opened");
    }
    else
    {
        digitalWrite(4, LOW);
        //Serial.println("Door closed");
    }

    int mil = millis();
    //Вычитание времени
    if (open_close == 0)
    {
        time -= mil - prev_millis;
        if (time < 0)
        {
            check_finish_time++;
            time = 0;
        }
    }
    prev_millis = mil;

    //Вывод времени с интервалом
    if (mil > interval_print + 1000)
    {
        Serial.println(time);
        interval_print = mil;
    }

    //Светодиод горит во время разогрева чего-либо
    if ((time > 0) && (open_close == 0))
    {
        digitalWrite(7, HIGH);
    }
    else
    {
        digitalWrite(7, LOW);
    }

    //Звуковой сигнал после готовки
    if (check_finish_time == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            tone(8, 1500);
            delay(200);
            noTone(8);
            delay(200);
        }
        check_finish_time++;
    }
}