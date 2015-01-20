#ifdef CHANGED
// Pour ne pas avoir une déclaration redondante
#ifndef FRAMEPROVIDERH
#define FRAMEPROVIDERH
#include "bitmap.h"

class FrameProvider {
    public:
        FrameProvider (int n);
      
        ~FrameProvider ();

        // fonction get pour récuperer un cadre libre
        int GetEmptyFrames();
        //libération d'un cadre
        void ReleaseFrame(int n);
        // donne le nombre de frames visibles
        int NumAvailFrame(); 

    private:
    	int taille;
    	BitMap *cadreFrame;    
};
#endif //FRAMEPROVIDERH
#endif //CHANGED