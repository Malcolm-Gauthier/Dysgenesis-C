#include "Dysgenesis.h"

int main() {
	SDL_SetError("Hello world!");
	int i = 0;
	return i;
}

SDL_bool GamemodeAction(Jeu* jeu) {

	return (SDL_bool)(jeu->gamemode == GAMEMODE_AVENTURE || jeu->gamemode == GAMEMODE_ARCADE);
}

i32 NbEnnemis(Jeu* jeu) {

	i32 resultat = 0;

	for (int i = 0; i < NB_ENNEMIS; i++) {

		if (jeu->ennemis[i].self.afficher)
			resultat++;
	}

	return resultat;
}

void ClearEnnemis(Jeu* jeu) {

	for (int i = 0; i < NB_ENNEMIS; i++) {

		jeu->ennemis->self.afficher = SDL_FALSE;
	}
}

void ClearProjectiles(Jeu* jeu) {

	for (int i = 0; i < NB_PROJECTILES; i++) {

		jeu->projectiles->self.afficher = SDL_FALSE;
	}
}

void ClearExplosions(Jeu* jeu) {

	for (int i = 0; i < NB_EXPLOSIONS; i++) {

		jeu->explosions->self.afficher = SDL_FALSE;
	}
}

void ClearItems(Jeu* jeu) {

	for (int i = 0; i < NB_ITEMS; i++) {

		jeu->items->self.afficher = SDL_FALSE;
	}
}

float DistanceV3(Vector3 a, Vector3 b) {

	float dist_x = a.x - b.x;
	float dist_y = a.y - b.y;
	float dist_z = a.z - b.z;

	return SDL_sqrtf(
		dist_x * dist_x + 
		dist_y * dist_y + 
		dist_z + dist_z
	);
}

float DistanceV2(Vector2 a, Vector2 b) {

	float dist_x = a.x - b.x;
	float dist_y = a.y - b.y;

	return SDL_sqrtf(
		dist_x * dist_x +
		dist_y * dist_y
	);
}

i32 RNG(i32 min, i32 max) {

	return rand() % SDL_abs(max - min) + min;
}

SDL_Color RGBAtoSDLColor(u32 RGBA) {

	return (SDL_Color) {
		.r = (u8)(RGBA >> 24),
		.g = (u8)(RGBA >> 16),
		.b = (u8)(RGBA >> 8),
		.a = (u8)RGBA
	};
}