#include "syscall.h"

int
main()
{
	char s [512];
	SynchGetString(s,128);
	SynchPutString(s);
	return 0;
}
