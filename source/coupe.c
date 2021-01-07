#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "coupe.h"

void configuration_objectif_coupe(){

	int tileX = 18;
	int tileY = 18;
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_COEUR;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_COEUR;

	//printf("config ok\n");
}


void mini_jeu_coupe(game_status* status){

	printf("on rentre dans le jeu\n");

	configuration_objectif_coupe();

	bool echec = false;
	bool success = false;

	int touchedPositions[16*16]; //contient toute les positions déjà touchée

	int j;
	for(j = 0; j < 16*16; ++j){
		touchedPositions[j] = 0;
	}

	//consoleDemoInit();

	touchPosition touch;
	while(!echec && !success){

		scanKeys();
		touchRead(&touch);

		printf("x: %i, y: %i\n", touch.px, touch.py);

		if(touch.px > 8*18 && touch.px < 8*20 && touch.py > 8*18 && touch.py < 8*20){ //si l'écran a été touché
			touchedPositions[(touch.py%(8*18))*16 + (touch.px%(8*18))] = 1;

			//on regarde si on a coupé qqch:
			int compteur = 0;
			int i;
			for(i = 0; i < 16*16; ++i){ //on traverse le bloc que constitue l'elem à couper
				compteur += touchedPositions[i]; //TODO: trouver la bonne position de l'objet
			}
			if(compteur >= 4){ //totalement arbitraire, permet de savoir si on a assez traversé un objet
				success = true;
				status->score->nombre += 1;
			}
		}

		swiWaitForVBlank();

	}
}
