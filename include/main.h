#define PORT 8888
#define ADDR "0.0.0.0"

#include "html.h"

int strtsckt();
int process(char* message,char** response);
struct msg httparse(char* requests);

int get(char* path,char** response);
int post(char* path,unsigned char* data,char** response);

int runc(char* ccode,struct html_tag* html);
int getcode(struct html_tag* html);

