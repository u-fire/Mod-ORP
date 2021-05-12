/*
   This allows you to run various functions in a command-line like interface.
   Enter:
   `config` to see the configuration of the device.
   'reset' to reset all the configuration stored in the device
   
   Measure ORP:
     `orp`

   Single Point Calibration:
    sin <calibration solution in pH> <solution temp [25]>
      `sin 60.0 t` - Calibrate at 600.0 mV

   Measure Temperature:
    temp

   Change the I2C address:
    i2c 0x0F
*/

#include <Arduino.h>
#include <Wire.h>
#include "uFire_Mod-ORP.h"

uFire::Mod_ORP::i2c orp;

String buffer, cmd, p1, p2;
const int fw_compatible = 1;
const int hw_compatible = 1;

void config()
{
  orp.getDeviceInfo();
  Serial.println((String) "uFire Mod-ORP Sensor: " + (orp.connected() ? "connected" : "*disconnected*"));
  if (!orp.connected()) return;
  if ((orp.fwVersion != fw_compatible) || (orp.hwVersion != hw_compatible))
  {
    Serial.println("*This version of shell was designed for a different hardware revision or firmware version*");
  }
  Serial.println("calibration:");
  Serial.print("    single point: ");
  if (orp.calibrationSingleOffset == orp.calibrationSingleOffset ? Serial.println(orp.calibrationSingleOffset, 3) : Serial.println("-"));
  Serial.print("hardware:firmware version: ");
  Serial.print(orp.hwVersion);
  Serial.print(":");
  Serial.println(orp.fwVersion);
}

void config_reset()
{
  orp.reset();
  config();
}

void temperature()
{
  orp.measureTemp();
  switch (orp.status)
  {
    case orp.STATUS_SYSTEM_ERROR:
      Serial.println("Error: no temperature sensor connected");
      break;
    case orp.STATUS_NO_ERROR:
      Serial.print("C|F: ");
      Serial.print(orp.tempC);
      Serial.print(" | ");
      Serial.println(orp.tempF);
      break;
  }
}

void i2c()
{
  uint8_t i2c_address;
  char * p;

  if (p1.length())
  {
    i2c_address = strtoul(p1.c_str(), & p, 16);
    Serial.println(i2c_address);
    if (i2c_address == 0)
    {
      Serial.println("Error: malformed I2C address");
    }
    else if ((i2c_address <= 0x07) || (i2c_address > 0x7f))
    {
      Serial.println("Error: I2C address not in valid range");
    }
    else
    {
      orp.setI2CAddress(i2c_address);
    }
  }
}

void measure_orp()
{
  while (Serial.available() == 0)
  {
    orp.measureORP();
    switch (orp.status)
    {
      case orp.STATUS_SYSTEM_ERROR:
        Serial.println("Error: Module not functioning properly");
        break;
      case orp.STATUS_NO_ERROR:
        Serial.print(orp.mV, 3);
        Serial.println((String)" mV");
        break;
    }
    delay(1000);
  }
}

void single()
{
  orp.calibrateSingle(p1.toFloat(), true);
  switch (orp.status)
  {
    case orp.STATUS_SYSTEM_ERROR:
      Serial.println("Error: Module not functioning properly");
      break;
    case orp.STATUS_NO_ERROR:
      config();
      break;
  }
}

void help()
{
  Serial.println(F("List of available commands, parameters separated by spaces, `low 4.0 22.1`, bracketed numbers are defaults if none provided"));
  Serial.println(F("config -or- c : no parameters : Displays all configuration and system information."));
  Serial.println(F("orp           : no parameters : Starts an ORP measurement."));
  Serial.println(F("i2c           : I2C_address : Changes the module's I2C address."));
  Serial.println(F("reset -or- r  : no parameters : Returns all configuration information to default values."));
  Serial.println(F("sin           : calibration_mV : Single-point calibration."));
  Serial.println(F("temp -or- t   : no parameters : Starts a temperature measurement"));
}

void cmd_run()
{
  if ((cmd == "conf") || (cmd == "config") || (cmd == "c")) config();
  if ((cmd == "reset") || (cmd == "r")) config_reset();
  if ((cmd == "temp") || (cmd == "t")) temperature();
  if (cmd == "sin") single();
  if (cmd == "i2c") i2c();
  if (cmd == "orp") measure_orp();
  if ((cmd == "help") || (cmd == "h")) help();
}

void cli_process()
{
  while (Serial.available())
  {
    char c = Serial.read();

    switch (c)
    {
    case '\n':
      Serial.println();
      cmd = buffer.substring(0, buffer.indexOf(" ", 0));
      cmd.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p1 = buffer.substring(0, buffer.indexOf(" ", 0));
      p1.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p2 = buffer.substring(0, buffer.indexOf(" ", 0));
      p2.trim();
      cmd_run();

      Serial.print("> ");
      buffer = "";
      break;

    case '\b': // backspace
      buffer.remove(buffer.length() - 1);
      Serial.print("\b \b");
      break;

    default: // everything else
      buffer += c;
      Serial.print(c);
    }
  }
}

void setup()
{
  Wire.begin();
  orp.begin();
  Serial.begin(9600);
  Serial.println("Type `help` for a list of commands");
  config();
  Serial.print("> ");
}

void loop()
{
  cli_process();
}
