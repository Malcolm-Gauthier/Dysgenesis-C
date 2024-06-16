#pragma once
#include "Sprite.h"

typedef enum TypeEnnemi {
    TYPEENNEMI_OCTAHEDRON,
    TYPEENNEMI_DIAMANT,
    TYPEENNEMI_TOURNANT,
    TYPEENNEMI_ENERGIE,
    TYPEENNEMI_CROISSANT,
    TYPEENNEMI_DUPLIQUEUR,
    TYPEENNEMI_PATRA,
    TYPEENNEMI_OCTAHEDRON_DUR,
    TYPEENNEMI_DIAMANT_DUR,
    TYPEENNEMI_TOURNANT_DUR,
    TYPEENNEMI_ENERGIE_DUR,
    TYPEENNEMI_CROISSANT_DUR,
    TYPEENNEMI_DUPLIQUEUR_DUR,
    TYPEENNEMI_PATRA_DUR,
    TYPEENNEMI_BOSS,
    TYPEENNEMI_PATRA_MINION,
    TYPEENNEMI_PATRA_MINION_DUR
} TypeEnnemi;

typedef enum StatusEnnemi {
    STATUSENNEMI_VIDE = 0,
    STATUSENNEMI_INITIALIZATION,
    STATUSENNEMI_NORMAL,
    STATUSENNEMI_MORT,

    STATUSENNEMI_DUPLIQUEUR_0_RESTANT = 60,
    STATUSENNEMI_DUPLIQUEUR_1_RESTANT,
    STATUSENNEMI_DUPLIQUEUR_2_RESTANT,

    STATUSENNEMI_PATRA_0_RESTANT = 70,
    STATUSENNEMI_PATRA_1_RESTANT,
    STATUSENNEMI_PATRA_2_RESTANT,
    STATUSENNEMI_PATRA_3_RESTANT,
    STATUSENNEMI_PATRA_4_RESTANT,
    STATUSENNEMI_PATRA_5_RESTANT,
    STATUSENNEMI_PATRA_6_RESTANT,
    STATUSENNEMI_PATRA_7_RESTANT,
    STATUSENNEMI_PATRA_8_RESTANT,

    STATUSENNEMI_BOSS_INIT = 150,
    STATUSENNEMI_BOSS_INIT_2,
    STATUSENNEMI_BOSS_INIT_3,
    STATUSENNEMI_BOSS_NORMAL,
    STATUSENNEMI_BOSS_MORT,
    STATUSENNEMI_BOSS_MORT_2,
    STATUSENNEMI_BOSS_MORT_3,
} StatusEnnemi;

typedef struct Ennemi {
	Sprite self;
    TypeEnnemi type;
    StatusEnnemi statut;

	i32 HP;
    i32 largeur;

    float intervale_tir;
    float vitesse;
    float vitesse_z;

    Vector2 velocite;
    Vector2 cible;

    Vector3* modele_en_marche;
} Ennemi;

const int NB_TYPES_ENNEMIS = 17;
const int BOSS_MAX_HP = 150;
const int DISTANCE_DE_BORD_EVITER_INIT = 200;
const float VITESSE_MOYENNE_ENNEMI = 0.4f;
const float VITESSE_MOYENNE_Z_ENNEMI = 2.5f;
const float VITESSE_MOYENNE_TIR_ENNEMI = 0.2f;
const float ENNEMI_FRICTION = 0.8f;

Ennemi* CreerEnnemi(Jeu* jeu, TypeEnnemi type, StatusEnnemi statut, Ennemi* parent);

SDL_bool ExistEnnemi(Ennemi* ennemi);

void ActualiserModeleEnnemi(Ennemi* ennemi);

SDL_bool CodeBoss(Ennemi* ennemi);

int CollisionProjectileEnnemi(Ennemi* ennemi, Projectile* projectile);

int CollisionJoueurEnnemi(Ennemi* ennemi, Joueur* joueur);

Vector2 TrouverCibleEnnemi(Ennemi* ennemi);

void BougerEnnemi(Ennemi* ennemi);

void TirEnnemi(Ennemi* ennemi);

void RenderEnnemi(Ennemi* ennemi);