#pragma once
#include "Dysgenesis.h"

typedef struct Sprite {
	Jeu* jeu;

	Vector3* modele;
	i32 modele_longueur;
	i32* indexs_lignes_sauter;

	Vector3 position;
	SDL_Color couleure;

	float taille;
	float pitch;
	float roll;

	i32 indexs_de_tir[2];
	i32 timer;
	SDL_bool afficher;
} Sprite;

int InitSprite(Sprite* sprite, Jeu* jeu);

int PositionLigneModele(Sprite* sprite, i32 index_ligne, float* output);

int RenderSprite(Sprite* sprite);