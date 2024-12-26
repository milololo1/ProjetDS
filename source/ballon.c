#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "ballon.h"
#include "spriteBallon.h"
#include "spriteBallonCasse.h"
#include "Graphics.h"
#include "Timer.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

//Configuration du temps total pour faire le mini jeux
void configuration_temp_total(game_status* status){
	status->minigame_total_time = TEMPS_MAX_BALLON/(status->vitesse->nombre) + TEMPS_MIN_BALLON;
	AnnexeCounter(status);
}

void afficher_objectif_rouge(){
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*OBJECTIF_TILEY + OBJECTIF_TILEX] = OFF_OBJECTIF_ROUGE;
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*OBJECTIF_TILEY + OBJECTIF_TILEX+1] = 1+OFF_OBJECTIF_ROUGE;
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*(OBJECTIF_TILEY+1) + OBJECTIF_TILEX] = 2+OFF_OBJECTIF_ROUGE;
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*(OBJECTIF_TILEY+1) + OBJECTIF_TILEX+1] = 3+OFF_OBJECTIF_ROUGE;
}

void afficher_objectif_vert(){
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*OBJECTIF_TILEY + OBJECTIF_TILEX] = OFF_OBJECTIF_VERT;
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*OBJECTIF_TILEY + OBJECTIF_TILEX+1] = 1+OFF_OBJECTIF_VERT;
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*(OBJECTIF_TILEY+1) + OBJECTIF_TILEX] = 2+OFF_OBJECTIF_VERT;
	BG_MAP_RAM(1)[SCREEN_TILE_WIDTH*(OBJECTIF_TILEY+1) + OBJECTIF_TILEX+1] = 3+OFF_OBJECTIF_VERT;
}

//ajoute un obstacle à la liste d'obstacles à un endroit donné
void ajouter_obstacle(obstacle** head, int tileX, int tileY){
	//Ne pas ajouter si l'obstacle n'est pas dans les limites du jeu
	if(tileX < 5 || SCREEN_TILE_WIDTH-6 < tileX || tileY < 1 || SCREEN_TILE_HEIGHT-3 < tileY){return;}

	//Ne pas ajouter si un obstacle se trouve deja dans la position donne
	obstacle* it = *head;
	int i;
	for(i=0; it!=NULL && i<MAX_OBSTACLES; ++i){
		if(it->tileX-1 <= tileX && tileX <= it->tileX+1 && it->tileY-1 <= tileY && tileY <= it->tileY+1) {return;}
		it = it->next;
	}

	//ajouter l'obstacle
	obstacle* new = malloc(sizeof(obstacle));
	if (new == NULL){}//Erreur?
	new->tileX = tileX;
	new->tileY = tileY;
	new->next = *head;
	*head = new;
}

//ajoute un nombre d'obstacles à la liste d'obstacles dans une région (block) donnée
void configuration_block_d_obstacles(obstacle** head, int block_tileX, int block_tileY, int height, int width, int nombre){

	int randomX;
	int randomY;
	int n;

	for(n=0; n<nombre; ++n){
		randomX = rand() % width;
		randomY = rand() % height;
		ajouter_obstacle(head, block_tileX + randomX, block_tileY + randomY);
	}
}

//configuration des obstacles selon la difficulté du jeu
void configuration_obstacles(obstacle** head, int difficulte){

	switch (difficulte){
		case 1:
			configuration_block_d_obstacles(head, 5, 3, 10, 10, 2);
			configuration_block_d_obstacles(head, 15, 3, 10, 10, 2);
			configuration_block_d_obstacles(head, 5, 13, 10, 10, 2);
			configuration_block_d_obstacles(head, 15, 13, 10, 10, 2);
			break;

		case 2:
			configuration_block_d_obstacles(head, 5, 3, 10, 10, 4);
			configuration_block_d_obstacles(head, 17, 1, 10, 10, 4);
			configuration_block_d_obstacles(head, 5, 13, 10, 10, 4);
			configuration_block_d_obstacles(head, 17, 11, 10, 10, 4);
			break;

		default: //3 ou plus
			configuration_block_d_obstacles(head, 5, 3, 10, 10, 6);
			configuration_block_d_obstacles(head, 17,1, 10, 10, 6);
			configuration_block_d_obstacles(head, 5, 13, 10, 10, 6);
			configuration_block_d_obstacles(head, 17, 11, 10, 10, 6);
	}
}

//affiche tout les obstacles de la liste
void afficher_obstacles(obstacle* head){
	obstacle* it = head;
	int i;
	for(i=0; it!=NULL && i<MAX_OBSTACLES; ++i){
		BG_MAP_RAM(1)[32*it->tileY + it->tileX] = OFF_OBSTACLE;
		BG_MAP_RAM(1)[32*it->tileY + it->tileX+1] = 1+OFF_OBSTACLE;
		BG_MAP_RAM(1)[32*(it->tileY+1) + it->tileX] = 2+OFF_OBSTACLE;
		BG_MAP_RAM(1)[32*(it->tileY+1) + it->tileX+1] = 3+OFF_OBSTACLE;
		it = it->next;
	}
}

//affiche le bord de piques
void afficher_bord(){
	int tileX;
	int tileY;

	for(tileX=0; tileX<SCREEN_TILE_WIDTH; ++tileX){
		BG_MAP_RAM(1)[tileX + (SCREEN_TILE_HEIGHT-1)*SCREEN_TILE_WIDTH] = OFF_PIQUE_SIMPLE;
		BG_MAP_RAM(1)[tileX] = OFF_PIQUE_SIMPLE + 2;
	}
	for(tileY=1; tileY<SCREEN_TILE_HEIGHT-1; ++tileY){
		BG_MAP_RAM(1)[tileY*SCREEN_TILE_WIDTH] = OFF_PIQUE_SIMPLE + 1;
		BG_MAP_RAM(1)[SCREEN_TILE_WIDTH-1 + tileY*SCREEN_TILE_WIDTH] = OFF_PIQUE_SIMPLE + 3;
	}
}

//retourne vrais si et seulement si le sprite du ballon touche le bord de piques
int collision_bord(int ballonX, int ballonY){
    return !((ballonX < (SCREEN_WIDTH - BALLON_HITBOX_WIDTH - 6))
    		&& (ballonY < (SCREEN_HEIGHT - BALLON_HITBOX_HEIGHT - 6))
    		&& (ballonX  > 6)
    		&& (ballonY  > 6));
}

//retourne vrais si et seulement si le sprite du ballon touche un des obstacles
int collision_obstacle(obstacle* head, int ballonX, int ballonY){
	obstacle* it = head;
	int i;
	for(i=0; it!=NULL && i<MAX_OBSTACLES; ++i){//obstacle en forme de croix suisse
		if(((ballonX < (it->tileX)*8 + 7)
	    	&& (ballonY < (it->tileY)*8 + 12)
	    	&& (ballonX  > (it->tileX)*8 - 7)
	    	&& (ballonY  > (it->tileY)*8 - 12))
	    	||
	    	((ballonX < (it->tileX)*8 + 12)
	    	&& (ballonY < (it->tileY)*8 + 7)
	    	&& (ballonX  > (it->tileX)*8 - 12)
	    	&& (ballonY  > (it->tileY)*8 - 7))){
	    		return 1;
	    	}
		it = it->next;
	}
	return 0;
}

//retourne vrais si et seulement si le sprite du ballon touche l'objectif
int collision_objectif(int ballonX, int ballonY){
	return ((ballonX < (OBJECTIF_TILEX*8 + 10))
    		&& (ballonY < (OBJECTIF_TILEY*8 + 10))
    		&& (ballonX  > (OBJECTIF_TILEX*8 - 10))
    		&& (ballonY  > (OBJECTIF_TILEY*8 - 10)));
}



void mini_jeu_ballon(game_status* status)
{
	//Initialisation variables
	int ballonX = BALLON_STARTX, ballonY = BALLON_STARTY, keys;
	bool echec = false;
	bool succes = false;

	//Initialisation temps
	configuration_temp_total(status);

	//Initialisation des obstacles
	obstacle* obstacles_head = NULL;
	configuration_obstacles(&obstacles_head, status->difficulte->nombre);

	//Affichage
	afficher_bord();
	afficher_obstacles(obstacles_head);
	afficher_objectif_rouge();

	//Initialisation de sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	swiCopy(spriteBallonPal, SPRITE_PALETTE, spriteBallonPalLen/2);
	swiCopy(spriteBallonTiles, gfx_ballon, spriteBallonTilesLen/2);

	//Boucle principal du mini jeu
	while(!echec && !succes){

		//Affiche la barre du temps
		upper_afficher_barre((int)(32*status->minigame_left_time/status->minigame_total_time));

	    //Read held keys
	    scanKeys();
	    keys = keysHeld();

	    //Modify position of the sprite accordingly
	    if(keys & KEY_RIGHT) ballonX+=2;
	    if(keys & KEY_DOWN) ballonY+=2;
	    if(keys & KEY_LEFT) ballonX-=2;
	    if(keys & KEY_UP) ballonY-=2;

	    oamSet(&oamMain, 	 // oam handler
	    	0,				 // Number of sprite
	    	ballonX, ballonY,// Coordinates
	    	0,				 // Priority
	    	0,				 // Palette to use
	    	SpriteSize_16x32,			// Sprite size
	    	SpriteColorFormat_256Color,	// Color format
	    	gfx_ballon,		// Loaded graphic to display
	    	-1,				// Affine rotation to use (-1 none)
	    	false,			// Double size if rotating
	    	false,			// Hide this sprite
	    	false, false,	// Horizontal or vertical flip
	    	false			// Mosaic
	    	);
	    swiWaitForVBlank();
	    //Update the sprites
		oamUpdate(&oamMain);

		if(collision_bord(ballonX, ballonY) || collision_obstacle(obstacles_head, ballonX, ballonY) || status->minigame_left_time <= 0){
			echec = true;
			update_vie(status, status->vie_restante-1);
			if(collision_bord(ballonX, ballonY) || collision_obstacle(obstacles_head, ballonX, ballonY)){
				//Copie dans la memoire de la bitmap et palette du ballon casse
				swiCopy(spriteBallonCassePal, SPRITE_PALETTE, spriteBallonCassePalLen/2);
				swiCopy(spriteBallonCasseTiles, gfx_ballon, spriteBallonCasseTilesLen/2);
			}
			mmEffect(SFX_ECHEC);
		}

		if(collision_objectif(ballonX, ballonY)){
			succes = true;
			afficher_objectif_vert();
			status->score->nombre += 1;
			status->difficulte->nombre = (status->difficulte->nombre % 3) + 1;
			if(status->difficulte->nombre == 1) status->vitesse->nombre += 1;
			mmEffect(SFX_VICTOIRE);
		}
	}

	//2 seconde d'attente afin de pouvoir constater le resultat du jeu
	Attendre(2);

	//cacher le sprite
	oamSet(&oamMain, 	     // oam handler
		    0,				 // Number of sprite
		    ballonX, ballonY,// Coordinates
		    0,				 // Priority
		    0,				 // Palette to use
		    SpriteSize_16x32,			// Sprite size
		    SpriteColorFormat_256Color,	// Color format
		    gfx_ballon,		// Loaded graphic to display
		    -1,				// Affine rotation to use (-1 none)
		    false,			// Double size if rotating
		    true,			// Hide this sprite
		    false, false,	// Horizontal or vertical flip
		    false			// Mosaic
		    );
	oamUpdate(&oamMain);

	//liberation de la memoire
	obstacle* temp;
	while(obstacles_head != NULL){
		temp = obstacles_head;
		obstacles_head = obstacles_head->next;
		free(temp);
	}

}
