#include "Graphics.h"
#include "background_upper.h"
#include "titleScreen.h"
#include "background_wood.h"
#include "background_sky.h"
#include "belowTiles.h"
#include "upperTiles.h"
#include "jeu.h"

//Couleurs de la barre du temps
#define TIME_COLOR_0 RGB15(15,15,15)
#define TIME_COLOR_1 RGB15(0,31,0)
#define TIME_COLOR_2 RGB15(0,0,31)

void upper_ini_ingame_screen()
{
	/*
	 * Mise a disposition de la RAM memory bank pour le sub engine
	 */
	VRAM_C_CR = VRAM_ENABLE
			| VRAM_C_SUB_BG;


	/*
	 * Configuration du background BG3 en tiled mode, 32x32 grille and 256 couleurs
	 */
	BGCTRL_SUB[3] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//Copie dans la memoire des tiles, map et palettes.
	dmaCopy(background_upperTiles, BG_TILE_RAM_SUB(1), background_upperTilesLen);
	dmaCopy(background_upperMap, BG_MAP_RAM_SUB(0), background_upperMapLen);
	dmaCopy(background_upperPal, BG_PALETTE_SUB, background_upperPalLen);


	/*
	 * Configuration du background BG2 en tiled mode, 32x32 grille and 256 couleurs
	 */
	BGCTRL_SUB[2] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(1) | BG_TILE_BASE(2);

	//Copie dans la memoire des tiles, map et palettes.
	dmaCopy(upperTilesTiles, BG_TILE_RAM_SUB(2), upperTilesTilesLen);
	int i,j;
	for(i=32; i<upperTilesTilesLen; i++){
		BG_TILE_RAM_SUB(2)[i] += background_upperPalLen << 8 | background_upperPalLen;
	}
	dmaCopy(upperTilesMap, BG_MAP_RAM_SUB(1), upperTilesMapLen);
	dmaCopy(upperTilesPal, &BG_PALETTE_SUB[background_upperPalLen], upperTilesPalLen);

	//Initialisation de la map en transparant
	for(j=0; j<32; ++j){
		for(i=0; i<32; ++i){
			BG_MAP_RAM_SUB(1)[32*j + i] = 0;
		}
	}


	/*
	 * activation background 2 et 3
	 */
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;
}

//Affiche un chiffre sur le SUB engine
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

//Affiche un nombre sur le SUB engine
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

//Cache un compteur (pas utilisé dans le projet au final)
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

//Cache la vie (pas utilisé dans le projet au final)
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

//dessine la barre restante avec la couleur
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

//Cache la barre (pas utilisé dans le projet au final)
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

//configuration du MAIN graphical engine en ROTOSCALE mode pour le wait et title screen
void below_ini_transi_screen(){
	/*
	 * Mise a disposition de la RAM memory bank pour le main engine
	 */
	// Configure the corresponding VRAM-D memory bank correctly
	VRAM_D_CR = VRAM_ENABLE	| VRAM_D_MAIN_BG;

	//Configure background BG2 en rotoscale mode avec 8-bit pixels
	BGCTRL[2] = BG_BMP_BASE(24) | BgSize_B8_256x256;

    //Set up affine matrix
    REG_BG2PA = 256;
    REG_BG2PC = 0;
    REG_BG2PB = 0;
    REG_BG2PD = 256;

	/*
	 * activation des backgrounds
	 */
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE;
}

void below_ini_wait_screen(){

	below_ini_transi_screen();

	//Copie dans la memoire de la map et des palettes.
	dmaCopy(background_woodBitmap, BG_BMP_RAM(24), background_woodBitmapLen);
	dmaCopy(background_woodPal, BG_PALETTE, background_woodPalLen);
}

void below_ini_title_screen(){

	below_ini_transi_screen();

	//Copie dans la memoire de la map et des palettes.
	dmaCopy(titleScreenBitmap, BG_BMP_RAM(24), titleScreenBitmapLen);
	dmaCopy(titleScreenPal, BG_PALETTE, titleScreenPalLen);
}


void below_ini_ingame_screen()
{
	/*
	 * Mise a disposition de la RAM memory bank pour le main engine et les sprites
	 */
	VRAM_A_CR = VRAM_ENABLE //Enable
			| VRAM_A_MAIN_BG; //Bank for the main engine
	VRAM_B_CR = VRAM_ENABLE |
			VRAM_B_MAIN_SPRITE_0x06400000; //offset pour les sprites car VRAM A est deja utilise


	/*
	 * Configuration du background BG3 en tiled mode, 32x32 grille and 256 couleurs
	 */
	BGCTRL[3] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//Copie dans la memoire des tiles, map et palettes.
	dmaCopy(background_skyTiles, BG_TILE_RAM(1), background_skyTilesLen);
	dmaCopy(background_skyMap, BG_MAP_RAM(0), background_skyMapLen);
	dmaCopy(background_skyPal, BG_PALETTE, background_skyPalLen);


	/*
	 * Configuration du background BG2 en tiled mode, 32x32 grille and 256 couleurs
	 */
	BGCTRL[2] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(1) | BG_TILE_BASE(2);

	//Copie dans la memoire des tiles, map et palettes.
	dmaCopy(belowTilesTiles, BG_TILE_RAM(2), belowTilesTilesLen);
	int i,j;
	for(i=32; i<belowTilesTilesLen; i++){
		BG_TILE_RAM(2)[i] += background_skyPalLen << 8 | background_skyPalLen;
	}
	dmaCopy(belowTilesMap, BG_MAP_RAM(1), belowTilesMapLen);
	dmaCopy(belowTilesPal, &BG_PALETTE[background_skyPalLen], belowTilesPalLen);

	//Initialisation de la map en transparant
	for(j=0; j<32; ++j){
		for(i=0; i<32; ++i){
			BG_MAP_RAM(1)[32*j + i] = 0;
		}
	}


	/*
	 * activation des backgrounds
	 */
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;
}




