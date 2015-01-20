#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "system.h"
#include "addrspace.h"



static void StartUserThread(int f){

  	//Recuperation des parametres passes precedements
  	argThreadUser *argRecup = (argThreadUser *) f;


    //Initialisation des registres
    // Mise en place le PC correctement, en debut de fonction
    machine->WriteRegister (PCReg, argRecup->f);
    // Mise place ensuite l'argument dans le registre 4
    machine->WriteRegister(4, argRecup->arg);
    
    // Mise en place du PC Next pour l'instruction suivante
    machine->WriteRegister (NextPCReg, argRecup->f+4);

    // Initialisation de la bitmap 
	machine->WriteRegister(StackReg, currentThread->space->InitRegistersU(&(currentThread->id)));
  	

    // Lancement 
    currentThread->space->addThread();
    machine->Run();
}


int do_UserThreadCreate(int f, int arg){
	Thread *newThread = new Thread("new thread");
	// Si le thread cree est nul, on renvoie -1
	if(newThread == NULL){
		return -1;
	}

	// Arguments concatenes
	argThreadUser * argv = new argThreadUser;
	argv->f = f;
	argv->arg = arg;

    newThread->id = currentThread->space->getID();
    if(newThread->id == -1){
        // Une erreur
        printf("\nError during the creation of a new thread\n");
        delete newThread;
        return -1;
    }else{
    	// Plaçage en liste d'attente
    	newThread->Fork(StartUserThread, (int)argv);
    }
    currentThread->space->semJoin[newThread->id]->P();

    //récupération de l'id du thread !
    machine->WriteRegister(2, newThread->id);
    // Sans cela, obligation d'effectuer une action  dans le main, car les threads sont justes créés mais
    // pas encore démarrés
    currentThread->Yield();
	return 0;
}

void 
do_UserThreadExit() {
		currentThread->space->semJoin[currentThread->id]->V();
        currentThread->space->deleteThread();
        currentThread->Finish();
}

void
do_UserThreadJoin(int i){

    currentThread->space->semJoin[i]->P();
}





#endif //CHANGED
