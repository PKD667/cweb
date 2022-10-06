#define PORT 8888
#define ADDR "127.0.0.1"



int strtsckt();
int process(char* message,char** response);
struct msg httparse(char* requests);

int get(char* path,char** response);
int post(char* path,unsigned char* data,char** response);