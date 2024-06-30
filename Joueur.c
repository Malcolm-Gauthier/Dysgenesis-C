#include "Joueur.h"
#include "Misc.h"

const Vector3 MODELE_P[] = {
	{-50,0,0}, // rouge
	{-46,-2,-50}, // orange
	{-46,0,-2}, // vomi
	{-46,-4,-2}, // -vomi
	{-46,0,-2}, // vomi
	{-10,5,-5}, // vert pâle
	{-10,-5,-5}, // -vert pâle
	{-10,5,-5}, // vert pâle
	{-5,5,-15}, // rose
	{-5,0,-15}, // -rose
	{-5,5,-15}, // rose
	{5,5,-15}, // rose
	{5,0,-15}, // -rose
	{5,5,-15}, // rose
	{10,5,-5}, // vert pâle
	{46,0,-2}, // vomi
	{46,-2,-50}, // orange
	{50,0,0}, // rouge
	{50,-5,0}, // rouge 2
	{13,-10,10}, // bleu
	{15,-9,20}, // mauve
	{4,10,10}, // vert foncé
	{13,-10,10}, // bleu
	{15,-9,20}, // mauve
	{0,-15,20}, // bleh
	{-15,-9,20}, // mauve
	{-4,10,10}, // vert foncé
	{-13,-10,10}, // bleu
	{-15,-9,20}, // mauve
	{-13,-10,10}, // bleu
	{-50,-5,0}, // rouge 2
	{-50,0,0}, // rouge
	{-50,-5,0}, // rouge 2
	{-46,-2,-50}, // orange
	{-46,-4,-2}, // -vomi
	{-10,-5,-5}, // -vert pâle
	{-5,0,-15}, // -rose
	{5,0,-15}, // -rose
	{10,-5,-5}, // -vert pâle
	{46,-4,-2}, // -vomi
	{46,-2,-50}, // orange
	{50,-5,0}, // rouge 2
	{50,0,0}, // rouge
	{10,5,10}, // -bleu
	{4,10,10}, // vert foncé
	{2,10,10}, // l'autre
	{1,15,10}, // bleu pâle
	{-1,15,10}, // bleu pâle
	{1,15,10}, // bleu pâle
	{0,15,25}, // jaune
	{0,-15,20}, // bleh
	{0,15,25}, // jaune
	{-1,15,10}, // bleu pâle
	{-2,10,10}, // l'autre
	{-4,10,10}, // vert foncé
	{-10,5,10}, // -bleu
	{-50,0,0} // rouge
};
extern const i32 SAUTS_MODELE_AUCUN;

int CreerJoueur(Jeu* jeu, Joueur* joueur) {

	joueur->self.jeu = jeu;

	joueur->self.indexs_de_tir[0] = 1;
	joueur->self.indexs_de_tir[1] = 16;

	joueur->self.modele = MODELE_P;
	joueur->self.modele_longueur = MODELE_JOUEUR_LONGUEUR;

	joueur->self.indexs_lignes_sauter = &SAUTS_MODELE_AUCUN;

	InitializerJoueur(joueur);

	return 0;
}

void InitializerJoueur(Joueur* joueur) {

	joueur->HP = JOUEUR_DEFAULT_HP;
	joueur->powerup = TYPEITEM_NONE;
	joueur->velocite = (Vector2){ .x = 0, .y = -30 };
	joueur->vitesse_tir = JOUEUR_VITESSE_TIR;
	joueur->self.position = (Vector3){ .x = W_SEMI_LARGEUR, .y = W_HAUTEUR, 0 };
	joueur->self.afficher = SDL_TRUE;
	joueur->self.pitch = 0;
	joueur->self.roll = 0;
	joueur->self.couleure = (SDL_Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
	joueur->self.taille = 1.0f;
}

SDL_bool ExistJoueur(Joueur* joueur) {

	if (JoueurMort(joueur)) {

		AnimationMort(joueur);
		return SDL_TRUE;
	}

	BougerJoueur(joueur);

	if (joueur->self.jeu->bombe->HP <= 0) {

		return SDL_FALSE;
	}

	TirJoueur(joueur);

	joueur->vagues_electriques += VAGUE_ELECTRIQUE_REGENERATION;

	return SDL_FALSE;
}

static int TirJoueur(Joueur* joueur) {

	const Vector2 DECALS_PROJ[] = {
		(Vector2) { .x=10, .y=-200 },
		(Vector2) { .x=-10, .y=-200 },
		(Vector2) { .x=30, .y=-240 },
		(Vector2) { .x=-30, .y=-240 },
		(Vector2) { .x=50, .y=-200 },
		(Vector2) { .x=-50, .y=-200 },
	};
	
	joueur->self.timer++;

	if (joueur->self.timer < joueur->vitesse_tir) {
		return 0;
	}

	if (!TouchePesee(joueur->self.jeu, TOUCHE_J)) {
		return 0;
	}

	joueur->self.timer = 0;

	i8 iterations = 2;
	if (joueur->powerup == TYPEITEM_SPREAD) {
		iterations = 6;
	}

	Projectile* projectiles[6] = { 0 };
	float ligne[4];

	for (int i = 0; i < iterations; i++) {

		if (PositionLigneModele(&joueur->self, joueur->self.indexs_de_tir[i % 2], ligne)) {
			return -1;
		}

		projectiles[i] = CreerProjectile(
			joueur->self.jeu,
			(Vector3) { ligne[0], ligne[1], 0 },
			(Vector3) {
				joueur->self.position.x + DECALS_PROJ[i].x,
				joueur->self.position.y + DECALS_PROJ[i].y,
				MAX_PROFONDEUR
			},
			PROPRIETAIREPROJ_JOUEUR,
			(u8)(i % 2)
		);
	}

	if (joueur->powerup == TYPEITEM_HOMING) {

		TrouverCible(projectiles[0]);
		TrouverCible(projectiles[1]);
	}

	return 1;
}

static void AnimationMort(Joueur* joueur) {

	const float JOUEUR_MORT_VITESSE_X = -0.6f;
	const float JOUEUR_MORT_VITESSE_Y = 0.6f;
	const float JOUEUR_MORT_VITESSE_ROLL = 1.0f / (1 * G_FPS);
	const float JOUEUR_MORT_FACTEUR_GRANDEUR = 1.0f / G_FPS;
	const i32 QUANTITE_LIGNES_MODELE_MORT = 3;

	const i32 IMAGES_AVANT_TEXTE = 2 * G_FPS;
	const i32 IMAGES_AVANT_RETOUR_MENU = 10 * G_FPS;

	static i32* lignes_a_sauter_temporaire;

	if (joueur->self.timer == 1) {

		JouerEffet(joueur->self.jeu, EFFET_EXPLOSION_JOUEUR);
		joueur->self.jeu->bouger_etoiles = SDL_TRUE;

		i32* lignes_a_sauter_temporaire = malloc(sizeof(i32) * joueur->self.modele_longueur);
		if (!lignes_a_sauter_temporaire)
			return;

		i32 longueure_liste = 0;
		for (int i = 0; i < joueur->self.modele_longueur; i++) {

			if (i % QUANTITE_LIGNES_MODELE_MORT != 0) {
				lignes_a_sauter_temporaire[longueure_liste] = i;
				longueure_liste++;
			}
		}
		lignes_a_sauter_temporaire[longueure_liste] = -1;
		joueur->self.indexs_lignes_sauter = lignes_a_sauter_temporaire;
	}

	if (joueur->self.timer < SDL_MAX_UINT8) {

		joueur->self.couleure.a = (u8)(SDL_MAX_UINT8 - joueur->self.timer);
		joueur->self.position.x += JOUEUR_MORT_VITESSE_X;
		joueur->self.position.y += JOUEUR_MORT_VITESSE_Y;
		joueur->self.roll += JOUEUR_MORT_VITESSE_ROLL;
		joueur->self.taille = 1 + joueur->self.timer * JOUEUR_MORT_FACTEUR_GRANDEUR;
	}

	if (joueur->self.timer > IMAGES_AVANT_TEXTE) {

		DisplayText(joueur->self.jeu, "game over", (Vector2) { 0, 0 }, 5, 0, joueur->self.timer - 120, 0);

		if (joueur->self.jeu->gamemode == GAMEMODE_ARCADE) {

			DisplayText(joueur->self.jeu, "0", (Vector2) { 0, 0 }, 5, 0, joueur->self.timer - 120, 0);
		}
	}

	if (joueur->self.timer > IMAGES_AVANT_RETOUR_MENU) {

		joueur->self.afficher = SDL_FALSE;
		joueur->self.timer = 0;

		joueur->self.indexs_lignes_sauter = NULL;
		free(lignes_a_sauter_temporaire);

		JouerMusique(joueur->self.jeu, MUSIQUE_DYSGENESIS, SDL_TRUE);
		joueur->self.jeu->bouger_etoiles = SDL_TRUE;
		//vider ennemis, explosions & projectiles
		joueur->self.jeu->gamemode = GAMEMODE_MENU_PRINCIPAL;
		return;
	}

	joueur->self.timer++;
}

static void BougerJoueur(Joueur* joueur) {

	const float JOUEUR_FRICTION = 0.9f;
	const float JOUEUR_PITCH_ACCELERATION = 0.05f;
	const float JOUEUR_ROLL_ACCELERATION = 0.05f;
	const float JOUEUR_PITCH_FRICTION = 0.95f;
	const float JOUEUR_ROLL_FRICTION = 0.95f;
	const float JOUEUR_PITCH_CONSTANT = 0.3f;
	const int JOUEUR_VITESSE = 1;

	if (TouchePesee(joueur->self.jeu, TOUCHE_W)) {

		joueur->velocite.y -= JOUEUR_VITESSE;
		joueur->self.pitch += JOUEUR_PITCH_ACCELERATION;
	}
	if (TouchePesee(joueur->self.jeu, TOUCHE_A)) {

		joueur->velocite.x -= JOUEUR_VITESSE;
		joueur->self.roll -= JOUEUR_ROLL_ACCELERATION;
	}
	if (TouchePesee(joueur->self.jeu, TOUCHE_S)) {

		joueur->velocite.y += JOUEUR_VITESSE;
		joueur->self.pitch -= JOUEUR_PITCH_ACCELERATION;
	}
	if (TouchePesee(joueur->self.jeu, TOUCHE_D)) {

		joueur->velocite.x += JOUEUR_VITESSE;
		joueur->self.roll += JOUEUR_ROLL_ACCELERATION;
	}

	joueur->velocite.x *= JOUEUR_FRICTION;
	joueur->velocite.y *= JOUEUR_FRICTION;
	joueur->self.pitch = (joueur->self.pitch - JOUEUR_PITCH_CONSTANT) * JOUEUR_PITCH_FRICTION + JOUEUR_PITCH_CONSTANT;
	joueur->self.roll *= JOUEUR_ROLL_FRICTION;

	joueur->self.position.x += joueur->velocite.x;
	joueur->self.position.y += joueur->velocite.y;

	if (joueur->self.position.x - JOUEUR_LARGEUR < 0)
		joueur->self.position.x = JOUEUR_LARGEUR;

	if (joueur->self.position.x + JOUEUR_LARGEUR > W_LARGEUR)
		joueur->self.position.x = W_LARGEUR - JOUEUR_LARGEUR;

	if (joueur->self.position.y - JOUEUR_HAUTEUR < 0)
		joueur->self.position.y = JOUEUR_HAUTEUR;

	if (joueur->self.position.y + JOUEUR_HAUTEUR > W_HAUTEUR)
		joueur->self.position.y = W_HAUTEUR - JOUEUR_HAUTEUR;
}

SDL_bool JoueurMort(Joueur* joueur) {

	return (SDL_bool)(joueur->HP <= 0);
}