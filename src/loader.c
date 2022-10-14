#include "dlfcn.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../include/html.h"

#define BASE_C "#include \"include/html.h\"\n#include \"string.h\"\n  int code(struct html_tag* html){%s}"


int runc(char* ccode,struct html_tag* html)
{   
    char * fincode = malloc(strlen(ccode)+strlen(BASE_C)+1);
    sprintf(fincode,BASE_C,ccode);
    printf("ccode is %s\n",ccode);
    FILE* f = fopen("dynhtml.c","w");
    fprintf(f,"%s",fincode);
    fclose(f);
    system("gcc -shared -fPIC dynhtml.c -o libdynhtml.so ");
    void* handle = dlopen("./libdynhtml.so",RTLD_LAZY);
    if (!handle)
    {
        printf("error: %s\n",dlerror());
        return 1;
    }
    int (*code)(struct html_tag* html) = dlsym(handle,"code");
    if (!code)
    {
        printf("error: %s\n",dlerror());
        return 1;
    }
    code(html);
    dlclose(handle);
    return 0;

}