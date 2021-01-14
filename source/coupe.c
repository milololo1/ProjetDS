#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "coupe.h"

#define MAX_OBJECTIF 5

objectif_coupe* list_of_objectif[MAX_OBJECTIF];

void configuration_tiles_coupe(int tileX, int tileY){
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_OBJECTIF_ROUGE;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_OBJECTIF_ROUGE;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_OBJECTIF_ROUGE;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_OBJECTIF_ROUGE;
}

void configuration_tiles_coupe_vert(int tileX, int tileY){
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_OBJECTIF_VERT;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_OBJECTIF_VERT;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_OBJECTIF_VERT;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_OBJECTIF_VERT;
}

void configuration_objectif_coupe(int nombre){

	int i;

	for(i = 0; i < MAX_OBJECTIF; ++i){ //on reset tout

		objectif_coupe* obj = malloc(sizeof(objectif_coupe));

		obj->tileX = 0; //valeur de référence pour vérifié si l'objectif est validé
		obj->tileY = 0;
		obj->compteur = 0;


		int j;
		for(j = 0; j < 256; ++j){
			obj->touchedPosition[j] = 0;
		}

		list_of_objectif[i] = obj;
	}

	int randomX = 0;
	int randomY = 0;
	for(i = 0; i < nombre; ++i){

		randomX = (rand() % 20) + 1;
		randomY = (rand() % 20) + 1;

		(list_of_objectif[i])->tileX = randomX;
		(list_of_objectif[i])->tileY = randomY;

		configuration_tiles_coupe(randomX, randomY);
	}


	printf("config ok\n");
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

	configuration_mini_jeu_coupe(status);

	bool echec = false;
	bool success = false;

	touchPosition touch;

	//mise en place du timer
	status->minigame_total_time = TEMPS_MAX/(status->vitesse->nombre) + TEMPS_MIN;
	AnnexeCounter(status);

	while(!echec && !success){

		upper_afficher_barre((int)(32*status->minigame_left_time/status->minigame_total_time));

		scanKeys();
		touchRead(&touch);

		//printf("x: %i, y: %i\n", touch.px, touch.py);

		int i;
		int objectif_ok = 0;
		for(i = 0; i < MAX_OBJECTIF; ++i){
			if(list_of_objectif[i]->tileX != 0){ //pour voir si l'objectif a été initialisé

				//printf("ici c'est ok\n");

				objectif_coupe* obj = list_of_objectif[i];
				if(touch.px > 8*obj->tileX && touch.px < 8*(obj->tileX+2) && touch.py > 8*obj->tileY && touch.py < 8*(obj->tileY+2)){
					obj->touchedPosition[(touch.py%(8*obj->tileY))*16 + (touch.px%(8*obj->tileX))] = 1;
				}
				int total = 0;
				int j;
				for(j = 0; j < 256; ++j){
					if(obj->touchedPosition[j] == 1){
						total++;
					}
				}
				obj->compteur = total;
				if(total >= 4){ //l'objet a été coupé
					configuration_tiles_coupe_vert(obj->tileX, obj->tileY);
					obj->tileX = 0; //pour signifier que l'objet n'est plus à considérer
				}
			}
			else{
				//printf("pas censé être là\n");
				objectif_ok++;
			}

		}

		if(objectif_ok >= 5){ //on a 5 objets pas init (ça veut dire que tout a été validé)
			success = true;
			status->score->nombre += 1;
			status->difficulte->nombre = (status->difficulte->nombre % 3) + 1;
			if(status->difficulte->nombre == 1) status->vitesse->nombre += 1;
		}

		if(status->minigame_left_time <= 0){
			echec = true;
			status->vie_restante -= 1;
		}

		/*if(touch.px > 8*18 && touch.px < 8*20 && touch.py > 8*18 && touch.py < 8*20){ //si l'écran a été touché
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
		}*/

		swiWaitForVBlank();

	}

	Attendre(2);

	int i;
	for(i = 0; i < MAX_OBJECTIF; ++i){
		free(list_of_objectif[i]);
	}
}
