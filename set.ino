void set()
{
  int DONE = 0;
  Serial.println("Entering Servo set mode");
  char in = -1;
  long unsigned starttime = 0;
  while (DONE == 0)
  {
    char in = Serial.read();
    int DONEIN = 0;
    switch (in)
    {
      case 'q':
        DONE = 1;
        break;
      case 'p':
        Serial.println("Entering lever set mode");
        in = 0;
        Serial.println("Pull the lever to one max and press d");
        in = Serial.read();
        while (in != 'd')
        {
          in = Serial.read();
        }
        MAX = analogRead(A0);
        Serial.println("Pull the lever to min and press d");
        in = Serial.read();
        while (in != 'd')
        {
          in = Serial.read();
        }
        MIN = analogRead(A0);
        Serial.println("Pull the lever to other max and press d");
        in = Serial.read();
        while (in != 'd')
        {
          in = Serial.read();
        }
        MAX2 = analogRead(A0);
        EEPROM.put(addr_MIN, MIN);
        EEPROM.put(addr_MAX, MAX);
        if ((MAX > MIN && MAX2 > MIN) || (MAX < MIN && MAX2 < MIN))
        {
          MAX2 = MAX;
        }
        EEPROM.put(addr_MAX2, MAX2);
        Serial.println("Exiting lever set mode");
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
                      CREALMAX = analogRead(A1);
                      Serial.print("Current max is ");
                      Serial.print(CSERVOMAX);
                      Serial.println("ms");
                      Serial.print("Pos: ");
                      Serial.println(CREALMAX);
                      clutch.writeMicroseconds(CSERVOMAX);
                    }
                    break;
                  case '-':
                    if (CSERVOMAX > SERVOMINMIN)
                    {
                      CSERVOMAX--;
                      CREALMAX = analogRead(A1);
                      Serial.print("Current max is ");
                      Serial.print(CSERVOMAX);
                      Serial.println("ms");
                      Serial.print("Pos: ");
                      Serial.println(CREALMAX);
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
              EEPROM.put(addr_CREALMAX, CREALMAX);
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
                      CREALMIN = analogRead(A1);
                      Serial.print("Current min is ");
                      Serial.print(CSERVOMIN);
                      Serial.println("ms");
                      Serial.print("Pos: ");
                      Serial.println(CREALMIN);
                      clutch.writeMicroseconds(CSERVOMIN);
                    }
                    break;
                  case '-':
                    if (CSERVOMIN > SERVOMINMIN)
                    {
                      CSERVOMIN--;
                      CREALMIN = analogRead(A1);
                      Serial.print("Current min is ");
                      Serial.print(CSERVOMIN);
                      Serial.print("ms ");
                      Serial.print("Pos: ");
                      Serial.println(CREALMIN);
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
              EEPROM.put(addr_CREALMIN, CREALMIN);
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
                      SPOSGEARUP = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARUP);
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
                      SPOSGEARUP = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARUP);
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
                      SPOSGEARDOWN = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARDOWN);
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
                      SPOSGEARDOWN = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARDOWN);
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
                      SPOSGEARHALFUP = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARHALFUP);
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
                      SPOSGEARHALFUP = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARHALFUP);
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
                      SPOSGEARHALFDOWN = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARHALFDOWN);
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
                      SPOSGEARHALFDOWN = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARHALFDOWN);
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
                      SPOSGEARMID = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARMID);
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
                      SPOSGEARMID = analogRead(A2);
                      Serial.print("Pos: ");
                      Serial.println(SPOSGEARMID);
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
      case 'b':
        Serial.println("Entering blip set mode");
        while (DONEIN == 0)
        {
          int DONEIN2 = 0;
          in = Serial.read();
          switch (in)
          {
            case 'u' :
              Serial.print("Current cut time is ");
              Serial.print(UPBLIPTIME);
              Serial.println("ms");
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (UPBLIPTIME < 500)
                    {
                      UPBLIPTIME++;
                      Serial.print("Current cut time is ");
                      Serial.print(UPBLIPTIME);
                      Serial.println("ms");
                    }
                    break;
                  case '-':
                    if (UPBLIPTIME > 0)
                    {
                      UPBLIPTIME--;
                      Serial.print("Current cut time is ");
                      Serial.print(UPBLIPTIME);
                      Serial.println("ms");
                    }
                    break;
                  case 't':
                    starttime = millis();
                    while ((millis() - starttime) < UPBLIPTIME)
                    {
                      digitalWrite(8, 1);
                    }
                    digitalWrite(8, 0);
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
              EEPROM.put(addr_UPBLIPTIME, UPBLIPTIME);
              Serial.println("Exiting set cut time");
              break;
            case 'd' :
              Serial.print("Current blip time is ");
              Serial.print(DOWNBLIPTIME);
              Serial.println("ms");
              while (DONEIN2 == 0)
              {
                in = Serial.read();
                switch (in)
                {
                  case '+':
                    if (DOWNBLIPTIME < 500)
                    {
                      DOWNBLIPTIME++;
                      Serial.print("Current blip time is ");
                      Serial.print(DOWNBLIPTIME);
                      Serial.println("ms");
                    }
                    break;
                  case '-':
                    if (DOWNBLIPTIME > 0)
                    {
                      DOWNBLIPTIME--;
                      Serial.print("Current blip time is ");
                      Serial.print(DOWNBLIPTIME);
                      Serial.println("ms");
                    }
                    break;
                  case 't':
                    starttime = millis();
                    while ((millis() - starttime) < DOWNBLIPTIME)
                    {
                      digitalWrite(7, 1);
                    }
                    digitalWrite(7, 0);
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
              EEPROM.put(addr_DOWNBLIPTIME, DOWNBLIPTIME);
              Serial.println("Exiting set blip time");
              break;
            case 'q' :
              DONEIN = 1;
              break;
            case '!' :
              DONEIN = 1;
              DONE = 1;
              break;
          }
        }
        Serial.println("Exiting blip set mode");
        break;
    }

  }
  Serial.println("Exiting Servo set mode");
}
