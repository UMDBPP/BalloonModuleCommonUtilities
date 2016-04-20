/*
 * Balloonduino.cpp
 * balloonduino: modified Arduino Mega
 */

#include <Balloonduino.h>

// Initializes all sensors. Needs to be run once in setup before any measurements can be taken.
void Balloonduino::begin()
{
    // initialize pressure / temperature / humidity sensor
    log("Initializing BME280 pressure, temperature, and humidity sensor...");
    if (BME280.begin())
    {
        updateSensorStatus(0, 0);
        log("BME280 initialized successfully.");
        log("Now attempting baseline reading...");
        // Print baseline pressure and temperature
        log("Pressure is " + String(getPressure()) + "mb.");
        log("Temperature is " + String(getTemperature()) + "C.");
        log("Humidity is " + String(getHumidity()) + "%.");
    }
    else
    {
        updateSensorStatus(0, 1);
        log("BME280 failed (is it disconnected?)");
    }

    // initialize orientation sensor
    log("Initializing BNO055 orientation sensor...");
    if (BNO055.begin())
    {
        updateSensorStatus(1, 0);
        log("BNO055 initialized successfully.");
    }
    else
    {
        updateSensorStatus(1, 1);
        log("BNO055 failed (is it disconnected?)");
    }

    // initialize real time clock
    log("Initializing DS1307 real time clock...");
    DS1307.begin();
    if (!DS1307.isrunning())
    {
        log("DS1307 failed (is it disconnected?)");
        updateSensorStatus(2, 1);
        // following line sets the RTC to the date & time this sketch was compiled
        DS1307.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    else
    {
        log("DS1307 initialized successfully.");
        log("Current time is " + getRealTimeString());
    }
    log("Initialization completed. Check console for errors.");

    displaySensorStatus();
}

// Returns temperature double in degrees Celsius
double Balloonduino::getTemperature()
{
    return BME280.readTemperature();
}

// Returns pressure double in millibars
double Balloonduino::getPressure()
{
    return BME280.readPressure();
}

// Returns altitude double in meters
double Balloonduino::getAltitude()
{
    return BME280.readAltitude(SENSORS_PRESSURE_SEALEVELHPA);
}

// Returns humidity double in percentage
double Balloonduino::getHumidity()
{
    return BME280.readHumidity();
}

// Returns current millisecond time string as "T+HH:MM:SS"
String Balloonduino::getMissionTimeString()
{
    byte seconds = millis() % 60, minutes = millis() / 60;
    byte hours = minutes / 60;

    String out = "T+";
    if (hours < 10)
    {
        out += "0";
    }
    out += String(hours) + ":";
    if (minutes < 10)
    {
        out += "0";
    }
    out += String(minutes) + ":";
    if (seconds < 10)
    {
        out += "0";
    }
    out += String(seconds);
    return out;
}

// Returns current time of day string as "hh:mm:ss a"
String Balloonduino::getRealTimeString()
{
    return String(DS1307.now().hour()) + ":" + String(DS1307.now().minute())
            + ":" + String(DS1307.now().second());
}

// Returns status report string as "[HH:MM:SS] | -60C | 1050m | 500mb | 75%"
String Balloonduino::getSensorReadingsString()
{
    return getRealTimeString() + String(getTemperature()) + "C | "
            + String(getAltitude()) + "m | " + String(getPressure()) + "mb | "
            + String(getHumidity()) + "%";
}

// Logs message
void Balloonduino::log(String message)
{
    Serial.println("[" + getMissionTimeString() + "] " + message);
}

// Displays sensor errors to pin 13 (built in LED), blinks once for OK status, twice otherwise
void Balloonduino::displaySensorStatus()
{
    digitalWrite(13, LOW);
    delay(500);
    for (byte index = 0; index < numberOfSensors; index++)
    {
        digitalWrite(13, HIGH);
        delay(125);
        digitalWrite(13, LOW);
        delay(125);
        if (sensors[index])
        {
            digitalWrite(13, HIGH);
            delay(125);
            digitalWrite(13, LOW);
            delay(125);
        }
        else
        {
            delay(250);
        }
        delay(250);
    }
    delay(250);
}

// Private function, run to update sensor status array
void Balloonduino::updateSensorStatus(byte address, byte status)
{
    sensors[address] = status;
}
