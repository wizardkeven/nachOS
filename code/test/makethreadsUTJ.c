#include "syscall.h"

void print(int i){
	int j = 0;
	for(j = 0; j < 3; j++){
		if(i%2 == 0){
			SynchPutString("Coucou, je m'execute dans un thread avec un param pair !");
		}else{
			SynchPutString("Coucou, je m'execute dans un thread avec un param impaire !");
		}
	}
	//char z = SynchGetChar();
	//PutChar(z);
	
	UserThreadExit();
}

int main(){

	int t1, t2;
	t1 = UserThreadCreate(print,(void *) 7);
	t2 = UserThreadCreate(print,(void *) 8);


	UserThreadJoin(t1);
	UserThreadJoin(t2);
	SynchPutString("Je suis le thread principal");
	//char a = SynchGetChar();
	


	return 0;
}
