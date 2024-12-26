#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "coupe.h"
#include "Timer.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#define MAX_OBJECTIF 5

objectif_coupe* list_of_objectif[MAX_OBJECTIF]; //contient tous les objets à toucher

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

//configure et initialise les objectifs afin qu'ils deviennent bleu si un nombre de leur pixels à été touché
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

		list_of_objectif[i] = obj; //ajoute dans la liste globale après l'avoir initialisé
	}

	uint16 randomX = 0;
	uint16 randomY = 0;
	for(i = 0; i < nombre; ++i){

		randomX = (rand() % (SCREEN_TILE_WIDTH - 2)) + 1;
		randomY = (rand() % (SCREEN_TILE_HEIGHT - 2)) + 1;

		(list_of_objectif[i])->tileX = randomX;
		(list_of_objectif[i])->tileY = randomY;

		configuration_tiles_coupe(randomX, randomY);
	}
}

//configuration des objectifs selon la difficulté du jeu (status)
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
	status->minigame_total_time = TEMPS_MAX_COUPE/(status->vitesse->nombre) + TEMPS_MIN_COUPE;
	AnnexeCounter(status);

	while(!echec && !success){

		upper_afficher_barre((int)(32*status->minigame_left_time/status->minigame_total_time));

		scanKeys();
		touchRead(&touch);

		int i;
		int objectif_ok = 0;
		for(i = 0; i < MAX_OBJECTIF; ++i){
			if(list_of_objectif[i]->tileX != 0){ //pour voir si l'objectif a été initialisé

				objectif_coupe* obj = list_of_objectif[i];

				if(touch.px > 8*obj->tileX && touch.px < 8*(obj->tileX+2) && touch.py > 8*obj->tileY && touch.py < 8*(obj->tileY+2)){
					obj->touchedPosition[(touch.py%(8*obj->tileY))*16 + (touch.px%(8*obj->tileX))] = 1; //on valide la position comme touchée dans le tableau de l'objectif concerné
				}
				int total = 0;
				int j;
				for(j = 0; j < 256; ++j){
					if(obj->touchedPosition[j] == 1){
						total++;
					}
				}
				obj->compteur = total;
				if(total >= 4){ //l'objet a été coupé (totalement arbitraire)
					configuration_tiles_coupe_vert(obj->tileX, obj->tileY);
					obj->tileX = 0; //pour signifier que l'objet n'est plus à considérer
				}
			}
			else{ //l'objectif a déjà été coupé ou n'a pas été initialisé
				objectif_ok++;
			}

		}

		if(objectif_ok >= 5){ //on a 5 objets qui ont tileX == 0 (ça veut dire que tout a été validé)
			success = true;
			status->score->nombre += 1;
			status->difficulte->nombre = (status->difficulte->nombre % 3) + 1;
			if(status->difficulte->nombre == 1) status->vitesse->nombre += 1;
			mmEffect(SFX_VICTOIRE);
		}

		if(status->minigame_left_time <= 0){ //échec par le temps
			echec = true;
			status->vie_restante -= 1;
			mmEffect(SFX_ECHEC);
		}

		swiWaitForVBlank();
	}

	//fin du minijeu
	Attendre(2);

	int i;
	for(i = 0; i < MAX_OBJECTIF; ++i){
		free(list_of_objectif[i]);
	}
}
