#include "Graphics.h"
#include "cafeteriaTest.h"
#include "mapImpostorTest.h"
#include "upper_ingameTest.h"
#include "costumTiles.h"

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

void Ini_upper_ingame_screen()
{
	//Enable a proper RAM memory bank for sub engine
	VRAM_C_CR = VRAM_ENABLE
			| VRAM_C_SUB_BG;


 	//activation background 3
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG3_ACTIVE;

	//Configure background BG3 in tiled mode using a 32x32 grid and 256 colors
	//Background configuration ---- cours| cours | 0*2kb | 1*16kb
	BGCTRL_SUB[3] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//Copie dans la memoire des tiles, map et palettes.
	dmaCopy(upper_ingameTestTiles, BG_TILE_RAM_SUB(1), upper_ingameTestTilesLen);
	dmaCopy(upper_ingameTestMap, BG_MAP_RAM_SUB(0), upper_ingameTestMapLen);
	dmaCopy(upper_ingameTestPal, BG_PALETTE_SUB, upper_ingameTestPalLen);


 	//activation background 2 et 3
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;

	//Configure background BG2 in tiled mode using a 32x32 grid and 256 colors
	//Background configuration ---- cours| cours | 1*2kb | 5*16kb
	BGCTRL_SUB[2] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(1) | BG_TILE_BASE(5);

	//Copie dans la memoire des tiles, map et palettes.
	//Initialisation des palettes
	BG_PALETTE_SUB[253] = TIME_COLOR_0;
	BG_PALETTE_SUB[254] = TIME_COLOR_1;
	BG_PALETTE_SUB[255] = TIME_COLOR_2;

	//Copie des tiles
	dmaCopy(TIME_COLOR_0_tile, (u8*)BG_TILE_RAM_SUB(5), 64);
	dmaCopy(TIME_COLOR_1_tile, (u8*)BG_TILE_RAM_SUB(5)+64, 64);
	dmaCopy(TIME_COLOR_2_tile, (u8*)BG_TILE_RAM_SUB(5)+2*64, 64);

	//Initialisation de la map
	int i,j;
	for(j=0; j<32; ++j){
		for(i=0; i<32; ++i){
			BG_MAP_RAM_SUB(1)[32*j + i] = i;
		}
	}
}

void Ini_below_ingame_screen()
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







