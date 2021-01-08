#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "coupe.h"

#define MAX_OBJECTIF 10

objectif_coupe list_of_objectif[MAX_OBJECTIF];

void configuration_tiles_coupe(int tileX, int tileY){
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_COEUR;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_COEUR;
}

void configuration_objectif_coupe(int nombre){

	int i = 0;

	objectif_coupe obj = {0, 0, {}, 0};
	for(i = 0; i < MAX_OBJECTIF; ++i){ //on reset tout
		list_of_objectif[i] = obj;
	}

	int randomX = 0;
	int randomY = 0;
	for(i = 0; i < nombre; ++i){

		randomX = rand() % 18;
		randomY = rand() % 18;

		configuration_tiles_coupe(randomX, randomY);
	}


	//printf("config ok\n");
}

void configuration_mini_jeu_coupe(game_status* status){

	switch (status->difficulte->nombre){
		case 1:
			configuration_objectif_coupe(3);
			break;

		case 2:
			configuration_objectif_coupe(4);
			break;

		default: //3 ou plus
			configuration_objectif_coupe(5);
			break;
	}
}


void mini_jeu_coupe(game_status* status){

	printf("on rentre dans le jeu\n");

	configuration_mini_jeu_coupe(status);

	bool echec = false;
	bool success = false;

	//int touchedPositions[16*16]; //contient toute les positions déjà touchée

	int j;
	for(j = 0; j < 16*16; ++j){
		//touchedPositions[j] = 0;
	}

	//consoleDemoInit();

	touchPosition touch;
	while(!echec && !success){

		scanKeys();
		touchRead(&touch);

		printf("x: %i, y: %i\n", touch.px, touch.py);

		if(touch.px > 8*18 && touch.px < 8*20 && touch.py > 8*18 && touch.py < 8*20){ //si l'écran a été touché
			//touchedPositions[(touch.py%(8*18))*16 + (touch.px%(8*18))] = 1;

			//on regarde si on a coupé qqch:
			int compteur = 0;
			int i;
			for(i = 0; i < 16*16; ++i){ //on traverse le bloc que constitue l'elem à couper
				//compteur += touchedPositions[i];
			}
			if(compteur >= 4){ //totalement arbitraire, permet de savoir si on a assez traversé un objet
				success = true;
				status->score->nombre += 1;
			}
		}

		swiWaitForVBlank();

	}
}
