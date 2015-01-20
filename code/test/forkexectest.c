#include "syscall.h"

int
main()
{

	SynchPutString("\nTest de ForkExec, ici le Process 0 !\n");


	ForkExec("./thread1");

	ForkExec("./thread2");

	SynchPutString("\nProgramme Process 0 terminé !\n");

	return 0;
}
