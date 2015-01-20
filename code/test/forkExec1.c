#include "syscall.h"

int
main()
{

	SynchPutString("\nTest de ForkExec, ici le Process 3 !\n");


	ForkExec("./thread1");

	ForkExec("./thread2");

	SynchPutString("\nProgramme Process 3 terminé !\n");

	return 0;
	return 0;
}
