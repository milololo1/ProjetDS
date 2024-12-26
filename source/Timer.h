#pragma once

#include <nds.h>
#include "jeu.h"

/*
 * Attendre:
 * fait attendre un nombre de secondes
 */
void Attendre(int secondes);

/*
 * AnnexeCounter:
 * initialise un compteur pour un jeu et son statut (status)
 */
void AnnexeCounter(game_status* status);
