#pragma once

#include <nds.h>
#include "jeu.h"

#define SCREEN_WIDTH	256
#define	SCREEN_HEIGHT	192

#define SCREEN_TILE_WIDTH	32
#define	SCREEN_TILE_HEIGHT	24

//Offset des tiles dans upperTiles
#define OFF_CHIFFRES_LIGNE1	1
#define OFF_CHIFFRES_LIGNE2	22
#define OFF_CHIFFRES_LIGNE3 42
#define OFF_COEUR 62
#define OFF_BARRE_COULEUR 66

//Offset des tiles dans belowTiles
#define OFF_PIQUE_SIMPLE 1
#define OFF_OBSTACLE 6
#define OFF_OBJECTIF_ROUGE 10
#define OFF_OBJECTIF_VERT 14



/*
 * upper_ini_ingame_screen:
 * configuration du SUB graphical engine en Tile mode avec les backgrounds 3 et 2 activés
 * et mise a disposition de la RAM memory bank
 */
void upper_ini_ingame_screen();

/*
 * below_ini_ingame_screen:
 * configuration du MAIN graphical engine en Tile mode avec les backgrounds 3 et 2 activés
 * et mise a disposition de la RAM memory bank
 */
void below_ini_ingame_screen();

/*
 * below_ini_title_screen:
 * configuration du MAIN graphical engine en ROTOSCALE mode avec le background 2 activé,
 * copie en mémoire de l'image titre (title_screen)
 * et mise a disposition de la RAM memory bank
 */
void below_ini_title_screen();

/*
 * below_ini_wait_screen:
 * configuration du MAIN graphical engine en ROTOSCALE mode avec le background 2 activé,
 * copie en mémoire de l'image d'attente (wait_screen)
 * et mise a disposition de la RAM memory bank
 */
void below_ini_wait_screen();

/*
 * upper_afficher_compteur:
 * affiche un compteur (cpt) sur le SUB engine
 */
void upper_afficher_compteur(compteur* cpt);

/*
 * upper_afficher_vie:
 * affiche la vie d'un jeu (status) sur le SUB engine
 */
void upper_afficher_vie(game_status* status);

/*
 * upper_afficher_barre:
 * affiche la barre restante sur le SUB engine
 */
void upper_afficher_barre(int barre_restante);
