#pragma once
#include "Dysgenesis.h"

const int TIMER_DISPARITION_ITEM = 2 * G_FPS;
const int HP_BONUS = 10;
const float FACTEUR_VITESSE_ITEM = 0.98f;
const float POS_Z_COLLISION_JOUEUR = 2.0f;
const int NB_TYPES_ITEM = 7;

typedef enum TypeItem {
	TYPEITEM_NONE,
	TYPEITEM_X2_SHOT,
	TYPEITEM_X3_SHOT,
	TYPEITEM_HOMING,
	TYPEITEM_SPREAD,
	TYPEITEM_LASER,
	TYPEITEM_HP,
	TYPEITEM_VAGUE
} TypeItem;

typedef struct Item {
	Sprite self;
	TypeItem type;
} Item;

Item* CreerItem(Ennemi* parent);

SDL_bool ItemExist(Item* item);

int BougerItem(Item* item);

int VerifierCollisionJoueur(Item* item);

void RenderItem(Item* item);