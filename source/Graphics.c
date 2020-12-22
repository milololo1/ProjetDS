#include "Graphics.h"
#include "cafeteriaTest.h"
#include "mapImpostorTest.h"
#include "upper_ingameTest.h"


void Graphics_ini()
{
	//Swap LCD pour pouvoir eventuellement utiliser le framebuffer sur le SUB
	REG_POWERCNT ^= BIT(15);

	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE //Enable
			| VRAM_A_MAIN_BG; //Bank for the main engine

	//Configure the main engine in mode 5 (2D)
	REG_DISPCNT = MODE_5_2D;


	//Enable a proper RAM memory bank for sub engine
	VRAM_C_CR = VRAM_ENABLE
			| VRAM_C_SUB_BG;
	//Configure the sub engine in mode 5 (2D)
	REG_DISPCNT_SUB = MODE_5_2D;
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
 	//activation background 3
	REG_DISPCNT_SUB = REG_DISPCNT_SUB | DISPLAY_BG3_ACTIVE;

	//Background configuration SUB
    BGCTRL_SUB[3] = BG_BMP_BASE(0) | BgSize_B8_256x256;

	//AFfine Matrix
    REG_BG3PA_SUB = 256;
    REG_BG3PC_SUB = 0;
    REG_BG3PB_SUB = 0;
    REG_BG3PD_SUB += 50;
    REG_BG3X_SUB = 0;
    REG_BG3Y_SUB = 192;

	//Copy of the palette and the bitmap
	dmaCopy(upper_ingameTestBitmap, BG_MAP_RAM_SUB(0), upper_ingameTestBitmapLen);
	dmaCopy(upper_ingameTestPal, BG_PALETTE_SUB, upper_ingameTestPalLen);
}

void Ini_below_ingame_screen()
{
 	//activation background 3
	REG_DISPCNT = REG_DISPCNT | DISPLAY_BG3_ACTIVE;

	//Background configuration MAIN ---- offset | tjrs la meme chose tkt ---
	BGCTRL[3] = BG_MAP_BASE(0) | BgSize_B8_256x256;

	//AFfine Matrix
    REG_BG3PA = 0;
    REG_BG3PC = 256;
    REG_BG3PB = 256;
    REG_BG3PD = 0;
    bgTransform[3]->dx = 100;
    REG_BG3X = 100;
    REG_BG3Y = 100;

	//Copy of the palette and the bitmap
	dmaCopy(cafeteriaTestBitmap, BG_MAP_RAM(0), cafeteriaTestBitmapLen);
	dmaCopy(cafeteriaTestPal, BG_PALETTE, cafeteriaTestPalLen);



}

void Push_Right_upper_ingame_BG2()
{
	REG_BG3X += 100;

}




