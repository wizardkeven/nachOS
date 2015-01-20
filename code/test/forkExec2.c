#include "syscall.h"

int
main()
{

	SynchPutString("\nTest de ForkExec, ici le Process 4 !\n");


	ForkExec("./thread5");

	ForkExec("./thread6");

	SynchPutString("\nProgramme Process 4 terminé !\n");

	return 0;
}
