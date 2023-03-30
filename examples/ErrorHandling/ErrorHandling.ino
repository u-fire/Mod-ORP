/*!
   microfire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@ufire.co to get in touch with someone

   Mod-ORP hardware version 2, firmware 1
*/

#include <Microfire_Mod-ORP.h>
Microfire::Mod_ORP::i2c orp;

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
  // take a pH measurement
  orp.measureORP();
  switch (orp.status)
  {
    case orp.STATUS_SYSTEM_ERROR:
      Serial.println("  Error: Module not functioning properly");
      break;
    case orp.STATUS_NO_ERROR:
      Serial.print(orp.mV);
      Serial.println((String)" mV @ " + orp.tempC + "°C");
      break;
  }

  delay(1000);
}