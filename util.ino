int exptoservo(float x)
{
  if (x < (DEADPER * 1023.0))
  {
    return CSERVOMIN;
  }
  else if (x < 1023.0 - (DEADPER * 1023.0))
  {
    float linx = x * (1.0 + (float) DEADPER);
    linx /= 1023.0;
    float expn = (-1.0/exp(linx*EXPFACTOR)+1.0)/(1.0-1.0/exp(EXPFACTOR));
    float frac = ((1 - DIV) * linx) + ((DIV) * expn);
    return frac * (CSERVOMAX - CSERVODEAD) + CSERVODEAD;
  }
  else
  {
    return CSERVOMAX;
  }
}

int tocfake(int x)
{
  int chosenmax = 0;
  if (MAX > MAX2 && MIN < MAX && MIN > MAX2)
  {
    if (x > MIN)
    {
      chosenmax = MAX;
    }
    else if (x < MIN)
    {
      chosenmax = MAX2;
    }
    else
    {
      return CLUTCHFAKEMIN;
    }
  }
  else if (MAX < MAX2 && MIN > MAX && MIN < MAX2)
  {
    if (x < MIN)
    {
      chosenmax = MAX;
    }
    else if (x > MIN)
    {
      chosenmax = MAX2;
    }
    else
    {
      return CLUTCHFAKEMIN;
    }
  }
  else if (MAX == MAX2)
  {
    if (MIN != MAX)
    {
      chosenmax = MAX;
    }
    else
    {
      return CLUTCHFAKEMAX;
    }
  }
  else
  {
    return CLUTCHFAKEMAX;
  }
  return ((float) x - (float) MIN) * (1023.0 / ((float)chosenmax - (float)MIN));
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
