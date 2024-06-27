#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <time.h>

#include "Definitions.h"
#include "Ennemi.h"
#include "Projectile.h"
#include "Misc.h"
#include "Niveaux.h"
#include "Sprite.h"
#include "Item.h"
#include "Joueur.h"

int Init(Jeu* jeu);

void Controlles(Jeu* jeu);

void Code(Jeu* jeu);

void Render(Jeu* jeu);

void SDLRender(Jeu* jeu);

void FreeMem(Jeu* jeu);