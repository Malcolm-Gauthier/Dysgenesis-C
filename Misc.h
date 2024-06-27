#pragma once
#include "Definitions.h"
#include "Projectile.h"
#include "Joueur.h"

void DessinerBombePulsar(SDL_Renderer* render, Vector2 position, u8 rayon, SDL_Color couleure, SDL_bool hyperbole_bleue);

int AnimationExplosionBombe(BombePulsar* bombe);

int CollisionBombeProjectile(BombePulsar* bombe, Jeu* jeu);

void ExistBombe(BombePulsar* bombe);


int CreerEtoiles(Vector2* etoiles, i32 limite, SDL_Rect bounds);

void BougerEtoiles(Vector2* etoiles, i32 nb_etoiles);

void RenderEtoiles(SDL_Renderer* render, Vector2* etoiles, i32 nb_etoiles);


static int GetListEntry(char c);

void DisplayText(Jeu* jeu, char* text, Vector2 position, float size, int color, int alpha, int scroll);


void Scene0(Jeu* jeu, i32 gTimer);

void Scene4(Jeu* jeu, i32 gTimer);


Curseur* CreerCurseur(Jeu* jeu);

SDL_bool ExistCurseur(Curseur* curseur);


Explosion* CreerExplosion(Jeu* jeu, Vector3 position);

SDL_bool ExistExplosion(Explosion* explosion);

void RenderExplosion(Explosion* explosion);


int InitSDLMixer(Son* son);

int JouerMusique(Jeu* jeu, Musique musique_a_jouer, SDL_bool boucle);

int JouerEffet(Jeu* jeu, EffetAudio effet_a_jouer);

void ChangerVolume(Jeu* jeu);

void RenderVolume(Jeu* jeu);