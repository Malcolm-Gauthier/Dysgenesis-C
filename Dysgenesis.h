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

SDL_bool TouchePesee(Jeu* jeu, Touche touche);

SDL_bool GamemodeAction(Jeu* jeu);

i32 NbEnnemis(Jeu* jeu);

void ClearEnnemis(Jeu* jeu);
void ClearProjectiles(Jeu* jeu);
void ClearExplosions(Jeu* jeu);
void ClearItems(Jeu* jeu);

float DistanceV3(Vector3 a, Vector3 b);

float DistanceV2(Vector2 a, Vector2 b);

i32 RNG(i32 min, i32 max);

void DessinerCercle(SDL_Renderer* render, Vector2 position, i32 rayon, i32 precision);