#include "Dysgenesis.h"

int main() {

	Jeu* jeu = SDL_malloc(sizeof(Jeu));

	int i = 0;
	if (i = Init(jeu)) {

		FreeMem(jeu);
		SDL_Quit();
		return i;
	}

	while (jeu->en_cours) {

		Controlles(jeu);
		Code(jeu);
		Render(jeu);
		SDLRender(jeu);

#ifndef DEBUG_FPS_INFINI
		while (jeu->temps_image > clock() - CLOCKS_PER_SEC / G_FPS);
#endif

		jeu->temps_image = clock();
		jeu->gTimer++;
	}

	FreeMem(jeu);
	SDL_Quit();

	return 0;
}

int Init(Jeu* jeu) {

	if (jeu == NULL) {

		return 1;
	}

	jeu->joueur = SDL_malloc(sizeof(Joueur));
	jeu->ennemis = SDL_malloc(sizeof(Ennemi) * NB_ENNEMIS);
	jeu->projectiles = SDL_malloc(sizeof(Projectile) * NB_PROJECTILES);
	jeu->explosions = SDL_malloc(sizeof(Explosion) * NB_EXPLOSIONS);
	jeu->items = SDL_malloc(sizeof(Item) * NB_ITEMS);

	jeu->vague_electrique = SDL_malloc(sizeof(VagueElectrique));
	jeu->bombe = SDL_malloc(sizeof(BombePulsar));
	jeu->son = SDL_malloc(sizeof(Son));

	if (!jeu->joueur || !jeu->explosions || !jeu->ennemis || !jeu->projectiles || !jeu->items || !jeu->vague_electrique || !jeu->bombe || !jeu->son) {

		return 2;
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {

		return 3;
	}

	if (!(jeu->fenetre = SDL_CreateWindow("Dysgenesis", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_LARGEUR, W_HAUTEUR,
#ifdef DEBUG_PLEIN_ECRAN
		SDL_WINDOW_FULLSCREEN |
#endif
		SDL_WINDOW_SHOWN
	))) {

		return 4;
	}

	if (!(jeu->render = SDL_CreateRenderer(jeu->fenetre, -1, SDL_RENDERER_ACCELERATED))) {

		return 5;
	}

	if (SDL_SetRenderDrawBlendMode(jeu->render, SDL_BLENDMODE_BLEND)) {

		return 6;
	}

	SDL_ShowCursor(SDL_FALSE);
	SDL_SetRenderDrawColor(jeu->render, jeu->couleure_fond_ecran.r, jeu->couleure_fond_ecran.g, jeu->couleure_fond_ecran.b, jeu->couleure_fond_ecran.a);
	SDL_RenderPresent(jeu->render);
	jeu->temps_image = clock();

	if (InitSDLMixer(jeu->son)) {

		return 7;
	}

	if (CreerEtoiles(jeu->etoiles, DENSITE_ETOILES, (SDL_Rect) { .x = 0, .y = 0, .w = W_LARGEUR, .h = W_HAUTEUR })) {

		return 8;
	}

	CreerJoueur(jeu->joueur);
	for (int i = 0; i < NB_ENNEMIS; i++) jeu->ennemis[i].self.afficher = SDL_FALSE;
	for (int i = 0; i < NB_EXPLOSIONS; i++) jeu->explosions[i].timer = 0;
	for (int i = 0; i < NB_PROJECTILES; i++) jeu->projectiles[i].self.afficher = SDL_FALSE;
	for (int i = 0; i < NB_ITEMS; i++) jeu->items[i].self.afficher = SDL_FALSE;
	CreerCurseur(jeu);
	jeu->vague_electrique->self.afficher = SDL_FALSE;
	jeu->bombe->jeu = jeu;

	return 0;
}




SDL_bool TouchePesee(Jeu* jeu, Touche touche) {

	return (SDL_bool)(jeu->touches_pesees & touche);
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

		jeu->ennemis[i].self.afficher = SDL_FALSE;
	}
}

void ClearProjectiles(Jeu* jeu) {

	for (int i = 0; i < NB_PROJECTILES; i++) {

		jeu->projectiles[i].self.afficher = SDL_FALSE;
	}
}

void ClearExplosions(Jeu* jeu) {

	for (int i = 0; i < NB_EXPLOSIONS; i++) {

		jeu->explosions[i].timer = 0;
	}
}

void ClearItems(Jeu* jeu) {

	for (int i = 0; i < NB_ITEMS; i++) {

		jeu->items[i].self.afficher = SDL_FALSE;
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

void DessinerCercle(SDL_Renderer* render, Vector2 position, i32 rayon, i32 precision) {

	float ang;
	float next_ang;

	for (int i = 0; i < precision; i++)
	{
		ang = (i * M_PI * 2) / precision;
		next_ang = ((i + 1) * M_PI * 2) / precision;

		SDL_RenderDrawLine(render,
			position.x + rayon * SDL_sinf(ang),
			position.y + rayon * SDL_cosf(ang),
			position.x + rayon * SDL_sinf(next_ang),
			position.y + rayon * SDL_cosf(next_ang)
		);
	}
}