#include "Graphics.h"
#include "cafeteriaTest.h"
#include "mapImpostorTest.h"


void Graphics_setup_main()
{
#ifdef ROTOSCALE
	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE //Enable
			| VRAM_A_MAIN_BG; //Bank for the main engine

	//Configure the main engine in mode 5 (2D) and activate Background 2
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE;


	//Enable a proper RAM memory bank for sub engine
	VRAM_C_CR = VRAM_ENABLE
			| VRAM_C_SUB_BG;
	//Configure the sub engine in mode 5 (2D) and activate Background 2
	REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE;
#endif
}


//Pas util pout l'instant (Je pense pas utiliser en vrais)
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

void Draw_default_screen()
{
 	//ROTOSCALE
	//Background configuration MAIN ---- offset | tjrs la meme chose tkt ---
	BGCTRL[2] = BG_MAP_BASE(0) | BgSize_B8_256x256;

	//AFfine Matrix
    REG_BG2PA = 256;
    REG_BG2PC = 0;
    REG_BG2PB = 0;
    REG_BG2PD = 256;

	//Copy of the palette and the bitmap
	dmaCopy(cafeteriaTestBitmap, BG_MAP_RAM(0), cafeteriaTestBitmapLen);
	dmaCopy(cafeteriaTestPal, BG_PALETTE, cafeteriaTestPalLen);

	//Background configuration SUB
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;

	//AFfine Matrix
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;

	//Copy of the palette and the bitmap
	dmaCopy(mapImpostorTestBitmap, BG_MAP_RAM_SUB(0), mapImpostorTestBitmapLen);
	dmaCopy(mapImpostorTestPal, BG_PALETTE_SUB, mapImpostorTestPalLen);

}

