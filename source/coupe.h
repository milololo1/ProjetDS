#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#include "Graphics.h"
#include "jeu.h"

#define SCREEN_WIDTH	256
#define	SCREEN_HEIGHT	192

#define	SPRITE_HITBOX_WIDTH	16
#define	SPRITE_HITBOX_HEIGHT 16

void mini_jeu_coupe(game_status* status);
