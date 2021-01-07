#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "ballon.h"
#include "spriteBallon.h"
#include "Graphics.h"

#define	SPRITE_HITBOX_WIDTH	16
#define	SPRITE_HITBOX_HEIGHT 16

#define OBSTACLE_1 (x > 40 && x <= 56 && y > 40 && y <= 56)
#define OBSTACLE_2 (x > 80 && x <= 96 && y > 80 && y <= 96)
#define OBJECTIF (x > 120 && x <= 136 && y > 120 && y <= 136)

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

void configuration_obstacle(){
	int tileX = 6;
	int tileY = 6;
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_COEUR;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_COEUR;

	tileX = 12;
	tileY = 12;
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_COEUR;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_COEUR;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_COEUR;

}

void configuration_objectif(){
	int tileX = 18;
	int tileY = 18;
	BG_MAP_RAM(1)[32*tileY + tileX] = OFF_OBJECTIF;
	BG_MAP_RAM(1)[32*tileY + tileX+1] = 1+OFF_OBJECTIF;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX] = 2+OFF_OBJECTIF;
	BG_MAP_RAM(1)[32*(tileY+1) + tileX+1] = 3+OFF_OBJECTIF;
}


void mini_jeu_ballon(game_status* status)
{
	//Configuration des sprites et initialisation des graphiques
	//configuration_Sprites();
	//configuration_obstacle();
	configuration_objectif();

	//Afficher bellowTiles
	int i, j;
	for (j=0; j<32; ++j){
		for (i=0; i<32; ++i){
			BG_MAP_RAM(1)[32*j + i] = 32*j + i;
		}
	}

	//test
	//BG_MAP_RAM(1)[32*4 + 4] = 6;

	bool echec = false;
	bool succes = false;

	status->minigame_current_total_time = 320; //arbitraire, à modifier avec la vitesse
	status->minigame_current_time = 320;

	//AnnexeCounter(status->minigame_current_total_time, status);

	//Position
	int x = 0, y = 0, keys;
	while(!echec && !succes){

		update_time_ballon(status);

	    //Read held keys
	    scanKeys();
	    keys = keysHeld();

	    //Modify position of the sprite accordingly
	    if((keys & KEY_RIGHT) && (x < (SCREEN_WIDTH - SPRITE_HITBOX_WIDTH))) x+=2;
	    if((keys & KEY_DOWN) && (y < (SCREEN_HEIGHT - SPRITE_HITBOX_HEIGHT))) y+=2;
	    if((keys & KEY_LEFT) && (x  > 0)) x-=2;
	    if((keys & KEY_UP) && (y  > 0)) y-=2;

	    oamSet(&oamMain, 	// oam handler
	    	0,				// Number of sprite
	    	x, y,			// Coordinates
	    	0,				// Priority
	    	0,				// Palette to use
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

		if(OBSTACLE_1 || OBSTACLE_2){
			echec = true;
			update_vie(status, status->vie_restante-1);
		}

		if(OBJECTIF){
			succes = true;
			status->score->nombre += 1;
		}
	}
/*
	oamSet(&oamMain, 	// oam handler
		    0,				// Number of sprite
		    x, y,			// Coordinates
		    0,				// Priority
		    0,				// Palette to use
		    SpriteSize_16x32,			// Sprite size
		    SpriteColorFormat_256Color,	// Color format
		    gfx_ballon,		// Loaded graphic to display
		    -1,				// Affine rotation to use (-1 none)
		    false,			// Double size if rotating
		    true,			// Hide this sprite
		    false, false,	// Horizontal or vertical flip
		    false			// Mosaic
		    );
*/
	oamUpdate(&oamMain);

}
