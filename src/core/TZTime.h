//
// This class implement some timezone
//
#ifndef TZTIME_H
#define TZTIME_H

// Max number of timezone, should not be too big :)
#define MAX_TIMEZONE 10

#include <Arduino.h>
#include <Time.h>
#include <Timezone.h>

class TZTime {
public:
TZTime();
~TZTime();

int tzCount = 0;
String name[MAX_TIMEZONE];
String label[MAX_TIMEZONE];
Timezone *tz[MAX_TIMEZONE];

private:

// Source of this code
// https://github.com/thijse/Arduino-Code-and-Libraries/blob/master/Libraries/Timezone/Examples/WorldClock/WorldClock.pde

//Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};      //UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};      //UTC + 10 hours

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};       //Central European Summer Time
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};         //Central European Standard Time

//United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};          //British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};           //Standard Time

//US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};    //Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};     //Eastern Standard Time = UTC - 5 hours

//US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, dowSunday, Mar, 2, -300};
TimeChangeRule usCST = {"CST", First, dowSunday, Nov, 2, -360};

//US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, dowSunday, Mar, 2, -360};
TimeChangeRule usMST = {"MST", First, dowSunday, Nov, 2, -420};

//Arizona is US Mountain Time Zone but does not use DST

//US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, dowSunday, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, dowSunday, Nov, 2, -480};

// Don't forget to increase MAX_TIMEZONE if you have more than MAX_TIMEZONE

};

#endif
