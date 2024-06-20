#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <time.h>

#include "Ennemi.h"
#include "Projectile.h"
#include "Misc.h"

#define G_FPS 60
#define MAX_PROFONDEUR 50
#define W_LARGEUR 1920
#define W_HAUTEUR 1080
#define W_SEMI_LARGEUR (W_LARGEUR / 2)
#define W_SEMI_HAUTEUR (W_HAUTEUR / 2)

#define NB_ENNEMIS 30
#define NB_PROJECTILES 50
#define NB_ITEMS 10
#define NB_EXPLOSIONS 30

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;

typedef struct Vector2 {
	float x;
	float y;
} Vector2;

typedef struct Vector3 {
	float x;
	float y;
	float z;
} Vector3;

typedef struct Jeu {
	SDL_Window* fenetre;
	SDL_Renderer* render;
	SDL_Event* event;

	Joueur* joueur;
	Ennemi* ennemis;
	Projectile* projectiles;
	Explosion* explosions;
	Item* items;

	VagueElectrique* vague_electrique;
	Curseur* curseur;
	BombePulsar* bombe;
	Son* son;

	i32 gTimer;

	Vector2* etoiles;
	SDL_bool bouger_etoiles;
	SDL_bool en_cours;
	SDL_bool arcade_debloque;
	u8 arcade_etapes;
	clock_t temps_image;
	Gamemode gamemode;
	i32 touches_pesees;
	i32 niveau;
	i32 niveau_continue;
	i32 ennemis_restant;
	i32 ennemis_tues;
	SDL_Color couleure_fond_ecran;
} Jeu;

typedef enum Gamemode {
	GAMEMODE_MENU_PRINCIPAL,
	GAMEMODE_AVENTURE,
	GAMEMODE_ARCADE,
	GAMEMODE_SCENE_INITIALIZATION,
	GAMEMODE_SCENE_DEBUT,
	GAMEMODE_SCENE_BONNE_FIN,
	GAMEMODE_SCENE_MAUVAISE_FIN,
	GAMEMODE_SCENE_GENERIQUE
} Gamemode;

typedef enum Touches {
	TOUCHE_W = 1,
	TOUCHE_A = 2,
	TOUCHE_S = 4,
	TOUCHE_D = 8,
	TOUCHE_J = 16,
	TOUCHE_K = 32,
	TOUCHE_M = 64,
	TOUCHE_R = 128,
	TOUCHE_C = 256,
	TOUCHE_E = 512,
	TOUCHE_PLUS = 1024,
	TOUCHE_MOINS = 2048
} Touche;

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