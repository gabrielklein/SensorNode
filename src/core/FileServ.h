//
// This class is used to share and distribute files
//
#ifndef FILESERV_H
#define FILESERV_H

#include <Arduino.h>
#include "FS.h"

class FileServ {

public:
FileServ();
~FileServ();
void setup();
File openR(const String &string);
File openW(const String &string);
};

#endif
