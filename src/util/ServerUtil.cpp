#include "ServerUtil.h"

String ServerUtil::getMime(String name) {

        if (name.equals("/")) {
                return "text/html";
        }
        else if (name.endsWith(".png")) {
                return "image/png";
        }
        else if (name.endsWith(".jpg")) {
                return "image/jpeg";
        }
        else if (name.endsWith(".js")) {
                return "application/javascript";
        }
        else if (name.endsWith(".css")) {
                return "text/css";
        }
        else if (name.endsWith(".html")) {
                return "text/html";
        }
        else if (name.endsWith(".txt")) {
                return "text/plain";
        }
        else if (name.endsWith(".json")) {
                return "application/json";
        }
        else if (name.endsWith(".xml")) {
                return "application/xml";
        }

        return "text/html";
};
