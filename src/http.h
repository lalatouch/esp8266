#ifndef _HTTP_H
#define _HTTP_H

namespace http {

void setup();
void handle();

namespace client {

void get(String path);
void post(String path, String data);

}

}

#endif

