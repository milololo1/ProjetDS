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


#include "mapImpostorTest.h"
#include "cafeteriaTest.h"
#include "belowTiles.h"



/*
 * Affiche qqch pendant quelques secondes, pour que le joueur
 * se prépare au prochain mini-jeu
 */
void EcranTemporaire(){
	//Affiche le waiting screen
	below_ini_wait_screen();

	//reinitialiser les tiles en transparant
	int i, j;
	for(j=0; j<SCREEN_TILE_HEIGHT; ++j){
		for(i=0; i<SCREEN_TILE_WIDTH; ++i){
			BG_MAP_RAM(1)[32*j + i] = 0;
		}
	}

	//Attente
	for(i = 1; i <= 3; ++i){
		Attendre(1);
		printf("%i\n", i);
	}

	//Affiche le screen des mini jeux
	below_ini_ingame_screen();
}


int main(void) {

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

	Graphics_ini();
	upper_ini_ingame_screen();
	below_ini_ingame_screen();
	below_ini_title_screen();


	status.score->nombre = 0;
	upper_afficher_compteur(status.score);
	status.difficulte->nombre = 1;
	upper_afficher_compteur(status.difficulte);
	status.vitesse->nombre = 1;
	upper_afficher_compteur(status.vitesse);
	
	update_vie(&status, 3);
	upper_afficher_vie(&status);

	upper_afficher_barre(32);

	srand(time(NULL)); //IMPORTANT (à faire une seule fois)

	/*mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoad(MOD_TEST1);
	mmStart(MOD_TEST1, MM_PLAY_LOOP);*/




	//while général qui contient tout le déroulement du jeu
	while(1){

		//while qui gère le retour après une défaite
		while(!handleKeysMenu()){

				swiWaitForVBlank();
			}

			lancement_du_jeu = false;

			//boucle qui lance les jeux
			while(status.vie_restante != 0){

				int random = rand() % 2; //retourne un nombre entre 0 et 1

				EcranTemporaire(); //On fait attendre le joueur entre chaque jeu

				if(random == 0){
					mini_jeu_ballon(&status);
				}
				else{
					mini_jeu_coupe(&status);
				}

				//Actualise l'affichage de l écran du dessus
				upper_afficher_vie(&status);
				upper_afficher_compteur(status.score);
				upper_afficher_compteur(status.difficulte);
				upper_afficher_compteur(status.vitesse);
				upper_afficher_barre(32);
				swiWaitForVBlank();


			}

			status.score->nombre = 0;
			status.difficulte->nombre = 1;
			status.vitesse->nombre = 1;
			status.vie_restante = VIE_MAX;

			//modifie l'écran pour remettre celui du début
			below_ini_title_screen();

			//réaffiche correctement l'écran du haut
			upper_ini_ingame_screen();

			upper_afficher_vie(&status);
			upper_afficher_compteur(status.score);
			upper_afficher_compteur(status.difficulte);
			upper_afficher_compteur(status.vitesse);
			upper_afficher_barre(32);
	}

}
