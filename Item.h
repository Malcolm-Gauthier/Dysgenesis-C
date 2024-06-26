#pragma once
#include "Definitions.h"


Item* CreerItem(Ennemi* parent);

SDL_bool ItemExist(Item* item);

int BougerItem(Item* item);

int VerifierCollisionJoueur(Item* item);

void RenderItem(Item* item);