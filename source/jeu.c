
#include "jeu.h"

void update_vie(game_status* status, int nb_vie){
	int vie_max = VIE_MAX;

	if(nb_vie > vie_max){
		status->vie_restante = vie_max;
	} else if(nb_vie < 0){
		status->vie_restante = 0;
	} else {
		status->vie_restante = nb_vie;
	}
}
