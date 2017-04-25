#include "TZTime.h"

TZTime::TZTime() {

        //United Kingdom (London, Belfast)
        name[tzCount] = "UTC";
        label[tzCount] = "Coordinated Universal Time (GMT)";
        tz[tzCount] = new Timezone(GMT, GMT);
        tzCount++;

        //Australia Eastern Time Zone (Sydney, Melbourne)
        name[tzCount] = "ausET";
        label[tzCount] = "Australia Eastern Time Zone (Sydney, Melbourne)";
        tz[tzCount] = new Timezone(aEDT, aEST);
        tzCount++;

        //Central European Time (Frankfurt, Paris)
        name[tzCount] = "CE";
        label[tzCount] = "Central European Time (Frankfurt, Paris)";
        tz[tzCount] = new Timezone(CEST, CET);
        tzCount++;

        //United Kingdom (London, Belfast)
        name[tzCount] = "UK";
        label[tzCount] = "United Kingdom (London, Belfast)";
        tz[tzCount] = new Timezone(BST, GMT);
        tzCount++;

        //US Eastern Time Zone (New York, Detroit)
        name[tzCount] = "usET";
        label[tzCount] = "US Eastern Time Zone (New York, Detroit)";
        tz[tzCount] = new Timezone(usEDT, usEST);
        tzCount++;

        //US Central Time Zone (Chicago, Houston)
        name[tzCount] = "usCT";
        label[tzCount] = "US Central Time Zone (Chicago, Houston)";
        tz[tzCount] = new Timezone(usCDT, usCST);
        tzCount++;

        //US Mountain Time Zone (Denver, Salt Lake City)
        name[tzCount] = "usMT";
        label[tzCount] = "US Mountain Time Zone (Denver, Salt Lake City)";
        tz[tzCount] = new Timezone(usMDT, usMST);
        tzCount++;

        //Arizona is US Mountain Time Zone but does not use DST
        name[tzCount] = "usAZ";
        label[tzCount] = "Arizona is US Mountain Time Zone but does not use DST";
        tz[tzCount] = new Timezone(usMST, usMST);
        tzCount++;

        //US Pacific Time Zone (Las Vegas, Los Angeles)
        name[tzCount] = "usPT";
        label[tzCount] = "US Pacific Time Zone (Las Vegas, Los Angeles)";
        tz[tzCount] = new Timezone(usPDT, usPST);
        tzCount++;

        // Don't forget to increase MAX_TIMEZONE if you have more than MAX_TIMEZONE

        if (tzCount>=MAX_TIMEZONE) {
                Serial.println("WARNING, you need to increase MAX_TIMEZONE in TZZime.h");
        }

};

TZTime::~TZTime() {
        for (int i=0; i<tzCount; i++) {
                delete(tz[i]);
                //delete(label[i]);
        }
};
