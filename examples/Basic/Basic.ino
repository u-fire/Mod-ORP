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
  // start Serial and I2C
  Serial.begin(9600);
  Wire.begin();

  // start the sensor
  orp.begin();
}

void loop()
{
  // take an ORP measurement
  orp.measureORP();

  // display the results
  Serial.println((String) orp.mV + " mV");
  Serial.println();

  delay(1000);
}