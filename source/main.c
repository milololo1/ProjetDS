/*
 * Template Nintendo DS
 * May 2011
 */

#include <nds.h>
#include <stdio.h>
#include "Graphics.h"
#include "Controls.h"
#include "Timer.h"
#include "jeu.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

/*
 * Affiche qqch pendant quelques secondes, pour que le joueur
 * se prépare au prochain mini-jeu
 */
void EcranTemporaire(){
	int i;
	for(i = 1; i < 10; ++i){
		printf("%i\n", i);
		Attendre(2);
	}

}


int main(void) {

	//à déf à l'extérieur (je sais pas encore)
	bool recharge_du_jeu = false;
	bool lancement_du_jeu = false;

	compteur score_cpt;
	score_cpt.tileX = SCORE_TILEX;
	score_cpt.tileY = SCORE_TILEY;
	score_cpt.nombre = 0;

	compteur difficulte_cpt;
	difficulte_cpt.tileX = DIFFICULTE_TILEX;
	difficulte_cpt.tileY = DIFFICULTE_TILEY;
	difficulte_cpt.nombre = 0;

	compteur vitesse_cpt;
	vitesse_cpt.tileX = VITESSE_TILEX;
	vitesse_cpt.tileY = VITESSE_TILEY;
	vitesse_cpt.nombre = 0;

	game_status status;
	status.score = &score_cpt;
	status.vie_restante = VIE_MAX;
	status.difficulte = &difficulte_cpt;
	status.vitesse = &vitesse_cpt;

	/*irqInit();
	irqEnable(IRQ_VBLANK);
	Timer_init();*/

	Graphics_ini();
	upper_ini_ingame_screen();

	status.score->nombre = 610;
	upper_afficher_compteur(status.score);
	status.difficulte->nombre = 2;
	upper_afficher_compteur(status.difficulte);
	status.vitesse->nombre = 34;
	upper_afficher_compteur(status.vitesse);
	
	//update_vie(&status, 2);
	upper_afficher_vie(&status);
	//upper_cacher_vie(&status);

	//upper_afficher_barre(32);

	while(1){
		swiWaitForVBlank();
	}

	/*mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoad(MOD_TEST1);
	mmStart(MOD_TEST1, MM_PLAY_LOOP);*/

	//consoleDemoInit();
    //printf("\nTemplate nds\n");

	//while général qui contient tout le déroulement du jeu
	while(1){
		printf("press A button\n");
		/*
		*
		* ici création du menu (bouton pour lancer le jeu + bg si nécessaire)
		*
		*/
		while(!lancement_du_jeu){
				lancement_du_jeu = handleKeysMenu();
				swiWaitForVBlank(); //nécessaire?
			}
			printf("jeu lance\n");
			lancement_du_jeu = false;
			EcranTemporaire();

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
