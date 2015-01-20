#include "syscall.h"

void print(int i){
	int j = 0;
	for(j = 0; j < 3; j++){
		SynchPutInt(i);
	}
	UserThreadExit();
}

int main(){

	int t1, t2, t3, t4, t5, t6, t7, t8;
	t1 = UserThreadCreate(print,(void *) 1);
	t2 = UserThreadCreate(print,(void *) 2);
	t3 = UserThreadCreate(print,(void *) 3);
	t4 = UserThreadCreate(print,(void *) 4);
	t5 = UserThreadCreate(print,(void *) 5);
	t6 = UserThreadCreate(print,(void *) 6);
	t7 = UserThreadCreate(print,(void *) 7);
	t8 = UserThreadCreate(print,(void *) 8);


	//UserThreadJoin(t1);
	SynchPutString("Je suis le thread principal");
	//char a = SynchGetChar();
	


	return 0;
}
