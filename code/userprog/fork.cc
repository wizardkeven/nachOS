#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "system.h"
#include "addrspace.h"

static void StartUserProcess(int arg){

    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    //printf ("Lancement de la machine \n");
    machine->Run();
}

int 
do_ForkExec(char *s){

    DEBUG('f', "ForkExec.\n");

    OpenFile *file = fileSystem->Open(s);
    AddrSpace *espace_memoire;
    
    if(file == NULL){
        printf("Erreur lors de l'execution de ForkExec\n");
        delete [] s;
        return -1;
    }

    espace_memoire = new AddrSpace(file);

    delete file;

    Thread *newThread = new Thread("new thread");
    newThread->space = espace_memoire;

    machine->MaJProcess(1);

    newThread->ForkProcess(StartUserProcess, 0);
    
    currentThread->Yield();
    return 0;

}

#endif //CHANGED