/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>

int main(void) {
	Graphics_setup_main();
	Draw_default_screen();
	
	/*consoleDemoInit();
    printf("\nTemplate nds\n"); */

    while(1)
        swiWaitForVBlank();	
}
