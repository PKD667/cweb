# CWEB

This is an attempt to creating a basic http server in C.
It will be a very basic server, but it will be able to handle multiple requests at the same time.

# Goals

The main goal of this project is to use the C language inside html files , like this:

```html

<html>
    <?c>
        char* date = calloc(100, sizeof(char));
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        strftime(date, 100, "%c", &tm);
    <?c\>
    <head>
        <title>My first page</title>
    </head>
    <body>
        <h1>Hello World</h1>
        <p>Today is {{date}}</p>
    </body>


</html>

```

# functioning

This software will work like the c shell, it will read the html file and will execute the c code inside the html file, and will replace the c code with the result of the execution.
