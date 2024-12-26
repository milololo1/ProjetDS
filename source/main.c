/*
 * Projet Nintendo DS
 * Léonard Vaney (284023), Aloïs Mattei (274617)
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

#include "belowTiles.h"


/*
 * Affiche l'écran d'attente pendant quelques secondes pour que le joueur
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

	//Initialisation des graphiques
	REG_POWERCNT ^= BIT(15); //Swap LCD pour pouvoir avoir plus d'options (et de banks) pour les mini-jeux
	upper_ini_ingame_screen();
	below_ini_ingame_screen();
	below_ini_title_screen();

	//Allocation de l'espace pour le graph afin d'afficher le sprite
	gfx_ballon = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);

	//Initialisation des variables
	bool lancement_du_jeu;

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

	status.difficulte->nombre = 1;
	status.vitesse->nombre = 1;
	status.vie_restante = VIE_MAX;
	upper_ini_ingame_screen();
	upper_afficher_compteur(status.score);
	upper_afficher_vie(&status);
	upper_afficher_compteur(status.difficulte);
	upper_afficher_compteur(status.vitesse);
	upper_afficher_barre(32);

	srand(time(NULL)); //IMPORTANT (à faire une seule fois)

	//Initialisation du son
	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoad(MOD_MARIOPAINT);
	mmLoadEffect(SFX_ECHEC);
	mmLoadEffect(SFX_VICTOIRE);

	mmStart(MOD_MARIOPAINT, MM_PLAY_LOOP);


	//while général qui contient tout le déroulement du jeu
	while(1){

		//while qui gère le retour après une défaite
		while(!handleKeysMenu()){
			swiWaitForVBlank();
		}
		lancement_du_jeu = false;

		//réinitialisation du score
		status.score->nombre = 0;
		upper_ini_ingame_screen();
		upper_afficher_compteur(status.score);
		upper_afficher_vie(&status);
		upper_afficher_compteur(status.difficulte);
		upper_afficher_compteur(status.vitesse);
		upper_afficher_barre(32);

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

		//modifie l'écran pour remettre celui du début
		below_ini_title_screen();

		//réaffiche correctement l'écran du haut
		status.difficulte->nombre = 1;
		status.vitesse->nombre = 1;
		status.vie_restante = VIE_MAX;
		upper_ini_ingame_screen();
		upper_afficher_compteur(status.score);
		upper_afficher_vie(&status);
		upper_afficher_compteur(status.difficulte);
		upper_afficher_compteur(status.vitesse);
		upper_afficher_barre(32);
	}
}
