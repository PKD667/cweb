#include "string.h"

#include "../include/http.h"

struct msg httparse(char* requests)
{   
    struct msg message;
    char* request = requests;
    message.method = strtok(request," ");
    message.path = strtok(NULL," ");
    message.version = strtok(NULL,"\r\n");
    message.data = (unsigned char*)strtok(NULL,"\r\n\r\n");

    return message;
}