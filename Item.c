#include "Item.h"
//#define DEBUG_ITEMS_GRATUIT

extern int InitSprite(Sprite* sprite, Jeu* jeu);

typedef struct ItemData {
	TypeItem ID;

	SDL_Color couleure;
	Vector3* modele;
	i32 modele_longueure;
	i32* sauts_modele;
} ItemData;

#define MODELE_I1_LEN 31
const Vector3 MODELE_I1[] = {
    {-5, -10, 0},
    {-15, -10, 0},
    {-20, -5, 0},
    {-5, -5, 0},
    {-5, -20, 0},
    {0, -25, 0},
    {10, -25, 0},
    {5, -20, 0},
    {-5, -20, 0},
    {5, -20, 0},
    {5, -5, 0},
    {20, -5, 0},
    {25, -10, 0},
    {10, -10, 0},
    {10, -25, 0},
    {10, -10, 0},
    {25, -10, 0},
    {25, 0, 0},
    {20, 5, 0},
    {20, -5, 0},
    {20, 5, 0},
    {5, 5, 0},
    {5, 20, 0},
    {10, 15, 0},
    {10, 5, 0},
    {10, 15, 0},
    {5, 20, 0},
    {-5, 20, 0},
    {-5, 5, 0},
    {-20, 5, 0},
    {-20, -5, 0}
};
const i32 MODELE_I1_SAUTS[] = { -1 };
#define MODELE_I3_LEN 28
const Vector3 MODELE_I3[] ={
    {-19, 22,0},
    {-70, 0,0},
    {-19, -22 ,0},
    {-19, 22 ,0},
    {-30, 12 ,0},
    {-30, -12 ,0},
    {-12, -30 ,0},
    {12, -30 ,0},
    {19, -22 ,0},
    {70, 0 ,0},
    {19, 22 ,0},
    {70, 0 ,0},
    {19, -22 ,0},
    {30, -12 ,0},
    {30, 12 ,0},
    {12, 30 ,0},
    {-12, 30 ,0},
    {-19, 22 ,0},
    {-14, -6 ,0},
    {-8, 6 ,0},
    {-8, -6 ,0},
    {-14, 6 ,0},
    {-4, -12 ,0},
    {10, -12 ,0},
    {10, 0 ,0},
    {-4, 0 ,0},
    {-4, 12 ,0},
    {10, 12 ,0}
};
const i32 MODELE_I3_SAUTS[] = { 3, 18, 20, 22, -1 };
#define MODELE_I4_LEN 29
const Vector3 MODELE_I4[] ={
    {19, 22 ,0},
    {70, 0 ,0},
    {19, -22 ,0},
    {19, 22 ,0},
    {30, 12 ,0},
    {30, -12 ,0},
    {12, -30 ,0},
    {-12, -30 ,0},
    {-19, -22 ,0},
    {-70, 0 ,0},
    {-19, 22 ,0},
    {-70, 0 ,0},
    {-19, -22 ,0},
    {-30, -12 ,0},
    {-30, 12 ,0},
    {-12, 30 ,0},
    {12, 30 ,0},
    {19, 22 ,0},
    {14, -6 ,0},
    {8, 6 ,0},
    {8, -6 ,0},
    {14, 6 ,0},
    {4, -12 ,0},
    {-10, -12 ,0},
    {-10, 0 ,0},
    {4, 0 ,0},
    {-10, 0 ,0},
    {-10, 12 ,0},
    {4, 12 ,0}
};
const i32 MODELE_I4_SAUTS[] = { 3, 18, 20, 22, -1 };
#define MODELE_I5_LEN 34
const Vector3 MODELE_I5[] ={
    {-19, 22 ,0},
    {-70, 0 ,0},
    {-19, -22 ,0},
    {-70, 0 ,0},
    {-19, 22 ,0},
    {-30, 12 ,0},
    {-30, -12 ,0},
    {-12, -30 ,0},
    {12, -30 ,0},
    {19, -22 ,0},
    {70, 0 ,0},
    {19, 22 ,0},
    {70, 0 ,0},
    {19, -22 ,0},
    {30, -12 ,0},
    {30, 12 ,0},
    {12, 30 ,0},
    {-12, 30 ,0},
    {-19, 22 ,0},
    {-16, 0 ,0},
    {-4, 0 ,0},
    {0, -4 ,0},
    {0, -16 ,0},
    {16, 0 ,0},
    {4, 0 ,0},
    {0, 4 ,0},
    {0, 16 ,0},
    {0, 10 ,0},
    {-10, 0 ,0},
    {0, -10 ,0},
    {10, 0 ,0},
    {0, 10  ,0},
    {0, 0, 0},
    {0, 0, 0}
};
const i32 MODELE_I5_SAUTS[] = { 19, 21, 23, 25, 32, -1 };
#define MODELE_I6_LEN 29
const Vector3 MODELE_I6[] ={
    {-19, -22 ,0},
    {-70, 0 ,0},
    {-19, 22 ,0},
    {-70, 0 ,0},
    {-19, -22 ,0},
    {-30, -12 ,0},
    {-30, 12 ,0},
    {-12, 30 ,0},
    {12, 30 ,0},
    {19, 22 ,0},
    {70, 0 ,0},
    {19, -22 ,0},
    {70, 0 ,0},
    {19, 22 ,0},
    {30, 12 ,0},
    {30, -12 ,0},
    {12, -30 ,0},
    {-12, -30 ,0},
    {-19, -22 ,0},
    {-16, 10 ,0},
    {-10, -10 ,0},
    {-4, 10 ,0},
    {-10, 10 ,0},
    {-10, -10 ,0},
    {4, 10 ,0},
    {10, -10 ,0},
    {16, 10 ,0},
    {10, 10 ,0},
    {10, -10 , 0}
};
const i32 MODELE_I6_SAUTS[] = { 19, 22, 24, 27, -1 };
#define MODELE_I7_LEN 25
const Vector3 MODELE_I7[] ={
    {-19, 22 ,0},
    {-70, 0 ,0},
    {-19, -22 ,0},
    {-70, 0 ,0},
    {-19, 22 ,0},
    {-30, 12 ,0},
    {-30, -12 ,0},
    {-12, -30 ,0},
    {12, -30 ,0},
    {19, -22 ,0},
    {70, 0 ,0},
    {19, 22 ,0},
    {70, 0 ,0},
    {19, -22 ,0},
    {30, -12 ,0},
    {30, 12 ,0},
    {12, 30 ,0},
    {-12, 30 ,0},
    {-19, 22 ,0},
    {-6, 20 ,0},
    {6, 12 ,0},
    {-6, 4 ,0},
    {6, -4 ,0},
    {-6, -12 ,0},
    {6, -20 , 0}
};
const i32 MODELE_I7_SAUTS[] = { 19, -1 };

#define DATAITEM_CAPACITE 7
ItemData DataItem[DATAITEM_CAPACITE] = {
	{.ID = TYPEITEM_HP, .couleure = {.r = 255,.g = 0,.b = 0,.a = 255}, .modele = MODELE_I1, .modele_longueure = MODELE_I1_LEN, .sauts_modele = MODELE_I1_SAUTS },
	{.ID = TYPEITEM_VAGUE, .couleure = {.r = 0,.g = 255,.b = 255,.a = 255}, .modele = NULL, .modele_longueure = 0, .sauts_modele = NULL },
	{.ID = TYPEITEM_X2_SHOT, .couleure = {.r = 255,.g = 128,.b = 0,.a = 255}, .modele = MODELE_I3, .modele_longueure = MODELE_I3_LEN, .sauts_modele = MODELE_I3_SAUTS },
	{.ID = TYPEITEM_X3_SHOT, .couleure = {.r = 255,.g = 255,.b = 0,.a = 255}, .modele = MODELE_I4, .modele_longueure = MODELE_I4_LEN, .sauts_modele = MODELE_I4_SAUTS },
	{.ID = TYPEITEM_HOMING, .couleure = {.r = 64,.g = 255,.b = 64,.a = 255}, .modele = MODELE_I5, .modele_longueure = MODELE_I5_LEN, .sauts_modele = MODELE_I5_SAUTS },
	{.ID = TYPEITEM_SPREAD, .couleure = {.r = 0,.g = 0,.b = 255,.a = 255}, .modele = MODELE_I6, .modele_longueure = MODELE_I6_LEN, .sauts_modele = MODELE_I6_SAUTS },
	{.ID = TYPEITEM_LASER, .couleure = {.r = 128,.g = 0,.b = 255,.a = 255}, .modele = MODELE_I7, .modele_longueure = MODELE_I7_LEN, .sauts_modele = MODELE_I7_SAUTS },
};

Item* CreerItem(Ennemi* parent) {

	if (parent == NULL) {

		return NULL;
	}

	Jeu* jeu = parent->self.jeu;

	Item* item = NULL;
	for (int i = 0; i < NB_ITEMS; i++) {

		if (!jeu->items[i].self.afficher) {

			item = &jeu->items[i];
			break;
		}
	}

	if (item == NULL) {

		return NULL;
	}

	float nb_hasard = RNG(0, 100);

	u8 facteur = 30;
	if (jeu->gamemode == GAMEMODE_ARCADE) {

		facteur = 40;
	}

	nb_hasard -= facteur * SDL_powf(0.8f, jeu->niveau);

#ifdef DEBUG_ITEMS_GRATUIT
	nb_hasard = 101;
#endif // DEBUG_ITEMS_GRATUIT

	if (nb_hasard < 80) {

		item->type = TYPEITEM_NONE;
		return item;
	}

	if (nb_hasard < 85)
		item->type = TYPEITEM_VAGUE;
	else if (nb_hasard < 90)
		item->type = TYPEITEM_HP;
	else if (nb_hasard < 92)
		item->type = TYPEITEM_HOMING;
	else if (nb_hasard < 94)
		item->type = TYPEITEM_X2_SHOT;
	else if (nb_hasard < 96)
		item->type = TYPEITEM_LASER;
	else if (nb_hasard < 98)
		item->type = TYPEITEM_SPREAD;
	else
		item->type = TYPEITEM_X3_SHOT;

#ifdef DEBUG_ITEMS_GRATUIT
	item->type = (TypeItem)RNG(1, NB_TYPES_ITEM + 1);
#endif // DEBUG_ITEMS_GRATUIT

	SDL_bool data_trouve = SDL_FALSE;
	int i;
	for (i = 0; i < DATAITEM_CAPACITE; i++) {

		if (DataItem[i].ID == item->type) {

			data_trouve = SDL_TRUE;
			break;
		}
	}

	if (!data_trouve) {

		item->type = TYPEITEM_NONE;
		return item;
	}

	InitSprite(&item->self, jeu);
    item->self.position = parent->self.position;
	item->self.afficher = SDL_TRUE;
	item->self.couleure = DataItem[i].couleure;
	item->self.modele = DataItem[i].modele;
	item->self.modele_longueur = DataItem[i].modele_longueure;
	item->self.indexs_lignes_sauter = DataItem[i].sauts_modele;

	return item;
}

SDL_bool ItemExist(Item* item) {

	if (BougerItem(item)) {

		return SDL_TRUE;
	}

	if (VerifierCollisionJoueur(item)) {

		return SDL_TRUE;
	}

	return SDL_FALSE;
}

int BougerItem(Item* item) {

	item->self.position.z *= FACTEUR_VITESSE_ITEM;

	if (item->self.position.z > POS_Z_COLLISION_JOUEUR) {

		return 0;
	}

	if (item->self.timer > TIMER_DISPARITION_ITEM) {

		item->self.afficher = SDL_FALSE;
		return 1;
	}

	item->self.timer++;

	return 0;
}

int VerifierCollisionJoueur(Item* item) {

	if (item->self.position.z > POS_Z_COLLISION_JOUEUR) {

		return 0;
	}

	Joueur* joueur = item->self.jeu->joueur;

	if (DistanceV2((Vector2) { .x = joueur->self.position.x, .y = joueur->self.position.y }, (Vector2) { .x = item->self.position.x, .y = item->self.position.y }) > JOUEUR_LARGEUR) {

		return 0;
	}

	switch (item->type) {

		case TYPEITEM_NONE:
			break;
		case TYPEITEM_HP:
			joueur->HP += HP_BONUS;
			break;
		case TYPEITEM_VAGUE:
			joueur->vagues_electriques += 1.0f;
			break;
		case TYPEITEM_HOMING:
		case TYPEITEM_X2_SHOT:
			joueur->vitesse_tir = JOUEUR_VITESSE_TIR / 2;
			joueur->powerup = item->type;
			break;
		case TYPEITEM_X3_SHOT:
			joueur->vitesse_tir = JOUEUR_VITESSE_TIR / 3;
			joueur->powerup = item->type;
			break;
		case TYPEITEM_LASER:
			joueur->vitesse_tir = JOUEUR_VITESSE_TIR * 2;
			joueur->powerup = item->type;
			break;
		default:
			joueur->vitesse_tir = JOUEUR_VITESSE_TIR;
			joueur->powerup = item->type;
			break;
	}

	item->self.afficher = SDL_FALSE;
	JouerEffet(item->self.jeu, EFFET_POWERUP);

	return 1;
}

void RenderItem(Item* item) {

	if (item->type == TYPEITEM_VAGUE) {

		SDL_SetRenderDrawColor(item->self.jeu->render, item->self.couleure.r, item->self.couleure.g, item->self.couleure.b, item->self.couleure.a);

		float pronfondeur = SDL_powf(0.95f, item->self.position.z);

		for (int i = 0; i < 3; i++) {

			DessinerCercle(item->self.jeu->render, (Vector2) { .x = item->self.position.x, .y = item->self.position.y }, (i32)((30 - i * 4) * pronfondeur), 50);
		}

		return;
	}

	RenderSprite(&item->self);
}