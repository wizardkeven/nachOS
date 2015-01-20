// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"

#include "addrspace.h"
#include "noff.h"

#include <strings.h>		/* for bzero */
#ifdef CHANGED
#include "frameprovider.h"


static void
ReadAtVirtual(OpenFile *executable, int virtualaddr, int numBytes,
    int position, TranslationEntry *pageTable, unsigned numPages){
    
    // Récupération des anciennes valeurs pour restaurer après
    // Sinon perte des valeurs...
    TranslationEntry *ex_pageT = machine->pageTable;
    unsigned ex_numP = machine->pageTableSize;

    // Ecriture des valeurs
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;

    char buffer[numBytes];


    int recuperation = executable->ReadAt(buffer, numBytes, position);
    int i = 0;
    // Re écriture en mémoire de l'adresse virtuelle
    for (i = 0; i < recuperation; i++){
        machine->WriteMem(virtualaddr+i, 1, buffer[i]);
    }

    // Remise des bonnes valeurs
    machine->pageTable = ex_pageT;
    machine->pageTableSize = ex_numP;
}
#endif //CHANGED


//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    NoffHeader noffH;
    unsigned int i, size;
    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;


    ASSERT (numPages <= NumPhysPages);	// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
	   numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    //printf("nbr de pages : %d", numPages);

    for (i = 0; i < numPages; i++)
      {
	  pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
	  //pageTable[i].physicalPage = i+1;
      
      // nouvelle méthode d'allocation des pages pour la mémoire
      #ifdef CHANGED
      if(frameProvider->NumAvailFrame() != 0){
           // printf("page allouée\n");
	       pageTable[i].physicalPage = frameProvider->GetEmptyFrames();
      }else{
            //printf("Erreur, pas assez de pages\n");
      }
      #endif //CHANGED


      pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }

// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
//    bzero (machine->mainMemory, size);

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
		 noffH.code.virtualAddr, noffH.code.size);

       #ifdef CHANGED
        ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, 
            noffH.code.inFileAddr, pageTable, numPages);
    #else
      executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
               noffH.code.size, noffH.code.inFileAddr);

    #endif //CHANGED
      }

    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
        
      #ifdef CHANGED
        ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size, 
            noffH.initData.inFileAddr, pageTable, numPages); 
        #else
      executable->ReadAt (&
			      (machine->mainMemory
			       [noffH.initData.virtualAddr]),
			      noffH.initData.size, noffH.initData.inFileAddr);
      
    #endif //CHANGED
      }

    #ifdef CHANGED
    nbT = 1; // le main

    DEBUG('z', "Initialisation de nbT : %d\n", nbT);



    semT = new Semaphore("nbT", 1); // Le problème du nombre de thread qui s'incrémentait mal
                                              // venait d'ici
    semBM = new Semaphore("BitMap", 1); // Semaphore pour la Bitmap également

    // initialisation du semJoin
    for (int j = 0; j<(UserStackSize/MAX_PAGE_THREADS) ; j++) {
        this->semJoin[j] = new Semaphore("Semaphore Join", 1);
    }


    bitmap = new BitMap(UserStackSize/(MAX_PAGE_THREADS * PageSize));
    //printf("taille de la BitMap : %d\n", (UserStackSize/(MAX_PAGE_THREADS * PageSize)) - 1);
    bitmap->Mark(0); // ne pas oublier le premier thread, le main

    semA = new Semaphore("Attente", 0); //Semaphore pour attendre la fin des autres threads
    

    liberation = false;
    #endif //CHANGED
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  // LB: Missing [] for delete
  // delete pageTable;
  delete [] pageTable;
  // End of modification
  #ifdef CHANGED

  unsigned i = 0;
  for(i = 0; i < numPages; i++){
    frameProvider->ReleaseFrame(pageTable[i].physicalPage);
  }


  delete bitmap; //suppression de la bitmap
  #endif //CHANGED
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to %d\n",
	   numPages * PageSize - 16);
}

#ifdef CHANGED

int
AddrSpace::InitRegistersU(int *threadId) {
    
    int startStack = numPages*PageSize;

    int renvoyer = -1;
    semBM->P();   
    //*threadId = bitmap->Find();
    if(*threadId != -1) {
        renvoyer = startStack - (PageSize*MAX_PAGE_THREADS*(*threadId));
    }
    semBM->V();
    return renvoyer;

}

// lors de la fin d'un thread

void
AddrSpace::deleteThread(){

    semT->P();
    semBM->P();

    nbT--;
    //Suppression du thread de la bitmap
    bitmap->Clear(currentThread->id);
    if(nbT == 0){
        //nous n'avons plus aucun autre thread que le thread principal, on peut donc l'arreter
        semA->V();
    }
    semT->V();
    semBM->V();
}

// lors de l'ajout d'un thread

void 
AddrSpace::addThread(){

    semT->P();
    nbT++;
    semT->V();
  
}

#endif //CHANGED

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
#ifdef CHANGED

void
AddrSpace::verificationEnd ()
{
    DEBUG('z', "VerificationEnd\n");
    
    semT->P();
    nbT--;
    semT->V();

    DEBUG('z', "Milieu VerificationEnd, nbT restant : %d\n", nbT);

    if(nbT != 0){
        // on doit bloquer le thread principal
        semA->P();
    }
    DEBUG('z', "Fin VerificationEnd\n");
}

//récupération de l'id

int
AddrSpace::getID()
{
    return bitmap->Find();
}



#endif //CHANGED