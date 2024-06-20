#include "Sprite.h"

int InitSprite(Sprite* sprite, Jeu* jeu) {

	if (sprite == NULL || jeu == NULL)
		return 1;

	sprite->jeu = jeu;
	sprite->taille = 1;
	sprite->afficher = SDL_FALSE;
	sprite->couleure = (SDL_Color){ .r = 255,.g = 255,.b = 255,.a = 255 };
	sprite->indexs_de_tir[0] = -1;
	sprite->indexs_de_tir[1] = -1;
	sprite->indexs_lignes_sauter = &SAUTS_MODELE_AUCUN;
	sprite->modele = NULL;
	sprite->modele_longueur = 0;
	sprite->pitch = 0;
	sprite->roll = 0;
	sprite->position = (Vector3){ 0.0f,0.0f,0.0f };
	sprite->timer = 0;

	return 0;
}

int PositionLigneModele(Sprite* sprite, i32 index_ligne, float* output) {

	if (sprite == NULL || output == NULL) {
		return 3;
	}

	if (index_ligne > sprite->modele_longueur || index_ligne < 0) {
		return 1;
	}

	if (sprite->modele == NULL) {
		return 2;
	}

	float sinroll = SDL_sinf(sprite->roll);
	float cosroll = SDL_cosf(sprite->roll);

	float grandeur_ligne = sprite->taille * (float)SDL_powf(0.95f, sprite->position.z);

	if (index_ligne == sprite->modele_longueur - 1) {

		output[0] = grandeur_ligne * (cosroll * -sprite->modele[index_ligne].x - sinroll * -sprite->modele[index_ligne].y) + sprite->position.x;
		output[1] = grandeur_ligne * (sinroll * -sprite->modele[index_ligne].x + cosroll * -sprite->modele[index_ligne].y) + sprite->position.y + sprite->modele[index_ligne].z * sprite->pitch;
		return 0;
	}

	output[0] = grandeur_ligne * (cosroll * -sprite->modele[index_ligne    ].x - sinroll * -sprite->modele[index_ligne    ].y) + sprite->position.x;
	output[1] = grandeur_ligne * (sinroll * -sprite->modele[index_ligne    ].x + cosroll * -sprite->modele[index_ligne    ].y) + sprite->position.y + sprite->modele[index_ligne    ].z * sprite->pitch;
	output[2] = grandeur_ligne * (cosroll * -sprite->modele[index_ligne + 1].x - sinroll * -sprite->modele[index_ligne + 1].y) + sprite->position.x;
	output[3] = grandeur_ligne * (sinroll * -sprite->modele[index_ligne + 1].x + cosroll * -sprite->modele[index_ligne + 1].y) + sprite->position.y + sprite->modele[index_ligne + 1].z * sprite->pitch;
	return 0;
}

int RenderSprite(Sprite* sprite) {

	if (sprite == NULL) {
		return 1;
	}

	if (!sprite->afficher || sprite->modele == NULL || sprite->modele_longueur <= 0) {
		return 0;
	}

	float positions_ligne[4];
	i32 index_sauts = 0;
	
	SDL_SetRenderDrawColor(sprite->jeu->render, sprite->couleure.r, sprite->couleure.g, sprite->couleure.b, sprite->couleure.a);

	for (int i = 0; i < sprite->modele_longueur; i++) {

		if (i == sprite->indexs_lignes_sauter[index_sauts] - 1) {

			index_sauts++;
			continue;
		}

		if (PositionLigneModele(sprite, i, positions_ligne)) {
			continue;
		}

		SDL_RenderDrawLine(sprite->jeu->render, positions_ligne[0], positions_ligne[1], positions_ligne[2], positions_ligne[3]);
	}

	return 0;
}