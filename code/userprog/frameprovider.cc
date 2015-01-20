#ifdef CHANGED
#include "frameprovider.h"
#include <string.h>
#include "system.h"

FrameProvider::FrameProvider(int n) {
  	cadreFrame = new BitMap(n);
  	taille = n;
}

FrameProvider::~FrameProvider() {
 	delete cadreFrame;
}

int
FrameProvider::GetEmptyFrames(){
	int recup = cadreFrame->Find();
	if(recup != -1){
		// on met & car bzero a besoin d'une adresse
		bzero(&machine->mainMemory[recup*PageSize], PageSize);
	}
	return recup;
}

void
FrameProvider::ReleaseFrame(int n){
	// libération dans la BitMap
	cadreFrame->Clear(n);
}

int
FrameProvider::NumAvailFrame() {
	// récupération du nombre de bits libres dans la BitMap
  	return cadreFrame->NumClear();
}

#endif //CHANGED