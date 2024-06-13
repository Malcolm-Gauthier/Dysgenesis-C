#include "Projectile.h"

Projectile* CreerProjectile(Jeu* jeu, Vector3 position, Vector3 destination, ProprietaireProjectile proprietaire, u8 ID) {

	if (jeu == NULL) {
		return NULL;
	}

	Projectile* projectile = NULL;
	for (int i = 0; i < NB_PROJECTILES; i++) {

		if (!jeu->projectiles[i].self.afficher) {

			projectile = &jeu->projectiles[i];
			break;
		}
	}

	if (projectile == NULL) {
		return NULL;
	}

	projectile->self.afficher = SDL_TRUE;

	projectile->proprietaire = proprietaire;
	projectile->self.position = position;
	projectile->cible = destination;
	projectile->ID = ID;

	projectile->laser = SDL_FALSE;
	projectile->z_init = position.z;

	if (proprietaire == JOUEUR) {

		if (GamemodeAction(jeu) && !son_cree) {

			JouerEffet(EFFET_TIR);
			son_cree = SDL_TRUE;
		}

		if (jeu->joueur->powerup == TYPEITEM_LASER) {

			projectile->laser;
		}
	}

	return projectile;
}

SDL_bool ExistProjectile(Projectile* projectile) {

	if (projectile == NULL || projectile->self.afficher == SDL_FALSE) {

		return SDL_TRUE;
	}

	if (CollisionProjectileJoueur(projectile) > 0) {

		return SDL_TRUE;
	}

	if (SDL_fabs(projectile->self.position.z - projectile->cible.z) < 1.0f) {

		projectile->self.afficher = SDL_FALSE;
		return SDL_TRUE;
	}

	if (projectile->self.position.z < projectile->cible.z) {
		projectile->self.position.z++;
	}
	else {
		projectile->self.position.z--;
	}

	return SDL_FALSE;
}

void TrouverCible(Projectile* projectile) {

	if (projectile == NULL) {

		return;
	}

	if (NbEnnemis(projectile->self.jeu) == 0) {

		return;
	}

	i32 plus_petite_distance = INT32_MAX;
	float distance;

	Ennemi* ennemi_plus_proche;
	for (int i = 0; i < NB_ENNEMIS; i++) {

		if (projectile->self.jeu->ennemis[i].self.afficher == SDL_FALSE) {

			continue;
		}

		distance = DistanceV3(projectile->self.position, projectile->self.jeu->ennemis[i].self.position);

		if (distance < plus_petite_distance) {

			plus_petite_distance = distance;
			ennemi_plus_proche = &projectile->self.jeu->ennemis[i];
		}
	}

	if (plus_petite_distance == INT32_MAX) {

		return;
	}

	projectile->cible.x = ennemi_plus_proche->self.position.x;
	projectile->cible.y = ennemi_plus_proche->self.position.y;
}

int PositionsSurEcran(Projectile* projectile, float* output) {

	if (projectile == NULL || output == NULL) {

		return 1;
	}

	Vector3 pos = projectile->self.position;
	Vector3 dest = projectile->cible;

	if (projectile->proprietaire == ENNEMI) {

		Vector3 temp = pos;
		pos = dest;
		dest = temp;

		float dist_x_de_dest = pos.x - dest.x;
		float dist_y_de_dest = pos.y - dest.y;
		float fact_profondeur_1 = SDL_powf((projectile->z_init - projectile->self.position.z) / (projectile->z_init - pos.z), 3.0f);
		float fact_profondeur_2 = SDL_powf((projectile->z_init - (projectile->self.position.z + 1)) / (projectile->z_init - pos.z), 3.0f);

		output[0] = dist_x_de_dest * fact_profondeur_1 + dest.x;
		output[1] = dist_y_de_dest * fact_profondeur_1 + dest.y;
		output[2] = dist_x_de_dest * fact_profondeur_2 + dest.x;
		output[3] = dist_y_de_dest * fact_profondeur_2 + dest.y;

		return 0;
	}

	float dist_x_de_dest = pos.x - dest.x;
	float dist_y_de_dest = pos.y - dest.y;
	float fact_profondeur_1 = SDL_powf(VITESSE_PROJECTILE, projectile->self.position.z);
	float fact_profondeur_2 = SDL_powf(VITESSE_PROJECTILE, projectile->self.position.z + 1);

	output[0] = dist_x_de_dest * fact_profondeur_1 + dest.x;
	output[1] = dist_y_de_dest * fact_profondeur_1 + dest.y;
	output[2] = dist_x_de_dest * fact_profondeur_2 + dest.x;
	output[3] = dist_y_de_dest * fact_profondeur_2 + dest.y;

	return 0;
}

int CollisionProjectileJoueur(Projectile* projectile) {

	const float MARGE_DE_MANOEUVRE = 0.75f;

	if (projectile == NULL) {

		return -1;
	}

	if (projectile->proprietaire == JOUEUR) {

		return -2;
	}

	Joueur* joueur = projectile->self.jeu->joueur;

	if (JoueurMort(joueur)) {

		projectile->self.afficher = SDL_FALSE;
		return 2;
	}

	float positions_projectile[4];

	if (PositionsSurEcran(projectile, positions_projectile)) {

		return -4;
	}

	if (DistanceV2(
		(Vector2) { positions_projectile[0], positions_projectile[1] },
		(Vector2) { joueur->self.position.x, joueur->self.position.y }) > MARGE_DE_MANOEUVRE * JOUEUR_LARGEUR)
		{

		return 0;
	}

	joueur->HP--;
	CreerExplosion(joueur->self.position);
	projectile->self.afficher = SDL_FALSE;

	if (!JoueurMort(joueur)) {

		return 1;
	}

	Mix_HaltMusic();
	joueur->self.timer = 0;

	if (projectile->self.jeu->curseur.max_selection < 2)
		projectile->self.jeu->curseur.max_selection = 2;

	if (projectile->self.jeu->gamemode == GAMEMODE_AVENTURE)
		projectile->self.jeu->niveau_continue = projectile->self.jeu->niveau;

	return 3;
}

void RenderProjectile(Projectile* projectile) {

	if (projectile == NULL) {

		return;
	}

	Joueur* joueur = projectile->self.jeu->joueur;

	if (JoueurMort(joueur)) {

		return;
	}

	SDL_Renderer* render = projectile->self.jeu->render;

	if (projectile->proprietaire == ENNEMI) {

		SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	}
	else {

		switch (joueur->powerup)
		{
			case TYPEITEM_X2_SHOT:
				SDL_SetRenderDrawColor(render, 255, 127, 0, 255);
				break;
			case TYPEITEM_X3_SHOT:
				SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
				break;
			case TYPEITEM_HOMING:
				SDL_SetRenderDrawColor(render, 64, 255, 64, 255);
				break;
			case TYPEITEM_SPREAD:
				SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
				break;
			case TYPEITEM_LASER:
				SDL_SetRenderDrawColor(render, 127, 0, 255, 255);
				break;
			default:
				SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
				break;
		}
	}

	float positions[4];

	if (projectile->laser) {

		if (PositionLigneModele(&joueur->self, joueur->self.indexs_de_tir[projectile->ID % 2], positions)) {
			return;
		}

		projectile->self.position.x = positions[0];
		projectile->self.position.y = positions[1];

		float pos_z = projectile->self.position.z;
		int largeur_laser;

		for (int i = 0; i < MAX_PROFONDEUR; i++) {

			const int LARGEUR_MAX_LASER = 5;

			largeur_laser = (int)(((MAX_PROFONDEUR - i) / (float)MAX_PROFONDEUR) * LARGEUR_MAX_LASER);
			projectile->self.position.z = i;

			PositionsSurEcran(projectile, positions);

			SDL_RenderDrawLine(projectile->self.jeu->render,
				positions[0] + RNG(-largeur_laser, largeur_laser),
				positions[1] + RNG(-largeur_laser, largeur_laser),
				positions[2] + RNG(-largeur_laser, largeur_laser),
				positions[3] + RNG(-largeur_laser, largeur_laser)
			);
		}

		projectile->self.position.z = pos_z;

		return;
	}

	PositionsSurEcran(projectile, positions);
	SDL_RenderDrawLine(projectile->self.jeu->render,
		positions[0],
		positions[1],
		positions[2],
		positions[3]
	);
}



int CreerVagueElectrique(VagueElectrique* vague) {
	
	if (vague == NULL) {

		return -1;
	}

	if (vague->self.afficher || JoueurMort(vague->self.jeu->joueur) || vague->self.jeu->joueur->vagues_electriques < 1.0f) {

		return 1;
	}

	vague->self.jeu->joueur->vagues_electriques -= 1.0f;
	vague->self.timer = 0;
	vague->rayon = 0;
	vague->self.afficher = SDL_TRUE;
	JouerEffet(EFFET_VAGUE);

	return 0;
}

void ExistVagueElectrique(VagueElectrique* vague) {

	if (!vague->self.afficher) {

		return;
	}

	vague->rayon = (1 - SDL_powf(0.9f, vague->self.timer)) * LARGEUR_MAX_VAGUE_ELECTRIQUE + LARGEUR_MIN_VAGUE_ELECTRIQUE;

	vague->self.timer++;
	if (vague->self.timer > DURATION_VAGUE_ELECTRIQUE) {

		vague->self.afficher = SDL_FALSE;
		return;
	}

	if (vague->self.timer < TEMPS_AVANT_VAGUE_FAIT_DEGATS || vague->self.timer % 3 != 0) {

		return;
	}

	Ennemi* ennemis = vague->self.jeu->ennemis;

	for (int i = 0; i < NB_ENNEMIS; i++) {

		if (ennemis[i].self.afficher == SDL_FALSE) {

			continue;
		}

		if (ennemis[i].self.position.z != 0) {

			continue;
		}

		if (DistanceV3(ennemis[i].self.position, vague->self.jeu->joueur->self.position) <= vague->rayon) {

			ennemis[i].HP--;
		}

		if (ennemis[i].HP <= 0) {

			ennemis[i].self.afficher = SDL_FALSE;
			vague->self.jeu->ennemis_tues++;
		}
	}
}

void RenderVagueElectrique(VagueElectrique* vague) {

	if (!vague->self.afficher) {

		return;
	}

	SDL_SetRenderDrawColor(vague->self.jeu->render, 0, 255, 255, 255);
	float angle = M_PI / (PRECISION_VAGUE_ELECTRIQUE / 2.0f);

	Vector3 pos_joueur = vague->self.jeu->joueur->self.position;

	const int NB_DE_CERCLES = 3;
	for (int i = 0; i < NB_DE_CERCLES; i++) {

		float rand;
		float angle_pos;
		Vector2 nouvelle_pos;
		Vector2 prev_pos = (Vector2){
			pos_joueur.x,
			pos_joueur.y + vague->rayon
		};

		for (float j = 0; j < PRECISION_VAGUE_ELECTRIQUE; j++) {

			rand = RNG(-20, 20) + vague->rayon;
			angle_pos = (j + 1) * angle;

			nouvelle_pos = (Vector2){
				pos_joueur.x + rand * SDL_sinf(angle_pos),
				pos_joueur.y + rand * SDL_cosf(angle_pos)
			};

			if (j == PRECISION_VAGUE_ELECTRIQUE - 1) {

				nouvelle_pos = (Vector2){ pos_joueur.x, pos_joueur.y + vague->rayon };
			}

			SDL_RenderDrawLine(vague->self.jeu->render,
				prev_pos.x,
				prev_pos.y,
				nouvelle_pos.x,
				nouvelle_pos.y
			);

			prev_pos = nouvelle_pos;
		}
	}
}