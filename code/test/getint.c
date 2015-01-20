#include "syscall.h"

int
main()
{
	SynchPutString("Combien fait 3 + 4?");
	int s;
	SynchGetInt(&s);
	SynchPutString("Vous avez tapé :");
	SynchPutInt(s);

	if(s != 7){
		SynchPutString("Mauvais résultat !");
	}else{
		SynchPutString("Bon résultat !");
	}

	SynchPutString("Et combien fait 2 * 0 ?");

	SynchGetInt(&s);
	SynchPutString("Vous avez tapé :");
	SynchPutInt(s);

	if(s != 0){
		SynchPutString("Mauvais résultat !");
	}else{
		SynchPutString("Bon résultat !");
	}

	return 0;
}