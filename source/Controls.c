#include "Controls.h"

/*
 * A appeler en tant que fonction de polling
 * (pour le menu de départ)
 */
bool handleKeysMenu(){

	scanKeys();
	u16 keys = keysDown();

	if(keys & KEY_A){
		return true;
	}

	return false;
}
