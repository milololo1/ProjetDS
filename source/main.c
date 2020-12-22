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
	int vitesse; //peut aller vers l'infini (défini le temps donné au joueur
} game_challenge;

int main(void) {

	//à déf à l'extérieur (je sais pas encore)
	bool recharge_du_jeu = false;
	bool lancement_du_jeu = false;

	game_status status;
	status.score = 0;
	status.vie_restante = VIE_MAX;

	game_challenge challenge;
	challenge.difficulte = 0;
	challenge.vitesse = 0;

	Graphics_ini();
	Ini_upper_ingame_screen();
	Ini_below_ingame_screen();
	
	/*mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoad(MOD_TEST1);
	mmStart(MOD_TEST1, MM_PLAY_LOOP);*/

	/*consoleDemoInit();
    printf("\nTemplate nds\n"); */

	//while général qui contient tout le déroulement du jeu
	while(1){

		while(!lancement_du_jeu){
					/*
					 *
					 *
					 * ici création du menu (bouton pour lancer le jeu + bg si nécessaire)
					 *
					 *
					 */
				swiWaitForVBlank(); //nécessaire?
			}

			lancement_du_jeu = false;

			while(!status.vie_restante){
					/*
					 *
					 *
					 *
					 * le jeu est lancé, on entre dans une boucle while qui lance des jeux
					 *
					 *
					 */
				swiWaitForVBlank(); //nécessaire?
			}

			while(!recharge_du_jeu){
					/*
					 *
					 *
					 * le jeu a été perdu, on affiche un écran particulier avec un bouton pour
					 * revenir au menu
					 *
					 *
					 */
				swiWaitForVBlank(); //nécessaire?
			}
	}

}
