/*
 */

#include <nds.h>
#include <stdio.h>

int main(void) {
	
    consoleDemoInit();
    
	printf("Bonjour");

    while(1)
    {
        swiWaitForVBlank();
    }
}


