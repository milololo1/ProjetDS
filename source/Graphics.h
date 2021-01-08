#pragma once

#include <nds.h>

#define SCREEN_WIDTH	256
#define	SCREEN_HEIGHT	192

#define SCREEN_TILE_WIDTH	32
#define	SCREEN_TILE_HEIGHT	24

//Offset des tiles dans upperTiles
#define OFF_CHIFFRES_LIGNE1	1
#define OFF_CHIFFRES_LIGNE2	22
#define OFF_CHIFFRES_LIGNE3 42
#define OFF_COEUR 62
#define OFF_BARRE_COULEUR 66

//Offset des tiles dans belowTiles
#define OFF_PIQUE_SIMPLE 1
#define OFF_OBSTACLE 6
#define OFF_OBJECTIF_ROUGE 10
#define OFF_OBJECTIF_VERT 14


/*
	This function does not receive any input parameter. It initializes the main
	graphical engine to be used.
	It also enables the corresponding VRAM bank to be used.
*/
void Graphics_ini();

//++ A commenter
void upper_ini_ingame_screen();
void below_ini_ingame_screen();

void upper_afficher_compteur();
void upper_cacher_compteur();
void upper_afficher_nombre();
void upper_afficher_chiffre();

void upper_afficher_vie();
void upper_cacher_vie();

void upper_afficher_barre();
void upper_cacher_barre();


//--------------------Truc pour le buffer peut etre???----------------------------------------
enum BUFFER_TYPE
{
    MAIN,
    SUB,
};
//members
u16* Graphics_mainBuffer;
u16* Graphics_subBuffer;
int Graphics_mainW;
int Graphics_mainH;
int Graphics_subW;
int Graphics_subH;
/*
 	 Function that assigns the buffer (already implemented) and its size
 */
void Graphics_assignBuffer(enum BUFFER_TYPE bT, u16* buffer, int w, int h);

