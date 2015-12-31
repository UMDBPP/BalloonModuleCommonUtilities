/*
 * BalloonModule.h
 * balloon module: Arduino Uno with SFE_BMP180 pressure sensor attached to 
 * green -> SDA, blue -> SCL, white -> 3.3v, black -> GND
 */

#include <SFE_BMP180.h>

double temperature, altitude, pressure, baselinePressure;
float MET, hours = 0, minutes = 0, seconds = 0;
int hoursInt = 0, minutesInt = 0, secondsInt = 0;
unsigned long millisecondTime = 0;
const int delayMilliseconds = 1000;    // time in milliseconds to delay (wait) each repetition
int launchTolerance = 0;

SFE_BMP180 pressureSensor;

class BalloonModule
{
    public:
        // base library type
        BalloonModule();
        // Initialize pressure sensor (start readings). This should be the first line of setup.
        void initialize();
        // Main looping code to detect and print altitude. Starts once launch is detected
        void printStatusAfterLaunch();
        // Returns current pressure reading using temperature
        double getPressure();
        // Returns current altitude difference from baseline reading using current pressure
        double getRelativeAltitude();
        // Prints time in [HH:MM:SS]
        void printTime();
        // Prints meters and feet equivalent to the console; for example, printMetersAndFeet(20000) prints "20000 meters (65616.96 feet)"
        void printMetersAndFeet(double value);
        // prints current altitude
        void printAltitude();
};
