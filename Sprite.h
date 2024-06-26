#pragma once
#include "Definitions.h"

int InitSprite(Sprite* sprite, Jeu* jeu);

int PositionLigneModele(Sprite* sprite, i32 index_ligne, float* output);

int RenderSprite(Sprite* sprite);