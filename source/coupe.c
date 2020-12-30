#include "coupe.h"

void configuration_objectif_coupe(){

	int tileX = 18;
	int tileY = 18;
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_COEUR;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_COEUR;

}


void mini_jeu_coupe(){

	configuration_objectif_coupe();

	bool echec = false;
	bool success = false;

	int touchedPositions[SCREEN_WIDTH*SCREEN_HEIGHT]; //contient toute les positions déjà touchée

	consoleDemoInit();

	touchPosition touch;
	while(!echec && !success){

		scanKeys();
		touchRead(&touch);
		if(touch.px || touch.py){ //si l'écran a été touché
			touchedPositions[SCREEN_WIDTH*touch.py + touch.px] = 1;

			printf("x: %i, y: %i\n", touch.px, touch.py);

			//on regarde si on a coupé qqch:
			int compteur = 0;
			int i;
			for(i = 0; i < 8*8*4; ++i){ //on traverse le bloc que constitue l'elem à couper
				//compteur += touchedPositions[18*8 + qqch];
			}
			if(compteur >= 4){ //totalement arbitraire, permet de savoir si on a assez traversé un objet
				success = true;
			}
		}




	}
}
