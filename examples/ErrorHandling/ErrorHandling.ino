/*!
   ufire.dev for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   Mod-ORP hardware version 1, firmware 1
*/

#include <uFire_Mod-ORP.h>
uFire::Mod_ORP::i2c orp;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  if (orp.begin() != true)
  {
    Serial.println("Error: the sensor isn't connected");
    while (1);
  }
}

void loop()
{
  // get the temperature of the solution
  orp.measureTemp();

  // check for errors
  if (orp.status)
  {
    Serial.println("Error:");
    switch (orp.status)
    {
      case orp.STATUS_SYSTEM_ERROR:
        Serial.println("  temperature sensor not connected");
        break;
    }
  }

  // take a pH measurement
  orp.measureORP();
  switch (orp.status)
  {
    case orp.STATUS_SYSTEM_ERROR:
      Serial.println("  Error: Module not functioning properly");
      break;
    case orp.STATUS_NO_ERROR:
      Serial.print(orp.mV, 3);
      Serial.println((String)" mV @ " + orp.tempC + "°C");
      Serial.println();
      break;
  }

  delay(1000);
}