
#define NOMBRE_DE_MINIJEU 0; //le nombre de minijeu total, à définir au fur et à mesure de l'avancée
#define VIE_MAX 3;
#define SCORE_TILEX 3;
#define SCORE_TILEY 3;
#define DIFFICULTE_TILEX 6;
#define DIFFICULTE_TILEY 3;
#define VITESSE_TILEX 9;
#define VITESSE_TILEY 3;



typedef struct {
	int tileX;
	int tileY;
	int nombre;
} compteur;

typedef struct {
	compteur* score;
	int vie_restante;

	compteur* difficulte; //0, 1, 2 (défini la difficulté du niveau)
	compteur* vitesse; //peut aller vers l'infini (défini le temps donné au joueur

	int minigame_current_total_time; //en dixième de secondes (pour les deux)
	int minigame_current_time;
} game_status;

typedef struct {
} game_challenge;

