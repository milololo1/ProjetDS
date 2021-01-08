#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "ballon.h"
#include "spriteBallon.h"
#include "Graphics.h"
#include "Timer.h"


void update_time_ballon(game_status* status){ //fonction qui tient compte du temps pour afficher la barre

	float delta = status->minigame_current_total_time / status->minigame_current_time;
	int barre_size = 32*delta;
	upper_afficher_barre(barre_size);
}

void configuration_Sprites()
{
	//Initialisation de sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_32, false);

	//Allocation de l'espace pour le graph afin d'afficher le sprite
	gfx_ballon = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);

	//Copie dans la memoire de la bitmap et palette
	swiCopy(spriteBallonPal, SPRITE_PALETTE, spriteBallonPalLen/2);
	swiCopy(spriteBallonTiles, gfx_ballon, spriteBallonTilesLen/2);
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

void configuration_obstacles(obstacle** head, int difficulte){

	switch (difficulte){
		case 1:
			configuration_block_d_obstacles(head, 5, 3, 10, 10, 3);
			configuration_block_d_obstacles(head, 15, 3, 10, 10, 3);
			configuration_block_d_obstacles(head, 5, 13, 10, 10, 3);
			configuration_block_d_obstacles(head, 15, 13, 10, 10, 3);
			break;

		case 2:
			configuration_block_d_obstacles(head, 5, 3, 10, 10, 6);
			configuration_block_d_obstacles(head, 17, 1, 10, 10, 6);
			configuration_block_d_obstacles(head, 5, 13, 10, 10, 6);
			configuration_block_d_obstacles(head, 17, 11, 10, 10, 6);
			break;

		default: //3 ou plus
			configuration_block_d_obstacles(head, 5, 3, 10, 10, 9);
			configuration_block_d_obstacles(head, 17,1, 10, 10, 9);
			configuration_block_d_obstacles(head, 5, 13, 10, 10, 9);
			configuration_block_d_obstacles(head, 17, 11, 10, 10, 9);
	}
}

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

int collision_bord(int ballonX, int ballonY){
    return !((ballonX < (SCREEN_WIDTH - BALLON_HITBOX_WIDTH - 6))
    		&& (ballonY < (SCREEN_HEIGHT - BALLON_HITBOX_HEIGHT - 6))
    		&& (ballonX  > 6)
    		&& (ballonY  > 6));
}

int collision_obstacle(obstacle* head, int ballonX, int ballonY){
	obstacle* it = head;
	int i;
	for(i=0; it!=NULL && i<MAX_OBSTACLES; ++i){
		if((ballonX < ((it->tileX)*8 + 14))
	    	&& (ballonY < (it->tileY)*8 + 14)
	    	&& (ballonX  > (it->tileX)*8 - 14)
	    	&& (ballonY  > (it->tileY)*8 - 14)){
	    		return 1;
	    	}
		it = it->next;
	}
	return 0;
}

int collision_objectif(int ballonX, int ballonY){
	return ((ballonX < (OBJECTIF_TILEX*8 + 12))
    		&& (ballonY < (OBJECTIF_TILEY*8 + 12))
    		&& (ballonX  > (OBJECTIF_TILEX*8 - 12))
    		&& (ballonY  > (OBJECTIF_TILEY*8 - 12)));
}

void mini_jeu_ballon(game_status* status)
{

	//Inisialisation variables
	int ballonX = BALLON_STARTX, ballonY = BALLON_STARTY, keys;

	//Initialisation des obstacles
	obstacle* obstacles_head = NULL;
	configuration_obstacles(&obstacles_head, 2);

	//Affichage
	afficher_bord();
	afficher_obstacles(obstacles_head);
	afficher_objectif_rouge();

	//Configuration des sprites et initialisation des graphiques
	configuration_Sprites();


	bool echec = false;
	bool succes = false;

	status->minigame_current_total_time = 320; //arbitraire, à modifier avec la vitesse
	status->minigame_current_time = 320;

	//AnnexeCounter(status->minigame_current_total_time, status);


	while(!echec && !succes){

		update_time_ballon(status);

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

		if(collision_bord(ballonX, ballonY) || collision_obstacle(obstacles_head, ballonX, ballonY)){
			echec = true;
			update_vie(status, status->vie_restante-1);
		}

		if(collision_objectif(ballonX, ballonY)){
			succes = true;
			afficher_objectif_vert();
			status->score->nombre += 1;
		}
	}

	//1 seconde d'attente afin de pouvoir constater le resultat du jeu
	Attendre(1);

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
	int i;
	obstacle* temp;
	for(i=0; obstacles_head!=NULL && i<MAX_OBSTACLES; ++i){
		temp = obstacles_head;
		obstacles_head = obstacles_head->next;
		free(temp);
	}

}
