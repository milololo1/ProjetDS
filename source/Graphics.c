#include "Graphics.h"
#include "cafeteriaTest.h"
#include "mapImpostorTest.h"
#include "upper_ingameTest.h"
#include "upperTiles.h"
#include "costumTiles.h"
#include "jeu.h"

#define TIME_COLOR_0 RGB15(15,15,15)
#define TIME_COLOR_1 RGB15(0,31,0)
#define TIME_COLOR_2 RGB15(0,0,31)


void Graphics_ini()
{
	//Swap LCD pour pouvoir eventuellement utiliser le framebuffer sur le SUB
	REG_POWERCNT ^= BIT(15);
}

//util si on utilise le framebuffer un jour
void Graphics_assignBuffer(enum BUFFER_TYPE bT, u16* buffer, int w, int h)
{
    switch(bT)
    {
        case MAIN: Graphics_mainBuffer = buffer;
            Graphics_mainW = w;
            Graphics_mainH = h;
            break;
        case SUB: Graphics_subBuffer = buffer;
            Graphics_subW = w;
            Graphics_subH = h;
            break;
    }
}

void upper_ini_ingame_screen()
{
	//Enable a proper RAM memory bank for sub engine
	VRAM_C_CR = VRAM_ENABLE
			| VRAM_C_SUB_BG;

	//Configure background BG3 in tiled mode using a 32x32 grid and 256 colors
	//Background configuration ---- cours| cours | 0*2kb | 1*16kb
	BGCTRL_SUB[3] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//Copie dans la memoire des tiles, map et palettes.
	dmaCopy(upper_ingameTestTiles, BG_TILE_RAM_SUB(1), upper_ingameTestTilesLen);
	dmaCopy(upper_ingameTestMap, BG_MAP_RAM_SUB(0), upper_ingameTestMapLen);
	dmaCopy(upper_ingameTestPal, BG_PALETTE_SUB, upper_ingameTestPalLen);

	//Configure background BG2 in tiled mode using a 32x32 grid and 256 colors
	//Background configuration ---- cours| cours | 1*2kb | 3*16kb
	BGCTRL_SUB[2] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(1) | BG_TILE_BASE(2);

	//Copie dans la memoire des tiles, map et palettes.
	dmaCopy(upperTilesTiles, BG_TILE_RAM_SUB(2), upperTilesTilesLen);
	int i,j;
	for(i=32; i<upperTilesTilesLen; i++){
		BG_TILE_RAM_SUB(2)[i] += upper_ingameTestPalLen << 8 | upper_ingameTestPalLen;
	}
	dmaCopy(upperTilesMap, BG_MAP_RAM_SUB(1), upperTilesMapLen);
	dmaCopy(upperTilesPal, &BG_PALETTE_SUB[upper_ingameTestPalLen], upperTilesPalLen);


	//Test
	for(j=0; j<32; ++j){
		for(i=0; i<32; ++i){
			BG_MAP_RAM_SUB(1)[32*j + i] = 0;
		}
	}
	//for(j=0; j<32; ++j){
	//	BG_MAP_RAM_SUB(1)[32*6 + j] = j;
	//}


 	//activation background 2 et 3 //A ACTIVER DANS LE MAIN???
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;
}

void upper_afficher_chiffre(int chiffre, int tileX, int tileY)
{
	if(tileX > 30 || tileX < 0 || tileY > 21 || tileY < 0) return;

	int chiffre_off = 2*chiffre;

	BG_MAP_RAM_SUB(1)[32*tileY + tileX] = chiffre_off + OFF_CHIFFRES_LIGNE1;
	BG_MAP_RAM_SUB(1)[32*tileY + tileX+1] = chiffre_off+1 + OFF_CHIFFRES_LIGNE1;
	BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX] = chiffre_off + OFF_CHIFFRES_LIGNE2;
	BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX+1] = chiffre_off+1 + OFF_CHIFFRES_LIGNE2;
	BG_MAP_RAM_SUB(1)[32*(tileY+2) + tileX] = chiffre_off + OFF_CHIFFRES_LIGNE3;
	BG_MAP_RAM_SUB(1)[32*(tileY+2) + tileX+1] = chiffre_off+1 + OFF_CHIFFRES_LIGNE3;
}

void upper_afficher_nombre(int nombre, int tileX, int tileY)
{
	if(tileX > 30 || tileX < 0 || tileY > 21 || tileY < 0) return;

	int unite = nombre%10;
	int dizaine = (nombre%100 - unite)/10;
	int centaine = (nombre%1000 - dizaine - unite)/100;

	int nb_chiffres = 3;
	if(centaine == 0) nb_chiffres = 2;
	if(centaine == 0 && dizaine == 0) nb_chiffres = 1;

	if(nb_chiffres == 3)	{
		upper_afficher_chiffre(centaine, tileX, tileY);
		upper_afficher_chiffre(dizaine, tileX + 2, tileY);
		upper_afficher_chiffre(unite, tileX + 4, tileY);
	}
	if(nb_chiffres == 2)	{
		upper_afficher_chiffre(dizaine, tileX, tileY);
		upper_afficher_chiffre(unite, tileX + 2, tileY);
	}
	if(nb_chiffres == 1)	{
		upper_afficher_chiffre(unite, tileX, tileY);
	}
}

void upper_afficher_compteur(compteur* cpt)
{
	upper_afficher_nombre(cpt->nombre, cpt->tileX, cpt->tileY);
}

void upper_cacher_compteur(compteur* cpt)
{
	int tileY = cpt->tileY;
	BG_MAP_RAM_SUB(1)[32*tileY + cpt->tileX] = 0;
	BG_MAP_RAM_SUB(1)[32*tileY + cpt->tileX+1] = 0;
	BG_MAP_RAM_SUB(1)[32*(tileY+1) + cpt->tileX] = 0;
	BG_MAP_RAM_SUB(1)[32*(tileY+1) + cpt->tileX+1] = 0;
	BG_MAP_RAM_SUB(1)[32*(tileY+2) + cpt->tileX] = 0;
	BG_MAP_RAM_SUB(1)[32*(tileY+2) + cpt->tileX+1] = 0;
}


void upper_afficher_vie(game_status* status)
{
	int tileX = VIE_TILEX;
	int tileY = VIE_TILEY;
	int vie_max = VIE_MAX;
	int i;
	for(i=0; i<2*status->vie_restante; i+=2){
		BG_MAP_RAM_SUB(1)[32*tileY + tileX-i] = OFF_COEUR;
		BG_MAP_RAM_SUB(1)[32*tileY + tileX+1-i] = 1+OFF_COEUR;
		BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX-i] = 2+OFF_COEUR;
		BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX+1-i] = 3+OFF_COEUR;

	}
	for(i=2*status->vie_restante; i<2*vie_max; i+=2){
		BG_MAP_RAM_SUB(1)[32*tileY + tileX-i] = 0;
		BG_MAP_RAM_SUB(1)[32*tileY + tileX+1-i] = 0;
		BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX-i] = 0;
		BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX+1-i] = 0;
	}
}

void upper_cacher_vie(game_status* status)
{
	int tileX = VIE_TILEX;
	int tileY = VIE_TILEY;
	int vie_max = VIE_MAX;
	int i;
	for(i=0; i<2*vie_max; i+=2){
		BG_MAP_RAM_SUB(1)[32*tileY + tileX-i] = 0;
		BG_MAP_RAM_SUB(1)[32*tileY + tileX+1-i] = 0;
		BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX-i] = 0;
		BG_MAP_RAM_SUB(1)[32*(tileY+1) + tileX+1-i] = 0;
	}
}

void upper_dessiner_barre(int barre_restante, int couleur)
{
	int tileX = BARRE_TILEX;
	int tileY = BARRE_TILEY;
	int i,j;
	for(i=0; i<barre_restante; ++i){
		for(j=0; j<2; j++){
			BG_MAP_RAM_SUB(1)[32*(tileY+j) + tileX-i] = couleur;
		}
	}
	for(i=barre_restante; i<32; ++i){
		for(j=0; j<2; j++){
			BG_MAP_RAM_SUB(1)[32*(tileY+j) + tileX-i] = 0;
		}
	}
}

void upper_afficher_barre(int barre_restante)
{
	/*
	BG_MAP_RAM_SUB(1)[0] = OFF_BARRE_COULEUR;
	BG_MAP_RAM_SUB(1)[1] = 1+OFF_BARRE_COULEUR;
	BG_MAP_RAM_SUB(1)[2] = 2+OFF_BARRE_COULEUR;
	 */

	if(barre_restante > 32) {
		barre_restante = 32;
	} else if(barre_restante < 0) {
		barre_restante = 0;
	}

	int couleur1_threshold = BARRE_COULEUR1_THRESHOLD;
	int couleur2_threshold = BARRE_COULEUR2_THRESHOLD;
	int couleur;

	if(barre_restante > couleur1_threshold) {
		couleur = OFF_BARRE_COULEUR;
		upper_dessiner_barre(barre_restante, couleur);
	} else if(barre_restante <= couleur1_threshold && barre_restante > couleur2_threshold){
		couleur = 1+OFF_BARRE_COULEUR;
		upper_dessiner_barre(barre_restante, couleur);
	} else {
		couleur = 2+OFF_BARRE_COULEUR;
		upper_dessiner_barre(barre_restante, couleur);
	}
}

void upper_cacher_barre()
{
	int tileX = BARRE_TILEX;
	int tileY = BARRE_TILEY;
	int i,j;
	for(i=0; i<32; ++i){
		for(j=0; j<2; j++){
			BG_MAP_RAM_SUB(1)[32*(tileY+j) + tileX-i] = 0;
		}
	}
}


void below_ini_ingame_screen()
{
	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE //Enable
			| VRAM_A_MAIN_BG; //Bank for the main engine

 	//activation background 3
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG3_ACTIVE;

	//Background configuration MAIN ---- offset | tjrs la meme chose tkt ---
	BGCTRL[3] = BG_MAP_BASE(0) | BgSize_B8_256x256;

	//AFfine Matrix
    REG_BG3PA = 256;
    REG_BG3PC = 0;
    REG_BG3PB = 0;
    REG_BG3PD = 256;

	//Copy of the palette and the bitmap
	dmaCopy(cafeteriaTestBitmap, BG_MAP_RAM(0), cafeteriaTestBitmapLen);
	dmaCopy(cafeteriaTestPal, BG_PALETTE, cafeteriaTestPalLen);

}







