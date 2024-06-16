#pragma once
#include "Sprite.h"
#include "Joueur.h"
#include "Dysgenesis.h"

const float VITESSE_PROJECTILE = 0.95f;
static SDL_bool son_cree;

typedef struct Projectile {
	Sprite self;

	Vector3 cible;
	ProprietaireProjectile proprietaire;
	SDL_bool laser;

	float z_init;
	u8 ID;
} Projectile;

typedef enum ProprietaireProjectile {
	PROPRIETAIREPROJ_JOUEUR,
	PROPRIETAIREPROJ_ENNEMI
} ProprietaireProjectile;

Projectile* CreerProjectile(Jeu* jeu, Vector3 position, Vector3 destination, ProprietaireProjectile proprietaire, u8 ID);

SDL_bool ExistProjectile(Projectile* projectile);

void TrouverCible(Projectile* projectile);

int PositionsSurEcran(Projectile* projectile, float* output);

int CollisionProjectileJoueur(Projectile* projectile);

void RenderProjectile(Projectile* projectile);


const int LARGEUR_MAX_VAGUE_ELECTRIQUE = 80;
const int LARGEUR_MIN_VAGUE_ELECTRIQUE = 50;
const int PRECISION_VAGUE_ELECTRIQUE = 50;
const int TEMPS_AVANT_VAGUE_FAIT_DEGATS = G_FPS / 6;
const int DURATION_VAGUE_ELECTRIQUE = 1 * G_FPS;

typedef struct VagueElectrique {
	Sprite self;
	float rayon;
} VagueElectrique;

int CreerVagueElectrique(VagueElectrique* vague);

void ExistVagueElectrique(VagueElectrique* vague);

void RenderVagueElectrique(VagueElectrique* vague);