#include "FileServ.h"

FileServ::FileServ() {
};
FileServ::~FileServ() {
};

/**
 * Called at setup time. Use this call to initialize some data.
 */
void FileServ::setup() {

        // We want to be able to read files
        SPIFFS.begin();

};

/**
 * Open a file and return the content
 */
File FileServ::openR(const String &string) {
        return SPIFFS.open(string, "r");
}

/**
 * Open a file and return the content
 */
File FileServ::openW(const String &string) {
        return SPIFFS.open(string, "w");
}
