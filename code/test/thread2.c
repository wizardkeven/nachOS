#include "syscall.h"

void print(int i){
	int j = 0;
	for(j = 0; j < 3; j++){
		if(i%2 == 0){
			SynchPutString("Coucou, Process 2 thread 1 !");
		}else{
			SynchPutString("Coucou, Process 2 thread 2 !");
		}
	}

	if(i%2 == 0){
		SynchPutString("\nProcess 2 thread 1 terminé !\n");
	}else{
		SynchPutString("\nProcess 2 thread 2 terminé !\n");
	}

	//char z = SynchGetChar();
	//PutChar(z);
	
	UserThreadExit();
}

int main(){

	SynchPutString("\nProcess 2 lancé !\n");

	int t1, t2;
	t1 = UserThreadCreate(print,(void *) 7);
	t2 = UserThreadCreate(print,(void *) 8);

	SynchPutString("\nJe suis le Process 2 et j'ai fini mon travail dans le main principal !\n");
	//char a = SynchGetChar();
	


	return 0;
}
