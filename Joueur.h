#pragma once
#include "Sprite.h"
#include "Item.h"
#include "Dysgenesis.h"

const float JOUEUR_MAX_VAGUES = 3.0f;
const float VAGUE_ELECTRIQUE_REGENERATION = 1.0f / (30.0f * G_FPS); // ~1/1800, 1 par 30 secondes
const i32 JOUEUR_VITESSE_TIR = 25;
const i32 JOUEUR_MAX_HP = 150;
const i32 JOUEUR_DEFAULT_HP = 100;
const i32 JOUEUR_LARGEUR = 50;
const i32 JOUEUR_HAUTEUR = 20;

typedef struct Joueur {
	Sprite self;
	Vector2 velocite;
	TypeItem powerup;
	float vagues_electriques;
	i32 HP;
	i32 vitesse_tir;
} Joueur;

int CreerJoueur(Joueur* joueur);

void InitializerJoueur(Joueur* joueur);

SDL_bool ExistJoueur(Joueur* joueur);

int TirJoueur(Joueur* joueur);

void AnimationMort(Joueur* joueur);

void BougerJoueur(Joueur* joueur);

SDL_bool JoueurMort(Joueur* joueur);