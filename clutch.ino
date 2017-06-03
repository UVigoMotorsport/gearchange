//Function needs a voltage difference of at least 2.53 volts (519 steps) to work correctly I LIED
#include <Servo.h>
#include <EEPROM.h>
Servo clutch;
Servo gearchange;

unsigned int OFFSET = 10;
unsigned char WRITTEN = 0;
int addr_WRITTEN = OFFSET;
unsigned short int MIN = 0;
int addr_MIN = addr_WRITTEN + sizeof(WRITTEN);
unsigned short int MAX = 1023;
int addr_MAX = addr_MIN + sizeof(MIN);
//const unsigned short int DIVNUMS[3] = {512, 256, 128};
unsigned char DIVNUM = 0;
int addr_DIVNUM = addr_MAX + sizeof(MAX);
float DIV = 0;
//unsigned short int DIV = DIVNUMS[DIVNUM];
unsigned char FLAG = 0;
unsigned long int TIMELAST = 0;
unsigned char GEARCHG = 0;
unsigned long int GEARLAST = 0;
char GEARS[9] = {'N', '1', '1', '2', '3', '4', '5', '6'};
unsigned char GEAR = 0;
unsigned char OLDGEAR = 0;
unsigned char RELEASE = 0;
unsigned short int CLUTCH = MAX;
unsigned char CLUTCHSTART = 0;
unsigned long int CLUTCHLAST = 0;
unsigned long int CONFIGLAST = 0;
int CSERVOMAX = 1000;
int addr_CSERVOMAX = addr_DIVNUM + sizeof(DIVNUM);
int CSERVODEAD = 1700;
int addr_CSERVODEAD = addr_CSERVOMAX + sizeof(CSERVOMAX);
int CSERVOMIN = 2000;
int addr_CSERVOMIN = addr_CSERVODEAD + sizeof(CSERVODEAD);
float DEADPER = 0.05;
int addr_DEADPER = addr_CSERVOMIN + sizeof(CSERVOMIN);
int CREALMIN = 100;
int addr_CREALMIN = addr_CSERVOMIN + sizeof(DEADPER);
int CREALMAX = 400;
int addr_CREALMAX = addr_CREALMIN + sizeof(CREALMIN);

int SGEARMID = 1500;
int addr_SGEARMID = addr_CREALMAX + sizeof(CREALMAX);
int SGEARHALFDOWN = 1250;
int addr_SGEARHALFDOWN = addr_SGEARMID + sizeof(SGEARMID);
int SGEARHALFUP = 1750;
int addr_SGEARHALFUP = addr_SGEARHALFDOWN + sizeof(SGEARHALFDOWN);
int SGEARUP = 2000;
int addr_SGEARUP = addr_SGEARHALFUP + sizeof(SGEARHALFUP);
int SGEARDOWN = 1000;
int addr_SGEARDOWN = addr_SGEARUP + sizeof(SGEARUP);

int SPOSGEARMID = 200;
int addr_SPOSGEARMID = addr_SGEARDOWN + sizeof(SGEARDOWN);
int SPOSGEARHALFDOWN = 250;
int addr_SPOSGEARHALFDOWN = addr_SPOSGEARMID + sizeof(SPOSGEARMID);
int SPOSGEARHALFUP = 150;
int addr_SPOSGEARHALFUP = addr_SPOSGEARHALFDOWN + sizeof(SGEARHALFDOWN);
int SPOSGEARUP = 300;
int addr_SPOSGEARUP = addr_SPOSGEARHALFUP + sizeof(SPOSGEARUP);
int SPOSGEARDOWN = 100;
int addr_SPOSGEARDOWN = addr_SPOSGEARUP + sizeof(SPOSGEARUP);

int BADCHANGE = 0;
unsigned long CHANGESTART = 0;
int CHANGESTARTED = 0;
int CURRGEARPOS = 0;
int CHANGEPOINT = 0;
int CHANGEPOS = 0;

#define SERVOMAXMAX 2500
#define SERVOMINMIN 500

#define MAXDIV 10

#define EXPCHG 1
#define CIMINMAX 2
#define CSMINMAX 4
#define GEARUP 1
#define GEARDOWN 2
#define CHANGELIMIT 500

void (*reset) (void) = 0;

void setup()
{
  EEPROM.get(addr_WRITTEN, WRITTEN);
  if (WRITTEN != 1)
  {
    EEPROM.put(addr_MIN, MIN);
    EEPROM.put(addr_MAX, MAX);
    EEPROM.put(addr_DIVNUM, DIVNUM);
    EEPROM.put(addr_CSERVOMAX, CSERVOMAX);
    EEPROM.put(addr_CSERVODEAD, CSERVODEAD);
    EEPROM.put(addr_CSERVOMIN, CSERVOMIN);
    EEPROM.put(addr_DEADPER, DEADPER);
    EEPROM.put(addr_SGEARMID, SGEARMID);
    EEPROM.put(addr_SGEARHALFDOWN, SGEARHALFDOWN);
    EEPROM.put(addr_SGEARHALFUP, SGEARHALFUP);
    EEPROM.put(addr_SGEARUP, SGEARUP);
    EEPROM.put(addr_SGEARDOWN, SGEARDOWN);
    EEPROM.put(addr_SPOSGEARMID, SPOSGEARMID);
    EEPROM.put(addr_SPOSGEARHALFDOWN, SPOSGEARHALFDOWN);
    EEPROM.put(addr_SPOSGEARHALFUP, SPOSGEARHALFUP);
    EEPROM.put(addr_SPOSGEARUP, SPOSGEARUP);
    EEPROM.put(addr_SPOSGEARDOWN, SPOSGEARDOWN);
    WRITTEN = 1;
    EEPROM.put(addr_WRITTEN, WRITTEN);
  }
  EEPROM.get(addr_MIN, MIN);
  EEPROM.get(addr_MAX, MAX);
  CLUTCH = MAX;
  EEPROM.get(addr_DIVNUM, DIVNUM);
  DIV = (float) DIVNUM / 10.0;
  EEPROM.get(addr_CSERVOMAX, CSERVOMAX);
  EEPROM.get(addr_CSERVODEAD, CSERVODEAD);
  EEPROM.get(addr_CSERVOMIN, CSERVOMIN);
  EEPROM.get(addr_DEADPER, DEADPER);
  EEPROM.get(addr_SGEARMID, SGEARMID);
  EEPROM.get(addr_SGEARHALFDOWN, SGEARHALFDOWN);
  EEPROM.get(addr_SGEARHALFUP, SGEARHALFUP);
  EEPROM.get(addr_SGEARUP, SGEARUP);
  EEPROM.get(addr_SGEARDOWN, SGEARDOWN);
  EEPROM.get(addr_SPOSGEARMID, SPOSGEARMID);
  EEPROM.get(addr_SPOSGEARHALFDOWN, SPOSGEARHALFDOWN);
  EEPROM.get(addr_SPOSGEARHALFUP, SPOSGEARHALFUP);
  EEPROM.get(addr_SPOSGEARUP, SPOSGEARUP);
  EEPROM.get(addr_SPOSGEARDOWN, SPOSGEARDOWN);
  pinMode(12, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  Serial.begin(57600);
  Serial.println("hello");
  clutch.attach(9);
  clutch.writeMicroseconds(CSERVOMAX);
  gearchange.attach(6);
  gearchange.writeMicroseconds(SGEARMID);
  delay(250);
  if (digitalRead(12) == 1)
  {
    OLDGEAR = 0;
  }
  else
  {
    neutsearch();
  }
}

void loop()
{
  if ((millis() - GEARLAST) > 25)
  {
    if (!GEARCHG)
    {
      gearchange.writeMicroseconds(SGEARMID);
    }
    if (RELEASE)
    {
      if (!(PIND & ((1 << PORTD2) | (1 << PORTD3))))
      {
        RELEASE = 0;
      }
    }
    else if (PIND & (1 << PORTD2))
    {
      if (GEARCHG == 0)
      {
        GEARCHG = GEARUP;
        if (GEAR < 7)
        {
          switch (GEAR)
          {
            case 0 :
              GEAR = 2;
              break;
            case 1 :
              GEAR = 3;
              break;
            default :
              GEAR++;
          }
        }
      }
      RELEASE = 1;
    }
    else if (PIND & (1 << PORTD3))
    {
      if (GEARCHG == 0)
      {
        GEARCHG = GEARDOWN;
        if (GEAR > 0)
        {
          GEAR--;
        }
      }
      RELEASE = 1;
    }
    GEARLAST = millis();
  }

  if ((millis() - CLUTCHLAST > 30))
  {
    unsigned short int clutchpos = analogRead(A0);
    if (!CLUTCHSTART)
    {
      if (clutchpos >= MAX)
      {
        CLUTCHSTART = 1;
      }
    }
    else if(BADCHANGE)
    {
      CLUTCH = MAX;
      CLUTCHSTART = 0;
    }
    else
    {
      CLUTCH = clutchpos;
    }
    clutch.writeMicroseconds(exptoservo((float) CLUTCH));
    CLUTCHLAST = millis();
  }

  if (GEARCHG != 0)
  {
    CURRGEARPOS = analogRead(A2);
    if (GEARS[GEAR] == GEARS[OLDGEAR])
    {

      GEARCHG = 0;
    }
    else
    {
      if (!CHANGESTARTED)
      {
        if (GEARS[GEAR] == 'N' && GEARS[OLDGEAR] == '1')
        {
          CHANGEPOINT = SGEARHALFUP;
          CHANGEPOS = SPOSGEARHALFUP;
          CHANGESTARTED = 1;
        }
        else if (GEARS[OLDGEAR] > GEARS[GEAR])
        {
          CHANGEPOINT = SGEARDOWN;
          CHANGEPOS = SPOSGEARDOWN;
          CHANGESTARTED = 1;
        }
        else if (GEARS[OLDGEAR] < GEARS[GEAR])
        {
          CHANGEPOINT = SGEARUP;
          CHANGEPOS = SPOSGEARUP;
          CHANGESTARTED = 1;
        }
      }
      if (CHANGESTARTED == 1)
      {
        gearchange.writeMicroseconds(CHANGEPOINT);
        CHANGESTART = millis();
        CHANGESTARTED = 2;
      }
      if (CHANGESTARTED == 2)
      {
        CURRGEARPOS = analogRead(A2);
        if (CURRGEARPOS == CHANGEPOS || CURRGEARPOS == (CHANGEPOS + 1) || CURRGEARPOS == (CHANGEPOS - 1))
        {
          CHANGESTARTED = 3;
        }
        else
        {
          if ((millis() - CHANGESTART) > CHANGELIMIT)
          {
            CHANGESTARTED = 0;
            GEARCHG = 0;
            BADCHANGE = 1;
            gearchange.writeMicroseconds(SGEARMID);
            GEAR = OLDGEAR;
          }
        }
      }
      if (CHANGESTARTED == 3)
      {
        gearchange.writeMicroseconds(SGEARMID);
        CHANGESTART = millis();
        CHANGESTARTED = 4;
      }
      if (CHANGESTARTED == 4)
      {
        CURRGEARPOS = analogRead(A2);
        if (CURRGEARPOS == SPOSGEARMID || CURRGEARPOS == (SPOSGEARMID + 1) || CURRGEARPOS == (SPOSGEARMID - 1))
        {
          CHANGESTARTED = 0;
          OLDGEAR = GEAR;
          GEARCHG = 0;
          if (GEARS[GEAR] == 'N' && digitalRead(12) == 1)
          {
            BADCHANGE = 0;
          }
        }
        else
        {
          if ((millis() - CHANGESTART) > CHANGELIMIT)
          {
            CHANGESTARTED = 0;
            GEARCHG = 0;
            BADCHANGE = 1;
            gearchange.writeMicroseconds(SGEARMID);
            GEAR = OLDGEAR;
          }
        }
      }
    }
  }

  if ((millis() - TIMELAST) > 750)
  {
    TIMELAST = millis();
    Serial.print("Clutch: ");
    int clutchposnow = exptoservo((float) CLUTCH);
    int clutchrealpos =  analogRead(A1);
    if (clutchposnow == CSERVOMAX)
    {
      CREALMAX = clutchrealpos;
      int oldmax = 0;
      EEPROM.get(addr_CREALMAX, oldmax);
      if (CREALMAX > oldmax + 1 || CREALMAX < oldmax - 1)
      {
        EEPROM.put(addr_CREALMAX, CREALMAX);
      }
    }
    if (clutchposnow == CSERVOMIN)
    {
      CREALMIN = clutchrealpos;
      int oldmin = 0;
      EEPROM.get(addr_CREALMIN, oldmin);
      if (CREALMIN > oldmin + 1 || CREALMIN < oldmin - 1)
      {
        EEPROM.put(addr_CREALMIN, CREALMIN);
      }
    }
    Serial.println((clutchposnow - CSERVOMIN) * (1.0 / (CSERVOMAX - CSERVOMIN)));
    Serial.print("Clutch real pos: ");
    Serial.println((clutchrealpos - CREALMIN) * (1.0 / (CREALMAX - CREALMIN)));
    Serial.print("Gear: [");
    if (GEARCHG == 0)
    {
      Serial.print(GEARS[GEAR]);
    }
    else
    {
      Serial.print("-");
    }
    if (BADCHANGE == 1)
    {
      Serial.print("X");
    }
    Serial.println("]");
    Serial.print("Percent exp: ");
    Serial.print(DIV * 100.0);
    Serial.println("%");
    Serial.println("");
  }
  if ((millis() - CONFIGLAST) > 100)
  {
    CONFIGLAST = millis();
    char in = Serial.read();
    char inner = 0;
    if (in == '+' || in == '-')
    {
      if (FLAG == EXPCHG)
      {
        in = 'e';
      }
      else if (FLAG == CIMINMAX)
      {
        in = 'c';
      }
    }
    switch (in)
    {
      case 'r':
        reset();
        break;
      case 'n':
        BADCHANGE = 0;
        GEAR = 0;
        OLDGEAR = 0;
        CLUTCHSTART = 0;
        CLUTCH = MAX;
        delay(CHANGELIMIT);
        clutch.writeMicroseconds(exptoservo((float) CLUTCH));
        delay(CHANGELIMIT);
        gearchange.writeMicroseconds(SGEARMID);
        delay(CHANGELIMIT);
        Serial.println("Put the gearbox in neutral");
        while(digitalRead(12) != 1);
        delay(CHANGELIMIT);
        break;
      case 'e' :
        if (FLAG == EXPCHG)
        {
          inner = in;
        }
        else
        {
          inner = Serial.read();
        }
        FLAG = EXPCHG;
        switch (inner)
        {
          case '+' :
            if (DIVNUM < MAXDIV)
            {
              DIVNUM++;
              //DIV = DIVNUMS[DIVNUM];
              DIV = (float) DIVNUM / 10.0;
              EEPROM.put(addr_DIVNUM, DIVNUM);
            }
            FLAG = 0;
            break;
          case '-' :
            if (DIVNUM > 0)
            {
              DIVNUM--;
              //DIV = DIVNUMS[DIVNUM];
              DIV = (float) DIVNUM / 10.0;
              EEPROM.put(addr_DIVNUM, DIVNUM);
            }
            FLAG = 0;
            break;
          case -1  :
            break;
          default :
            FLAG = 0;
        }
        break;
      case 'c' :
        if (FLAG == CIMINMAX)
        {
          inner = in;
        }
        else
        {
          inner = Serial.read();
        }
        FLAG = CIMINMAX;
        switch (inner)
        {
          case '+' :
            CLUTCH = analogRead(A0);
            if (CLUTCH > MIN)
            {
              MAX = CLUTCH;
              EEPROM.put(addr_MAX, MAX);
            }
            FLAG = 0;
            break;
          case '-' :
            CLUTCH = analogRead(A0);
            if (CLUTCH < MAX)
            {
              MIN = CLUTCH;
              EEPROM.put(addr_MIN, MIN);
            }
            FLAG = 0;
            break;
          case -1  :
            break;
          default  :
            FLAG = 0;
        }
        break;
      case '!':
        cleardats();
      case 's' :
        set();
      case -1:
        break;
      default :
        FLAG = 0;
    }
  }

}

int exptoservo(float x)
{
  x = ((float) (x - MIN) * (1023.0 / (float) (MAX - MIN)) / 1023.0);

  if (x > DEADPER)
  {
    float linx = (x - DEADPER) * (1.0 / (1.0 - DEADPER));
    float frac = ((1 - DIV) * linx) + ((DIV) * exp(linx - 1));
    return frac * (CSERVOMAX - CSERVODEAD) + CSERVODEAD;
  }
  else
  {
    return CSERVOMIN;
  }
}
void set()
{
  int DONE = 0;
  Serial.println("Entering Servo set mode");
  char in = -1;
  while (DONE == 0)
  {
    char in = Serial.read();
    int DONEIN = 0;
    switch (in)
    {
      case 'q':
        DONE = 1;
        break;
      case 'c':
        Serial.println("Entering clutch set mode");
        while (DONEIN == 0)
        {
          int DONEIN2 = 0;
          in = Serial.read();
          switch (in)
          {
            case '+':
              Serial.print("Current max is ");
              Serial.print(CSERVOMAX);
              Serial.println("ms");
              clutch.writeMicroseconds(CSERVOMAX);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (CSERVOMAX < SERVOMAXMAX)
                    {
                      CSERVOMAX++;
                      Serial.print("Current max is ");
                      Serial.print(CSERVOMAX);
                      Serial.println("ms");
                      clutch.writeMicroseconds(CSERVOMAX);
                    }
                    break;
                  case '-':
                    if (CSERVOMAX > SERVOMINMIN)
                    {
                      CSERVOMAX--;
                      Serial.print("Current max is ");
                      Serial.print(CSERVOMAX);
                      Serial.println("ms");
                      clutch.writeMicroseconds(CSERVOMAX);
                    }
                    break;
                  case 'q':

                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_CSERVOMAX, CSERVOMAX);
              Serial.println("Exiting set max");
              break;
            case '-':
              Serial.print("Current min is ");
              Serial.print(CSERVOMIN);
              Serial.println("ms");
              clutch.writeMicroseconds(CSERVOMIN);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (CSERVOMIN < SERVOMAXMAX)
                    {
                      CSERVOMIN++;
                      Serial.print("Current min is ");
                      Serial.print(CSERVOMIN);
                      Serial.println("ms");
                      clutch.writeMicroseconds(CSERVOMIN);
                    }
                    break;
                  case '-':
                    if (CSERVOMIN > SERVOMINMIN)
                    {
                      CSERVOMIN--;
                      Serial.print("Current min is ");
                      Serial.print(CSERVOMIN);
                      Serial.println("ms");
                      clutch.writeMicroseconds(CSERVOMIN);
                    }
                    break;
                  case 'q':
                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_CSERVOMIN, CSERVOMIN);
              Serial.println("Exiting set min");
              break;
            case 'd':
              Serial.print("Current dead is ");
              Serial.print(CSERVODEAD);
              Serial.println("ms");
              clutch.writeMicroseconds(CSERVODEAD);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (CSERVODEAD < SERVOMAXMAX)
                    {
                      CSERVODEAD++;
                      Serial.print("Current dead is ");
                      Serial.print(CSERVODEAD);
                      Serial.println("ms");
                      clutch.writeMicroseconds(CSERVODEAD);
                    }
                    break;
                  case '-':
                    if (CSERVODEAD > SERVOMINMIN)
                    {
                      CSERVODEAD--;
                      Serial.print("Current dead is ");
                      Serial.print(CSERVODEAD);
                      Serial.println("ms");
                      clutch.writeMicroseconds(CSERVODEAD);
                    }
                    break;
                  case 'q':
                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_CSERVODEAD, CSERVODEAD);
              Serial.println("Exiting set dead");
              break;
            case 'q':
              DONEIN = 1;
              break;
            case '!':
              DONEIN = 1;
              DONE = 1;
              break;
          }
        }
        Serial.println("Exiting clutch set mode");
        break;
      case 'g':
        Serial.println("Entering gearchange set mode");
        while (DONEIN == 0)
        {
          int DONEIN2 = 0;
          in = Serial.read();
          switch (in)
          {
            case '+':
              Serial.print("Current upchange is ");
              Serial.print(SGEARUP);
              Serial.println("ms");
              gearchange.writeMicroseconds(SGEARUP);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (SGEARUP < SERVOMAXMAX)
                    {
                      SGEARUP++;
                      Serial.print("Current upchange is ");
                      Serial.print(SGEARUP);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARUP);
                    }
                    break;
                  case '-':
                    if (SGEARUP > SERVOMINMIN)
                    {
                      SGEARUP--;
                      Serial.print("Current upchange is ");
                      Serial.print(SGEARUP);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARUP);
                    }
                    break;
                  case 'q':
                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_SGEARUP, SGEARUP);
              SPOSGEARUP = analogRead(A2);
              EEPROM.put(addr_SPOSGEARUP, SPOSGEARUP);
              Serial.println("Exiting set upchange");
              break;
            case '-':
              Serial.print("Current downchange is ");
              Serial.print(SGEARDOWN);
              Serial.println("ms");
              gearchange.writeMicroseconds(SGEARDOWN);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (SGEARDOWN < SERVOMAXMAX)
                    {
                      SGEARDOWN++;
                      Serial.print("Current downchange is ");
                      Serial.print(SGEARDOWN);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARDOWN);
                    }
                    break;
                  case '-':
                    if (SGEARDOWN > SERVOMINMIN)
                    {
                      SGEARDOWN--;
                      Serial.print("Current downchange is ");
                      Serial.print(SGEARDOWN);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARDOWN);
                    }
                    break;
                  case 'q':
                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_SGEARDOWN, SGEARDOWN);
              SPOSGEARDOWN = analogRead(A2);
              EEPROM.put(addr_SPOSGEARDOWN, SPOSGEARDOWN);
              Serial.println("Exiting set downchange");
              break;
            case 'u':
              Serial.print("Current halfup is ");
              Serial.print(SGEARHALFUP);
              Serial.println("ms");
              gearchange.writeMicroseconds(SGEARHALFUP);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (SGEARHALFUP < SERVOMAXMAX)
                    {
                      SGEARHALFUP++;
                      Serial.print("Current halfup is ");
                      Serial.print(SGEARHALFUP);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARHALFUP);
                    }
                    break;
                  case '-':
                    if (SGEARHALFUP > SERVOMINMIN)
                    {
                      SGEARHALFUP--;
                      Serial.print("Current halfup is ");
                      Serial.print(SGEARHALFUP);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARHALFUP);
                    }
                    break;
                  case 'q':
                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_SGEARHALFUP, SGEARHALFUP);
              SPOSGEARHALFUP = analogRead(A2);
              EEPROM.put(addr_SPOSGEARHALFUP, SPOSGEARHALFUP);
              Serial.println("Exiting set halfup");
              break;
            case 'd':
              Serial.print("Current halfdown is ");
              Serial.print(SGEARHALFDOWN);
              Serial.println("ms");
              gearchange.writeMicroseconds(SGEARHALFDOWN);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (SGEARHALFDOWN < SERVOMAXMAX)
                    {
                      SGEARHALFDOWN++;
                      Serial.print("Current halfdown is ");
                      Serial.print(SGEARHALFDOWN);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARHALFDOWN);
                    }
                    break;
                  case '-':
                    if (SGEARHALFDOWN > SERVOMINMIN)
                    {
                      SGEARHALFDOWN--;
                      Serial.print("Current halfdown is ");
                      Serial.print(SGEARHALFDOWN);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARHALFDOWN);
                    }
                    break;
                  case 'q':
                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_SGEARHALFDOWN, SGEARHALFDOWN);
              SPOSGEARHALFDOWN = analogRead(A2);
              EEPROM.put(addr_SPOSGEARHALFDOWN, SPOSGEARHALFDOWN);
              Serial.println("Exiting set halfdown");
              break;
            case 'm':
              Serial.print("Current midpoint is ");
              Serial.print(SGEARMID);
              Serial.println("ms");
              gearchange.writeMicroseconds(SGEARMID);
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (SGEARMID < SERVOMAXMAX)
                    {
                      SGEARMID++;
                      Serial.print("Current midpoint is ");
                      Serial.print(SGEARMID);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARMID);
                    }
                    break;
                  case '-':
                    if (SGEARMID > SERVOMINMIN)
                    {
                      SGEARMID--;
                      Serial.print("Current midpoint is ");
                      Serial.print(SGEARMID);
                      Serial.println("ms");
                      gearchange.writeMicroseconds(SGEARMID);
                    }
                    break;
                  case 'q':
                    DONEIN2 = 1;
                    break;
                  case '!':
                    DONEIN2 = 1;
                    DONEIN = 1;
                    DONE = 1;
                    break;
                }
              }
              EEPROM.put(addr_SGEARMID, SGEARMID);
              SPOSGEARMID = analogRead(A2);
              EEPROM.put(addr_SPOSGEARMID, SPOSGEARMID);
              Serial.println("Exiting set midpoint");
              break;
            case 'q':
              DONEIN = 1;
              break;
            case '!':
              DONEIN = 1;
              DONE = 1;
              break;
          }
        }
        Serial.println("Exiting gearchange set mode");
        break;
    }

  }
  Serial.println("Exiting Servo set mode");
}

void cleardats()
{
  Serial.println("Please confirm you want to erase stored data");
  while (Serial.available() == 0)
  {
    delay(10);
  }
  char in = Serial.read();
  if (in == '!')
  {
    EEPROM.put(addr_WRITTEN, 0);
    Serial.println("Done");
    reset();
  }
  return;

}

void neutsearch()
{
  gearchange.writeMicroseconds(SGEARMID);
  Serial.print("Car not started in neutral");
  int DONE = 0;
  CHANGESTART = millis();
  while (millis() - CHANGESTART < 5000)
  {
    clutch.writeMicroseconds(CSERVOMAX);
    if (Serial.read() == '!')
    {
      gearchange.writeMicroseconds(SGEARMID);
      Serial.print("Put the car in neutral");
      while (!digitalRead(12));
      DONE = 1;
      break;
    }
    if (digitalRead(12) == 1)
    {
      DONE = 1;
      break;
    }
  }
  if (DONE == 0)
  {
    Serial.println("... searching...");
  }
  while (!DONE)
  {
    CHANGESTART = millis();
    while (millis() - CHANGESTART < 750)
    {
      if (Serial.read() == '!')
      {
        gearchange.writeMicroseconds(SGEARMID);
        Serial.print("Put the car in neutral");
        while (!digitalRead(12));
        DONE = 1;
        break;
      }
      if (digitalRead(12) == 1)
      {
        DONE = 1;
        break;
      }
    }
    if (DONE == 1)
    {
      break;
    }
    CHANGESTART = millis();
    gearchange.writeMicroseconds(SGEARHALFUP);
    while (millis() - CHANGESTART < CHANGELIMIT)
    {
      if (Serial.read() == '!')
      {
        gearchange.writeMicroseconds(SGEARMID);
        Serial.print("Put the car in neutral");
        while (!digitalRead(12));
        DONE = 1;
        break;
      }
      CURRGEARPOS = analogRead(A2);
      if (CURRGEARPOS == SPOSGEARHALFUP || CURRGEARPOS == (SPOSGEARHALFUP + 1) || CURRGEARPOS == (SPOSGEARHALFUP - 1))
      {
        if (digitalRead(12) == 1)
        {
          DONE = 1;
        }
        break;
      }
    }
    if (DONE == 1)
    {
      break;
    }
    CHANGESTART = millis();
    gearchange.writeMicroseconds(SGEARMID);
    while (millis() - CHANGESTART < CHANGELIMIT)
    {
      CURRGEARPOS = analogRead(A2);
      if (CURRGEARPOS == SPOSGEARMID || CURRGEARPOS == (SPOSGEARMID + 1) || CURRGEARPOS == (SPOSGEARMID - 1))
      {
        if (digitalRead(12) == 1)
        {
          DONE = 1;
        }
        break;
      }
    }
    if (DONE == 1)
    {
      break;
    }
    CHANGESTART = millis();
    while (millis() - CHANGESTART < 750)
    {
      if (Serial.read() == '!')
      {
        gearchange.writeMicroseconds(SGEARMID);
        Serial.print("Put the car in neutral");
        while (!digitalRead(12));
        DONE = 1;
        break;
      }
      if (digitalRead(12) == 1)
      {
        DONE = 1;
        break;
      }
    }
    if (DONE == 1)
    {
      break;
    }
    CHANGESTART = millis();
    gearchange.writeMicroseconds(SGEARDOWN);
    while (millis() - CHANGESTART < CHANGELIMIT)
    {
      if (Serial.read() == '!')
      {
        gearchange.writeMicroseconds(SGEARMID);
        Serial.print("Put the car in neutral");
        while (!digitalRead(12));
        DONE = 1;
        break;
      }
      CURRGEARPOS = analogRead(A2);
      if (CURRGEARPOS == SPOSGEARDOWN || CURRGEARPOS == (SPOSGEARDOWN + 1) || CURRGEARPOS == (SPOSGEARDOWN - 1))
      {
        if (digitalRead(12) == 1)
        {
          DONE = 1;
        }
        break;
      }
    }
    if (DONE == 1)
    {
      break;
    }
    CHANGESTART = millis();
    gearchange.writeMicroseconds(SGEARMID);
    while (millis() - CHANGESTART < CHANGELIMIT)
    {
      CURRGEARPOS = analogRead(A2);
      if (CURRGEARPOS == SPOSGEARMID || CURRGEARPOS == (SPOSGEARMID + 1) || CURRGEARPOS == (SPOSGEARMID - 1))
      {
        if (digitalRead(12) == 1)
        {
          DONE = 1;
        }
        break;
      }
    }
    if (DONE == 1)
    {
      break;
    }
  }
  gearchange.writeMicroseconds(SGEARMID);
  delay(CHANGELIMIT);
  GEAR = 0;
  OLDGEAR = 0;
}

