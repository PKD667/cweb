#include "string.h"

#include "../include/main.h"
#include "../include/http.h"

int process(char* message,char** response)
{
    struct msg request = httparse(message);

    if (strcmp(request.method,"GET") == 0)
    {
        return get(request.path,response);
    }
    else if (strcmp(request.method,"POST") == 0)
    {
        return post(request.path,request.data,response);
    }
    else
    {
        *response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n<html><body><h1>405 Method Not Allowed</h1></body></html>";
        return 405;
    }

    return 200;
}

int get(char* path,char** response)
{
    if (strcmp(path,"/") == 0)
    {
        *response = "<html><body><h1>200 OK</h1></body></html>";
        return 200;
    }
    else
    {
        *response = "n<html><body><h1>404 Not Found</h1></body></html>";
        return 404;
    }
    return 0;
}

int post(char* path,unsigned char* data,char** response)
{
    if (strcmp(path,"/") == 0)
    {
        *response = "<html><body><h1>200 OK</h1></body></html>";
        return 200;
    }
    else
    {
        *response = "<html><body><h1>404 Not Found</h1></body></html>";
        return 404;
    }
    return 0;
}