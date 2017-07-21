#include "gearchange.h"

void loop()
{
  if ((millis() - GEARLAST) > 25 && (!CHANGESTAGE))
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
        if (GEAR < 7)
        {
          GEARCHG = GEARUP;
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
          if (AUTO)
          {
            CHANGESTAGE++;
            if (OLDGEAR != 0)
            {
              BLIPTYPE = CUT;
              BLIPTIME = UPBLIPTIME;
              digitalWrite(8, 1);
            }
            else
            {
              BLIPTYPE = NOBLIP;
              digitalWrite(8, 1);
            }
          }
        }
      }
      RELEASE = 1;
    }
    else if (PIND & (1 << PORTD3))
    {
      if (GEARCHG == 0)
      {
        if (GEAR > 0)
        {
          GEARCHG = GEARDOWN;
          GEAR--;
          if (AUTO)
          {
            CHANGESTAGE++;
            if (GEAR == 0)
            {
              BLIPTYPE = NOBLIP;
            }
            else if (GEAR == 1)
            {
              GEARCHG = 0;
            }
            else
            {
              BLIPTYPE = BLIP;
              BLIPTIME = DOWNBLIPTIME;
            }
          }
          if (GEAR == 1)
          {
            OLDGEAR = 1;
            CHANGESTAGE = 0;
            LASTNEUTSAVE = millis();
          }
        }
      }
      RELEASE = 1;
    }
    GEARLAST = millis();
    if (GEAR == 1 && ((millis() - LASTNEUTSAVE) >= NEUTSAVEMAX))
    {
      GEAR++;
      OLDGEAR++;
    }
  }

  if (AUTO && (CHANGESTAGE == 3))
  {
    if (BLIPTYPE == CUT)
    {
      digitalWrite(8, 1);
      CHANGESTAGE++;
      BLIPDUR = millis();
    }
    else if (BLIPTYPE == BLIP)
    {
      digitalWrite(7, 1);
      CHANGESTAGE++;
      BLIPDUR = millis();
    }
    else if (BLIPTYPE == NOBLIP)
    {
      CHANGESTAGE = 5;
    }
    else
    {
      CHANGESTAGE = 0;
    }
  }

  if (AUTO && (CHANGESTAGE == 4))
  {
    if ((millis() - BLIPDUR) >= BLIPTIME)
    {
      digitalWrite(8, 0);
      digitalWrite(7, 0);
      CHANGESTAGE++;
    }
  }

  if ((millis() - CLUTCHLAST > 30) || CHANGESTAGE == 1 || CHANGESTAGE == 5)
  {
    CLUTCHPOS = tocfake(analogRead(A0));
    CLUTCHREALPOS = analogRead(A1);
    if (!CLUTCHSTART)
    {
      if (CLUTCHPOS >= CLUTCHFAKEMAX)
      {
        CLUTCHSTART = 1;
        if (WANTAUTO == 1)
        {
          AUTO = 1;
        }
      }
      else if (CHANGESTAGE)
      {
        CHANGESTAGE = 0;
        GEARCHG = 0;
      }
    }
    else if (BADCHANGE)
    {
      CLUTCH = CLUTCHFAKEMAX;
      CLUTCHSTART = 0;
    }
    else if (CHANGESTAGE == 1)
    {
      CLUTCH = CLUTCHFAKEMAX;
      CLUTCHREALPOS = analogRead(A1);
      if (CLUTCHREALPOS <= CREALMAX + FUDGEPOS && CLUTCHREALPOS >= CREALMAX - FUDGEPOS)
      {
        CHANGESTAGE++;
      }
    }
    else if (CHANGESTAGE == 5)
    {
      if (BLIPTYPE != NOBLIP)
      {
        CLUTCH = MIN;
      }
      CLUTCHREALPOS = analogRead(A1);
      if ((CLUTCHREALPOS <= CREALMIN + FUDGEPOS && CLUTCHREALPOS >= CREALMIN - FUDGEPOS) || BLIPTYPE == NOBLIP)
      {
        CHANGESTAGE = 0;
      }
    }
    else if (CHANGESTAGE == 0)
    {
      CLUTCH = CLUTCHPOS;
    }
    clutch.writeMicroseconds(exptoservo((float) CLUTCH));
    CLUTCHLAST = millis();
  }

  if (GEARCHG != 0 && !AUTO || CHANGESTAGE == 2 && AUTO)
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
        Serial.println(CURRGEARPOS);
        if ((CURRGEARPOS <= CHANGEPOS + FUDGEPOS && CURRGEARPOS >= CHANGEPOS - FUDGEPOS)  && (millis() - CHANGESTART > MINCHANGE))
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
            if (CHANGESTAGE)
            {
              CHANGESTAGE = 0;
            }
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
        Serial.println(CURRGEARPOS);
        if ((CURRGEARPOS <= SPOSGEARMID + FUDGEPOS && CURRGEARPOS >= SPOSGEARMID - FUDGEPOS) && (millis() - CHANGESTART > MINCHANGE))
        {
          CHANGESTARTED = 0;
          OLDGEAR = GEAR;
          GEARCHG = 0;
          if (GEARS[GEAR] == 'N' && digitalRead(12) == 1)
          {
            BADCHANGE = 0;
          }
          if (CHANGESTAGE)
          {
            CHANGESTAGE++;
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
            if (CHANGESTAGE)
            {
              CHANGESTAGE = 0;
            }
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
    Serial.print(clutchposnow);
    Serial.print("|");
    Serial.println(analogRead(A0));
    Serial.print(MAX);
    Serial.print("|");
    Serial.print(MIN);
    Serial.print("|");
    Serial.println(MAX2);
    Serial.print("Clutch real pos: ");
    Serial.println(CLUTCHREALPOS);
    Serial.print("Gear: [");
    Serial.print(GEARS[GEAR]);
    if (GEARCHG == GEARUP)
    {
      Serial.print("/\\");
    }
    else if (GEARCHG == GEARDOWN)
    {
      Serial.print("\\/");
    }
    else if (GEARCHG == 0)
    {
      Serial.print("-");
    }
    else
    {
      Serial.print("X");
    }
    Serial.print(GEARS[OLDGEAR]);
    if (BADCHANGE == 1)
    {
      Serial.print("X");
    }
    Serial.println("]");
    Serial.print("Percent exp: ");
    Serial.print(DIV * 100.0);
    Serial.println("%");
    Serial.print("Auto: ");
    Serial.println(AUTO);
    Serial.print("Changestage: ");
    Serial.println(CHANGESTAGE);
    Serial.println("");

  }
  if ((millis() - CONFIGLAST) > 100)
  {
    CONFIGLAST = millis();
    char in = Serial.read();
    char inner = 0;
    switch (in)
    {
      case 'a':
        if (AUTO < AUTOMAX)
        {
          AUTO++;
          if (!CLUTCHSTART)
          {
            AUTO--;
            WANTAUTO = AUTO;
          }
        }
        break;
      case 'd':
        if (AUTO > 0)
        {
          AUTO--;
          WANTAUTO = 0;
        }
        break;
      case 'r':
        reset();
        break;
      case 'n':
        BADCHANGE = 0;
        GEAR = 0;
        OLDGEAR = 0;
        CLUTCHSTART = 0;
        CLUTCH = CLUTCHFAKEMAX;
        delay(CHANGELIMIT);
        clutch.writeMicroseconds(exptoservo((float) CLUTCH));
        delay(CHANGELIMIT);
        gearchange.writeMicroseconds(SGEARMID);
        delay(CHANGELIMIT);
        Serial.println("Put the gearbox in neutral");
        while (digitalRead(12) != 1);
        delay(CHANGELIMIT);
        break;
      case 'e' :
        if (DIVNUM < MAXDIV)
        {
          DIVNUM++;
          DIV = (float) DIVNUM / 10.0;
          EEPROM.put(addr_DIVNUM, DIVNUM);
        }
        break;
      case 'q' :
        if (DIVNUM > 0)
        {
          DIVNUM--;
          DIV = (float) DIVNUM / 10.0;
          EEPROM.put(addr_DIVNUM, DIVNUM);
        }
        break;
      case 't':
        if (DEADPER < 1.0)
        {
          DEADPER += .01;
          Serial.print("Dead % is: ");
          Serial.println(DEADPER);
        }
        EEPROM.put(addr_DEADPER, DEADPER);
        break;
      case 'y':
        if (DEADPER > 0.0)
        {
          DEADPER -= .01;
          Serial.print("Dead % is: ");
          Serial.println(DEADPER);
        }
        EEPROM.put(addr_DEADPER, DEADPER);
        break;
      case 'o':
        if (EXPFACTOR < 10)
        {
          EXPFACTOR += .1;
          Serial.print("exp factor is: ");
          Serial.println(EXPFACTOR);
        }
        EEPROM.put(addr_EXPFACTOR, EXPFACTOR);
        break;
      case 'p':
        if (EXPFACTOR > 1)
        {
          EXPFACTOR -= .1;
          Serial.print("exp factor is: ");
          Serial.println(EXPFACTOR);
        }
        EEPROM.put(addr_EXPFACTOR, EXPFACTOR);
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

