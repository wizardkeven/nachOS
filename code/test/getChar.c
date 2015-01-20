#include "syscall.h"

int
main()
{
	char s = SynchGetChar();
	PutChar(s);
	PutChar('\n');
	return 0;
}