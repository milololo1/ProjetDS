
#define NOMBRE_DE_MINIJEU 0; //le nombre de minijeu total, à définir au fur et à mesure de l'avancée

#define VIE_MAX 3;
#define VIE_TILEX 28;
#define VIE_TILEY 18;

#define SCORE_TILEX 5;
#define SCORE_TILEY 8;
#define DIFFICULTE_TILEX 14;
#define DIFFICULTE_TILEY 14;
#define VITESSE_TILEX 14;
#define VITESSE_TILEY 18;

#define BARRE_TILEX 31;
#define BARRE_TILEY 15;

#define BARRE_ORANGE_THRESHOLD 15;
#define BARRE_ROUGE_THRESHOLD 5;


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


void update_vie(game_status* status, int nb_vie);
