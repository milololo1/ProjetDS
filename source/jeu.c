#include "jeu.h"

void update_vie(game_status* status, int nb_vie){

	if(nb_vie > VIE_MAX){
		status->vie_restante = VIE_MAX;
	} else if(nb_vie < 0){
		status->vie_restante = 0;
	} else {
		status->vie_restante = nb_vie;
	}
}
