#include "syscall.h"

int
main()
{

	SynchPutString("\nTest de ForkExec, ici le Process 0 !\n");


	ForkExec("./forkExec1");

	ForkExec("./forkExec2");

	SynchPutString("\nFin du processus 0 !\n");

	
	return 0;
}
