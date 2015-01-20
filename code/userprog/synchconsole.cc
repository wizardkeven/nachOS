#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
#include "console.h"


static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *semaphorePutChar;
static Semaphore *semaphoreGetChar;
static Semaphore *semaphorePutString;
static Semaphore *semaphoreGetString;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console(readFile, writeFile, ReadAvail, WriteDone, 0); // Completer, initialisation des sempahores a 0
	semaphorePutChar = new Semaphore("PutChar", 1);
	semaphoreGetChar = new Semaphore("GetChar", 1);
	semaphorePutString = new Semaphore("PutString", 1);
	semaphoreGetString = new Semaphore("GetString", 1);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	semaphorePutChar->P();
	/* Zone pour les <> autour des lettres
	if(ch != '\n'){
		console->PutChar('<');
		writeDone->P();
	}
	*/
	        
	console->PutChar(ch);
        writeDone->P();

    /* Zone pour <>    
	if(ch != '\n'){
		console->PutChar('>');
	    writeDone->P();
	}*/

	semaphorePutChar->V();
}

char SynchConsole::SynchGetChar()
{
	semaphoreGetChar->P();
	char c;
	readAvail->P ();
	c = console->GetChar ();


	semaphoreGetChar->V();
	return c;
}

void SynchConsole::SynchPutString(const char s[])
{
	semaphorePutString->P();
	int i;
        for(i=0;i<MAX_STRING_SIZE && s[i]!='\0';i++) {
                synchconsole->SynchPutChar((char)s[i]);
        }
        synchconsole->SynchPutChar('\n');
    semaphorePutString->V();    
}

void SynchConsole::SynchGetString(char *s, int n)
{
	semaphoreGetString->P();
	int i;
	char c;
	// attention a ne pas depasser la taille limite...
	for(i=0; i < n-1; i++){
		c = synchconsole->SynchGetChar();
		if(c == EOF || c == '\n'){
			break;
		}else{
			s[i] = c;
		}
	}
	s[i] = '\0';
	semaphoreGetString->V();
}

void SynchConsole::SynchPutInt(int n)
{
	
	char *s = new char[MAX_STRING_SIZE];
	snprintf(s, MAX_STRING_SIZE, "%d", n);
	synchconsole->SynchPutString(s);

	delete [] s;
	
}

void SynchConsole::SynchGetInt( int *n)
{
	
	int retour;
	// On travail sur des entiers...
	char *conversion = new char[12];
	SynchGetString(conversion, 12);
	sscanf(conversion, "%d", &retour);
	machine->WriteMem(*n, 4, retour);
	delete [] conversion;
	
}


#endif // CHANGED
