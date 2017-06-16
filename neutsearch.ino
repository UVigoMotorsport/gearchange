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
