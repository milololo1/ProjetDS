/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>
#include "Graphics.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#define NOMBRE_DE_MINIJEU 0; //le nombre de minijeu total, à définir au fur et à mesure de l'avancée
#define VIE_MAX 3;


typedef struct {
	int score;
	int vie_restante;
} game_status;

typedef struct {
	int difficulte; //0, 1, 2 (défini la difficulté du niveau)
	int vitesse; //peut aller vers l'infini (défini
} challenge;

int main(void) {
	Graphics_setup_main();
	Draw_default_screen();
	
	mmInitDefaultMem((mm_addr)soundbank_bin);

	mmLoad(MOD_TEST1);

	mmStart(MOD_TEST1, MM_PLAY_LOOP);

	/*consoleDemoInit();
    printf("\nTemplate nds\n"); */


	/*
	 *
	 *
	 * ici création du menu (bouton pour lancer le jeu + bg si nécessaire)
	 *
	 *
	 */


	/*
	 *
	 *
	 *
	 * le jeu est lancé, on entre dans une boucle while qui lance des jeux
	 *
	 *
	 */



	/*
	 *
	 *
	 *
	 * le jeu a été perdu, on affiche un écran particulier avec un bouton pour
	 * revenir au menu
	 *
	 *
	 */

    while(1)
        swiWaitForVBlank();	
}
