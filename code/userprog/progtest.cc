// progtest.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"

#ifdef CHANGED
#include "synchconsole.h"
#endif // CHANGED
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

void
StartProcess (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", filename);
	  return;
      }
    space = new AddrSpace (executable);
    currentThread->space = space;

    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register



    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void
ReadAvail (int arg)
{
    readAvail->V ();
}
static void
WriteDone (int arg)
{
    writeDone->V ();
}
//--------------------------------
// SynchConsole
//--------------------------------
#ifdef CHANGED
void
SynchConsoleTest (char *in, char *out)
{
char ch;
	delete synchconsole;
	synchconsole = new SynchConsole(in, out);
	while ((ch = synchconsole->SynchGetChar()) != EOF ){
		//if(ch != 'q'){
			synchconsole->SynchPutChar(ch);
		//}else{
		//	return;
		//}
	}
	fprintf(stderr, "Solaris: EOF detected in SynchConsole!\n");
}
#endif // CHANGED


//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest (char *in, char *out)
{
    char ch;
    #ifdef CHANGED
    delete synchconsole;
    #endif //CHANGED
    console = new Console (in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore ("read avail", 0);
    writeDone = new Semaphore ("write done", 0);

    for (;;)
      {
		


	  readAvail->P ();	// wait for character to arrive
	  ch = console->GetChar ();

	  
	  if(ch == EOF){
	  	#ifdef CHANGED
	  	synchconsole = new SynchConsole(NULL, NULL);
	  	break;
	  	#endif //CHANGED
	  }
 		
	  if (ch != '\n'){	// On affiche pas les caractères retour a la ligne et end of file
		console->PutChar ('<');	
	  	writeDone->P ();	// Attente de la fin du write du <	
		console->PutChar (ch);	// echo it!
  		writeDone->P ();	// wait for write to finish
	  	console->PutChar ('>');	
	  	writeDone->P ();	// Attente de la fin du write du >
	  }else{
	  	console->PutChar('\n');
	  	writeDone->P ();
	  }

	  if (ch == EOF)
	      return;		// if q, quit
      }
}