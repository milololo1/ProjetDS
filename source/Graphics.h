#pragma once

#include <nds.h>

//Offset des tiles dans upperTiles
#define OFF_CHIFFRES_LIGNE1 1;
#define OFF_CHIFFRES_LIGNE2 22;
#define OFF_CHIFFRES_LIGNE3 42;
#define OFF_COEUR 62;
#define OFF_BARRE_COULEUR 66;


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

