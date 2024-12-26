#pragma once

#define VIE_MAX 3
#define VIE_TILEX 28
#define VIE_TILEY 7

#define SCORE_TILEX 4
#define SCORE_TILEY 7
#define DIFFICULTE_TILEX 23
#define DIFFICULTE_TILEY 13
#define VITESSE_TILEX 23
#define VITESSE_TILEY 18

#define BARRE_TILEX 31
#define BARRE_TILEY 22

#define BARRE_COULEUR1_THRESHOLD 15
#define BARRE_COULEUR2_THRESHOLD 5


typedef struct {
	int tileX;
	int tileY;
	int nombre;
} compteur;

/*
 * game_status:
 * est la structure qui contient les informations principals du jeu
 */
typedef struct {
	compteur* score;
	int vie_restante;

	compteur* difficulte; //0, 1, 2 (défini la difficulté du niveau)
	compteur* vitesse; //peut aller vers l'infini (défini le temps donné au joueur

	int minigame_total_time; //en centième de secondes (pour les deux)
	int minigame_left_time;
} game_status;

/*
 * update_vie:
 * met à jour la vie avec nb_vie donné
 */
void update_vie(game_status* status, int nb_vie);
