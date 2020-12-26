#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "ballon.h"
#include "spriteBallon.h"
#include "Graphics.h"

#define SCREEN_WIDTH	256
#define	SCREEN_HEIGHT	192

#define	SPRITE_HITBOX_WIDTH	16
#define	SPRITE_HITBOX_HEIGHT 16

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
}


void mini_jeu_ballon(game_status* status)
{
	//Configuration des sprites et initialisation des graphiques
	configuration_Sprites();
	configuration_obstacle();

	//test
	//BG_MAP_RAM(1)[32*4 + 4] = 6;

	bool echec = false;

	//Position
	int x = 0, y = 0, keys;
	while(!echec){

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

		if(x > 40 && x <= 56 && y > 40 && y <= 56){
			echec = true;
		}
	}
}
