// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#ifdef CHANGED
#define MAX_PAGE_THREADS 3 //nombre max de pages par threads thread
#endif // CHANGED

#include "copyright.h"
#include "filesys.h"

#ifdef CHANGED
#include "synch.h"
#include "bitmap.h"

#endif //CHANGED

#define UserStackSize		2048	// increase this as necessary!

class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space
    #ifdef CHANGED
    Semaphore *semJoin[UserStackSize/MAX_PAGE_THREADS];
    #endif //CHANGED
    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    #ifdef CHANGED
    int InitRegistersU(int *threadId); // InitRegister mais pour UserThread (complément de fonction)
    void deleteThread(); //suppression d'un thread
    void addThread(); //ajout d'un thread
     
    
    #endif // CHANGED

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

    void verificationEnd();

    int getID();

    bool liberation;
  private:
    TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space

    #ifdef CHANGED
    Semaphore *semT;
    Semaphore *semBM;
    Semaphore *semA;

    int nbT;
    BitMap *bitmap;
    #endif //CHANGED



};

#endif // ADDRSPACE_H
