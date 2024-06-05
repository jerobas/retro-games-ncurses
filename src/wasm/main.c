#include <stdio.h>
#include <emscripten/emscripten.h>
#define EXTERN

EXTERN EMSCRIPTEN_KEEPALIVE void myFunction(int argc, char **argv)
{
    printf("MyFunction Called\n");
}