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
#include "ballon.h"
#include "coupe.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

/*
 * Affiche qqch pendant quelques secondes, pour que le joueur
 * se prépare au prochain mini-jeu
 */
void EcranTemporaire(){
	int i;
	for(i = 1; i <= 3; ++i){
		Attendre(1);
		printf("%i\n", i);
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

	//irqInit();
	//irqEnable(IRQ_VBLANK);

	Graphics_ini();
	upper_ini_ingame_screen();
	below_ini_ingame_screen();

	status.score->nombre = 0;
	upper_afficher_compteur(status.score);
	status.difficulte->nombre = 1;
	upper_afficher_compteur(status.difficulte);
	status.vitesse->nombre = 1;
	upper_afficher_compteur(status.vitesse);
	
	update_vie(&status, 2);
	upper_afficher_vie(&status);
	//upper_cacher_vie(&status);

	upper_afficher_barre(27);
	//upper_cacher_barre();
	//upper_afficher_barre(7);

	//consoleDemoInit();

	//mini_jeu_ballon(&status);
	//mini_jeu_coupe(&status);

	/*while(1){
		swiWaitForVBlank();
	}*/

	srand(time(NULL)); //IMPORTANT (à faire une seule fois)

	/*mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoad(MOD_TEST1);
	mmStart(MOD_TEST1, MM_PLAY_LOOP);*/


    //printf("\nTemplate nds\n");

	//while général qui contient tout le déroulement du jeu
	while(1){
		printf("press A button\n");
		/*
		*
		* ici création du menu (bouton pour lancer le jeu + bg si nécessaire)
		*
		*/
		while(!handleKeysMenu()){

				swiWaitForVBlank(); //nécessaire?
			}
			printf("jeu lance\n");
			lancement_du_jeu = false;

			//irqInit();
			//irqEnable(IRQ_VBLANK);

			while(status.vie_restante != 0){
					/*
					 *
					 *
					 *
					 * le jeu est lancé, on entre dans une boucle while qui lance des jeux
					 *
					 *
					 */

				printf("on utilise le random\n");

				int random = rand() % 2; //retourne un nombre entre 0 et 1

				printf("random number: %i\n", random);

				printf("on attend\n");

				EcranTemporaire(); //On fait attendre le joueur entre chaque jeu

				printf("on attend plus\n");

				if(random == 0){
					mini_jeu_ballon(&status);
				}
				else{
					printf("rentre ici au moins bordel\n");
					mini_jeu_coupe(&status);
				}

				//encore à faire: vider l'écran du dessous et actualiser celui du dessus

				int i, j;
				for(j=0; j<32; ++j){
					for(i=0; i<32; ++i){
						BG_MAP_RAM(1)[32*j + i] = 0;
					}
				}

				upper_afficher_vie(&status);
				upper_afficher_barre(32);
				upper_afficher_compteur(status.score);

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
