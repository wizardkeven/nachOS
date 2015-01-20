#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"
const int N = 1; // Choose it large enough!


void putsUser(char *s)
{
    char *p; for (p = s; *p != '\0'; p++) PutChar(*p);
}

void f(void *s)
{
    int i; for (i = 0; i < N; i++) putsUser((char *)s);
}


int main()
{
   void* fist= f;
   UserThreadCreate(fist, (void *) THIS);
   SynchPutString("coucou\n");
   f((void*) THAT);
   return 0;
}

