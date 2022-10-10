#include "stdlib.h"
#include "stdio.h"
#include "unistdio.h"
#include "unistd.h"

#include "../include/main.h"
#include "../include/html.h"

int main(void)
{
    test_parser("test.html");
    exit(0);

    // now we can start the server
    strtsckt();

    sleep(100000000);
    return 0;
}
