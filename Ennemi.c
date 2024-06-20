#include "Ennemi.h"

typedef struct EnnemiData {
	TypeEnnemi ID;

	float vitesse;
	float vitesse_z;
	float vitesse_tir;

	i32 hp_max;
	i32 largeur;
	SDL_Color couleure;
	i32 indexs_tir[2];

	Vector3* modele;
	i32 modele_longueur;
	i32* modele_sauts;
} EnnemiData;

#define DATAENNEMI_CAPACITE 17
EnnemiData DataEnnemi[DATAENNEMI_CAPACITE] = {
	0
};

Ennemi* CreerEnnemi(Jeu* jeu, TypeEnnemi type, StatusEnnemi statut, Ennemi* parent) {

	Ennemi* ennemi = NULL;
	for (int i = 0; i < NB_ENNEMIS; i++) {

		if (!jeu->ennemis[i].self.afficher) {

			ennemi = &jeu->ennemis[i];
			break;
		}
	}

	if (ennemi == NULL) {

		return NULL;
	}

	SDL_bool data_trouve = SDL_FALSE;
	int i;
	for (i = 0; i < DATAENNEMI_CAPACITE; i++) {

		if (DataEnnemi[i].ID == type) {

			data_trouve = SDL_TRUE;
			break;
		}
	}

	if (!data_trouve) {

		return NULL;
	}

	ennemi->type = type;
	ennemi->statut = statut;
	ennemi->velocite = (Vector2){ 0, 0 };
	ennemi->cible = (Vector2){ jeu->joueur->self.position.x, jeu->joueur->self.position.y };

	ennemi->vitesse = DataEnnemi[i].vitesse;
	ennemi->vitesse_z = DataEnnemi[i].vitesse_z;
	ennemi->intervale_tir = G_FPS / DataEnnemi[i].vitesse_tir;
	ennemi->HP = DataEnnemi[i].hp_max;
	ennemi->largeur = DataEnnemi[i].largeur;

	InitSprite(&ennemi->self, jeu);
	ennemi->self.couleure = DataEnnemi[i].couleure;
	ennemi->self.indexs_de_tir[0] = DataEnnemi[i].indexs_tir[0];
	ennemi->self.indexs_de_tir[1] = DataEnnemi[i].indexs_tir[1];
	ennemi->self.modele = DataEnnemi[i].modele;
	ennemi->self.modele_longueur = DataEnnemi[i].modele_longueur;
	ennemi->self.indexs_lignes_sauter = DataEnnemi[i].modele_sauts;

	ennemi->modele_en_marche = NULL;
	if (ennemi->self.modele != NULL) {

		ennemi->modele_en_marche = malloc(sizeof(Vector3) * ennemi->self.modele_longueur);
		if (ennemi->modele_en_marche == NULL) return NULL;
		SDL_memcpy(ennemi->modele_en_marche, ennemi->self.modele, sizeof(Vector3) * ennemi->self.modele_longueur);
	}

	ennemi->self.afficher = SDL_TRUE;
	ennemi->self.taille = 1.0f;

	if (parent != NULL) {

		ennemi->self.position = jeu->joueur->self.position;
	}
	else {

		ennemi->self.position = (Vector3){
			.x = RNG(DISTANCE_DE_BORD_EVITER_INIT, W_LARGEUR - DISTANCE_DE_BORD_EVITER_INIT),
			.y = RNG(DISTANCE_DE_BORD_EVITER_INIT, W_HAUTEUR - DISTANCE_DE_BORD_EVITER_INIT),
			.z = MAX_PROFONDEUR
		};
	}

	if (ennemi->type == TYPEENNEMI_BOSS) {

		ennemi->statut = STATUSENNEMI_BOSS_INIT;
		ennemi->self.position.z = 99;
	}
	else if (ennemi->type == TYPEENNEMI_PATRA || ennemi->type == TYPEENNEMI_PATRA_DUR) {

		ennemi->statut = STATUSENNEMI_PATRA_8_RESTANT;
	}
	else if (ennemi->type == TYPEENNEMI_DUPLIQUEUR || ennemi->type == TYPEENNEMI_DUPLIQUEUR_DUR) {

		switch (ennemi->statut) {

		case STATUSENNEMI_DUPLIQUEUR_0_RESTANT:
			ennemi->HP /= 4;
			ennemi->self.taille *= 0.5f;
			break;

		case STATUSENNEMI_DUPLIQUEUR_1_RESTANT:
			ennemi->HP /= 2;
			ennemi->self.taille *= 0.75f;
			break;

		default:
			ennemi->statut = STATUSENNEMI_DUPLIQUEUR_2_RESTANT;
			break;
		}
	}
	else if (ennemi->statut == STATUSENNEMI_INITIALIZATION) {

		ennemi->statut = STATUSENNEMI_NORMAL;
	}

	return ennemi;
}

SDL_bool ExistEnnemi(Ennemi* ennemi) {

	ennemi->self.timer++;

	if (JoueurMort(ennemi->self.jeu->joueur)) {

		ActualiserModeleEnnemi(ennemi);
		return SDL_FALSE;
	}

	if (CodeBoss(ennemi)) {

		return SDL_FALSE;
	}

	BougerEnnemi(ennemi);

	ActualiserModeleEnnemi(ennemi);

	ennemi->cible = TrouverCibleEnnemi(ennemi);

	for (int i = 0; i < NB_PROJECTILES; i++) {

		if (CollisionProjectileEnnemi(ennemi, &ennemi->self.jeu->projectiles[i])) {

			return SDL_TRUE;
		}
	}

	if (CollisionJoueurEnnemi(ennemi, ennemi->self.jeu->joueur)) {

		return SDL_TRUE;
	}

	TirEnnemi(ennemi);

	return SDL_FALSE;
}

void ActualiserModeleEnnemi(Ennemi* ennemi) {

	if (ennemi->modele_en_marche == NULL) {

		return;
	}

	float vitesse = 0;
	float angle;

	switch (ennemi->type) {

	case TYPEENNEMI_OCTAHEDRON_DUR:
		vitesse = 50;
	case TYPEENNEMI_OCTAHEDRON:
		if (vitesse == 0)
			vitesse = 20;

		angle = ennemi->self.timer / vitesse;

		for (int i = 0; i < ennemi->self.modele_longueur; i++)
		{
			if (i == 0 || i == 3 || i == 9)
			{
				ennemi->modele_en_marche[i].x = SDL_cosf(angle) * 20;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle) * 20;
			}
			else if (i == 1 || i == 5 || i == 10)
			{
				ennemi->modele_en_marche[i].x = SDL_cosf(angle + 2.0f) * 20;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 2.0f) * 20;
			}
			else if (i == 2 || i == 7)
			{
				ennemi->modele_en_marche[i].x = SDL_cosf(angle + 4.0f) * 20;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 4.0f) * 20;
			}
		}
		break;

	case TYPEENNEMI_TOURNANT_DUR:
		vitesse = 50;
	case TYPEENNEMI_TOURNANT:
		if (vitesse == 0)
			vitesse = 20;

		angle = ennemi->self.timer / vitesse;

		for (int i = 0; i < ennemi->self.modele_longueur; i++)
		{
			switch (i)
			{
			case 16:
			case 19:
				ennemi->modele_en_marche[i].y = SDL_cosf(angle) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle) * 10;
				break;
			case 13:
			case 17:
				ennemi->modele_en_marche[i].y = SDL_cosf(angle + 1.5f) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 1.5f) * 10;
				break;
			case 6:
			case 12:
				ennemi->modele_en_marche[i].y = SDL_cosf(angle + 3) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 3) * 10;
				break;
			case 7:
			case 20:
				ennemi->modele_en_marche[i].y = SDL_cosf(angle + 4.5f) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 4.5f) * 10;
				break;
			case 15:
			case 23:
				ennemi->modele_en_marche[i].y = SDL_cosf(-angle) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(-angle) * 10;
				break;
			case 4:
			case 14:
				ennemi->modele_en_marche[i].y = SDL_cosf(-angle + 1.5f) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(-angle + 1.5f) * 10;
				break;
			case 5:
			case 25:
				ennemi->modele_en_marche[i].y = SDL_cosf(-angle + 3) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(-angle + 3) * 10;
				break;
			case 21:
			case 24:
				ennemi->modele_en_marche[i].y = SDL_cosf(-angle + 4.5f) * 10;
				ennemi->modele_en_marche[i].z = SDL_sinf(-angle + 4.5f) * 10;
				break;
			}
		}
		break;

	case TYPEENNEMI_CROISSANT_DUR:
		vitesse = 50;
	case TYPEENNEMI_CROISSANT:
		if (vitesse == 0)
			vitesse = 20;

		angle = ennemi->self.timer / vitesse;

		for (int i = 0; i < ennemi->self.modele_longueur; i++)
		{
			if (i == 49 || i == 56)
			{
				ennemi->modele_en_marche[i].x = SDL_cosf(angle) * 20;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle) * 20;
			}
			if (i == 53 || i == 50)
			{
				ennemi->modele_en_marche[i].x = SDL_cosf(angle + 1.5f) * 20;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 1.5f) * 20;
			}
			if (i == 52 || i == 54)
			{
				ennemi->modele_en_marche[i].x = SDL_cosf(angle + 3) * 20;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 3) * 20;
			}
			if (i == 55 || i == 57)
			{
				ennemi->modele_en_marche[i].x = SDL_cosf(angle + 4.5f) * 20;
				ennemi->modele_en_marche[i].z = SDL_sinf(angle + 4.5f) * 20;
			}
		}
		break;

	case TYPEENNEMI_PATRA:
	case TYPEENNEMI_PATRA_DUR:

		if (ennemi->statut == STATUSENNEMI_PATRA_0_RESTANT) {

			break;
		}

		SDL_SetRenderDrawColor(ennemi->self.jeu->render, ennemi->self.couleure.r, ennemi->self.couleure.g, ennemi->self.couleure.b, ennemi->self.couleure.a);

		angle = SDL_fmodf(ennemi->self.timer / 20.0f, M_PI * 2);
		float profondeur = SDL_powf(0.95f, ennemi->self.position.z);
		float angle_prochain = M_PI / 1.5f;

		for (int i = 0; i < ennemi->statut - STATUSENNEMI_PATRA_0_RESTANT; i++) {

			float subAng = SDL_fmodf(ennemi->self.timer / -10.0f, M_PI * 2.0f);

			float centreX2 = SDL_cosf(angle) * 80 * profondeur + ennemi->self.position.x;
			float centreY2 = SDL_sinf(angle) * 80 * profondeur + ennemi->self.position.y;

			for (int j = 0; j < 3; j++) {

				SDL_RenderDrawLine(ennemi->self.jeu->render,
					centreX2 + SDL_cosf(subAng) * profondeur * 20,
					centreY2 + SDL_sinf(subAng) * profondeur * 20,
					centreX2 + SDL_cosf(subAng + angle_prochain) * profondeur * 20,
					centreY2 + SDL_sinf(subAng + angle_prochain) * profondeur * 20
				);

				subAng += M_PI / 1.5f;
			}

			angle += M_PI / 4.0f;
		}

		break;
	}
}

SDL_bool CodeBoss(Ennemi* ennemi) {

	if (ennemi->type == TYPEENNEMI_BOSS || ennemi->statut == STATUSENNEMI_BOSS_NORMAL) {

		return SDL_FALSE;
	}

	switch (ennemi->statut) {

	case STATUSENNEMI_BOSS_INIT:

		ennemi->self.timer = 0;
		ennemi->self.jeu->joueur->powerup = TYPEITEM_NONE;
		ennemi->self.jeu->joueur->vitesse_tir = JOUEUR_VITESSE_TIR;
		ennemi->self.position = (Vector3){ .x = W_SEMI_LARGEUR, .y = W_SEMI_HAUTEUR, .z = 100 };
		ennemi->statut = STATUSENNEMI_BOSS_INIT_2;

		break;

	case STATUSENNEMI_BOSS_INIT_2:

		ennemi->self.position.z = 100 - ennemi->self.timer;

		if (ennemi->self.position.z <= 20) {

			ennemi->statut = STATUSENNEMI_BOSS_INIT_3;
			ennemi->self.timer = 0;
			ennemi->velocite.y = -10;
			ennemi->self.jeu->bouger_etoiles = SDL_TRUE;
		}

		break;

	case STATUSENNEMI_BOSS_INIT_3:

#ifdef DEBUG_MONOLOGUE_SKIP
		ennemi->self.timer = 1021;
#endif

		if (ennemi->self.timer < 240) {

			AfficherText("allo pilote. ton vaisseau n'est pas le seul.",
				(Vector2) {
				TEXT_CENTRE, W_SEMI_HAUTEUR + 250
			}, 2, TEXT_BLANC, TEXT_OPAQUE, ennemi->self.timer / 2);
		}
		else if (ennemi->self.timer >= 260 && ennemi->self.timer < 480) {

			AfficherText("un espion nous a transféré les plans.",
				(Vector2) {
				TEXT_CENTRE, W_SEMI_HAUTEUR + 250
			}, 2, TEXT_BLANC, TEXT_OPAQUE, ennemi->self.timer / 2 - 130);
		}
		else if (ennemi->self.timer >= 500 && ennemi->self.timer < 800) {

			AfficherText("la bombe à pulsar est très puissante, et",
				(Vector2) {
				TEXT_CENTRE, W_SEMI_HAUTEUR + 250
			}, 2, TEXT_BLANC, TEXT_OPAQUE, ennemi->self.timer / 2 - 250);

			AfficherText("encore plus fragile. je ne peux pas te laisser près d'elle.",
				(Vector2) {
				TEXT_CENTRE, W_SEMI_HAUTEUR + 288
			}, 2, TEXT_BLANC, TEXT_OPAQUE, ennemi->self.timer / 2 - 250);
		}
		else if (ennemi->self.timer >= 820 && ennemi->self.timer < 1020) {

			AfficherText("que le meilleur pilote gagne. en guarde.",
				(Vector2) {
				TEXT_CENTRE, W_SEMI_HAUTEUR + 250
			}, 2, TEXT_BLANC, TEXT_OPAQUE, ennemi->self.timer / 2 - 410);
		}

		if (ennemi->self.timer == 886) {

			JouerEffet(EFFET_DOTV_ENTREE);
		}

		if (ennemi->self.timer > 1020) {

			ennemi->statut = STATUSENNEMI_BOSS_NORMAL;
			ennemi->self.jeu.bombe_pulsar.HP = BOMBE_PULSAR_MAX_HP;
			JouerMusique(MUSIQUE_DOTV, SDL_TRUE);
		}

		break;

	case STATUSENNEMI_BOSS_MORT:

		if (Mix_PlayingMusic()) {

			Mix_HaltMusic();
		}

		if (DistanceV2((Vector2) { .x = ennemi->self.position.x, .y = ennemi->self.position.y }, (Vector2) { .x = W_SEMI_LARGEUR, .y = W_SEMI_HAUTEUR }) > 30) {

			if (ennemi->self.position.x > W_SEMI_LARGEUR) {

				ennemi->self.position.x -= ennemi->vitesse;
			}
			else {

				ennemi->self.position.x += ennemi->vitesse;
			}

			if (ennemi->self.position.y > W_SEMI_HAUTEUR) {

				ennemi->self.position.y -= ennemi->vitesse;
			}
			else {

				ennemi->self.position.y += ennemi->vitesse;
			}
		}
		else {

			ennemi->statut = STATUSENNEMI_BOSS_MORT_2;
			ennemi->self.timer = 0;
		}

		if (ennemi->self.jeu->gTimer % 30 == 0) {

			CreerExplosion(ennemi->self.jeu, (Vector3) { .x = ennemi->self.position.x + RNG(-20, 20), .y = ennemi->self.position.y + RNG(-20, 20), 0 });
		}

		break;

	case STATUSENNEMI_BOSS_MORT_2:

		ennemi->self.position.x = W_SEMI_LARGEUR + RNG(-2, 2);
		ennemi->self.position.y = W_SEMI_HAUTEUR + RNG(-2, 2);

		if (ennemi->self.jeu->gTimer % 30 == 0) {

			CreerExplosion(ennemi->self.jeu, (Vector3) { .x = ennemi->self.position.x + RNG(-20, 20), .y = ennemi->self.position.y + RNG(-20, 20), 0 });
		}

		if (ennemi->self.timer > 300) {

			ennemi->statut = STATUSENNEMI_BOSS_MORT_3;
		}

		break;

	case STATUSENNEMI_BOSS_MORT_3:

		if (ennemi->self.timer == 302) {

			JouerEffet(EFFET_EXPLOSION_JOUEUR);
		}

		ennemi->self.roll += 0.05f;
		ennemi->self.pitch += 0.005f;

		if (ennemi->self.jeu->gTimer % 30 == 0) {

			CreerExplosion(ennemi->self.jeu, (Vector3) { .x = ennemi->self.position.x + RNG(-20, 20), .y = ennemi->self.position.y + RNG(-20, 20), 0 });
		}

		if (ennemi->self.timer < UINT8_MAX) {

			ennemi->self.jeu->couleure_fond_ecran.a = (u8)ennemi->self.timer;
			ennemi->self.couleure.a = (u8)(0xFF - ennemi->self.timer);
		}

		if (ennemi->self.timer) {

			ennemi->self.jeu->gamemode = GAMEMODE_SCENE_MAUVAISE_FIN;

			ClearEnnemis(ennemi->self.jeu);
			ClearExplosions(ennemi->self.jeu);
		}

		break;
	}

	return SDL_TRUE;
}

int CollisionProjectileEnnemi(Ennemi* ennemi, Projectile* projectile) {

	if (!ennemi->self.afficher || projectile->proprietaire == PROPRIETAIREPROJ_ENNEMI) {

		return 0;
	}

	if (SDL_fabsf(projectile->self.position.z - ennemi->self.position.z) > 2.0f && !projectile->laser) {

		return 0;
	}

	if (projectile->laser && projectile->self.jeu->gTimer % 10 != 0) {

		return 0;
	}

	float proj_pos[4];
	PositionsSurEcran(projectile, proj_pos);

	if (DistanceV2((Vector2) { .x = ennemi->self.position.x, .y = ennemi->self.position.y }, (Vector2) { .x = proj_pos[0], .y = proj_pos[1] }) > ennemi->largeur) {

		return 0;
	}

	ennemi->HP--;
	CreerExplosion(ennemi->self.jeu, ennemi->self.position);

	if (!projectile->laser) {

		projectile->self.afficher = SDL_FALSE;
	}

	if (ennemi->statut > STATUSENNEMI_PATRA_0_RESTANT && ennemi->statut <= STATUSENNEMI_PATRA_8_RESTANT) {

		ennemi->HP++;
		ennemi->statut--;

		CreerEnnemi(
			ennemi->self.jeu,
			ennemi->type == TYPEENNEMI_PATRA ? TYPEENNEMI_PATRA_MINION : TYPEENNEMI_PATRA_MINION_DUR,
			STATUSENNEMI_INITIALIZATION,
			ennemi
		);
	}

	if (ennemi->HP > 0) {

		return 0;
	}

	ennemi->self.afficher = SDL_FALSE;

	if (ennemi->type == TYPEENNEMI_BOSS) {

		ennemi->statut = STATUSENNEMI_BOSS_MORT;
		return 1;
	}

	ennemi->self.afficher = SDL_FALSE;

	if (ennemi->type == TYPEENNEMI_PATRA_MINION || ennemi->type == TYPEENNEMI_PATRA_MINION_DUR) {

		ennemi->statut = STATUSENNEMI_MORT;
		return 1;
	}

	if (ennemi->statut > STATUSENNEMI_DUPLIQUEUR_0_RESTANT && ennemi->statut <= STATUSENNEMI_DUPLIQUEUR_2_RESTANT) {

		const i32 DUPLIQUEUR_DIST_SEPARATION_ENFANT = 30;
		StatusEnnemi nouveau_status = ennemi->statut - 1;

		ennemi->self.position.x += DUPLIQUEUR_DIST_SEPARATION_ENFANT;
		CreerEnnemi(ennemi->self.jeu, ennemi->type, nouveau_status, ennemi);
		ennemi->self.position.x -= DUPLIQUEUR_DIST_SEPARATION_ENFANT * 2;
		CreerEnnemi(ennemi->self.jeu, ennemi->type, nouveau_status, ennemi);

		ennemi->statut = STATUSENNEMI_MORT;
		return 1;
	}

	ennemi->statut = STATUSENNEMI_MORT;
	ennemi->self.jeu->ennemis_tues++;
	CreerItem(ennemi);

	return 1;
}

int CollisionJoueurEnnemi(Ennemi* ennemi, Joueur* joueur) {

	const i32 DOMMAGES_COLLISION_JOUEUR = 3;

	if (ennemi->self.position.z != 0) {

		return 0;
	}

	if (DistanceV3(ennemi->self.position, joueur->self.position) > JOUEUR_LARGEUR) {

		return 0;
	}

	CreerExplosion(ennemi->self.jeu, ennemi->self.position);
	joueur->HP -= DOMMAGES_COLLISION_JOUEUR;

	if (! // <--- !!!
		(ennemi->type == TYPEENNEMI_PATRA_MINION
			|| ennemi->type == TYPEENNEMI_PATRA_MINION_DUR
			|| (ennemi->statut >= STATUSENNEMI_DUPLIQUEUR_0_RESTANT && ennemi->statut < STATUSENNEMI_DUPLIQUEUR_2_RESTANT)
			|| (ennemi->statut > STATUSENNEMI_PATRA_0_RESTANT && ennemi->statut <= STATUSENNEMI_PATRA_8_RESTANT)
			)) {

		ennemi->self.jeu->ennemis_restant++;
	}

	ennemi->statut = STATUSENNEMI_MORT;
	ennemi->self.afficher = SDL_FALSE;

	return 1;
}

Vector2 TrouverCibleEnnemi(Ennemi* ennemi) {

	const i32 BORD_EVITER = 100;
	const i32 BORD_EVITER_BAS = 400;
	const i32 LIMITE_BOUCLE = 500;

	i32 anti_boucle_infini = 0;
	Vector3 pos_joueur = ennemi->self.jeu->joueur->self.position;

	if (ennemi->self.position.z == 0) {

		return (Vector2) { .x = pos_joueur.x, .y = pos_joueur.y };
	}

	i32 dist_cible = DistanceV2((Vector2) { .x = ennemi->cible.x, .y = ennemi->cible.y }, (Vector2) { ennemi->self.position.x, ennemi->self.position.y });

	if (dist_cible > 30) {

		return ennemi->cible;
	}

	Vector2 nouveau;
	Vector3* joueur_pos = &ennemi->self.jeu->joueur->self.position;
	float dist_joueur_ennemi = DistanceV2(
		(Vector2) { .x = ennemi->self.position.x, .y = ennemi->self.position.y },
		(Vector2) { .x = joueur_pos->x, .y = joueur_pos->y }
	);

	if (dist_joueur_ennemi < 200) {

		do {

			nouveau.x = RNG(BORD_EVITER, W_LARGEUR - BORD_EVITER);
			nouveau.y = RNG(BORD_EVITER, W_HAUTEUR - BORD_EVITER_BAS);

			if (++anti_boucle_infini > LIMITE_BOUCLE)
				break;
		}
		while (DistanceV2(nouveau, (Vector2) { .x = joueur_pos->x, .y = joueur_pos->y }) < 800);

		return nouveau;
	}

	if (dist_joueur_ennemi < 800) {

		float yVerif = joueur_pos->y;
		if (pos_joueur.y > W_HAUTEUR - BORD_EVITER_BAS)
			yVerif -= BORD_EVITER_BAS;

		return (Vector2) { .x = joueur_pos->x, yVerif };
	}

	do {

		nouveau.x = RNG(BORD_EVITER, W_LARGEUR - BORD_EVITER);
		nouveau.y = RNG(BORD_EVITER, W_HAUTEUR - BORD_EVITER_BAS);

		if (++anti_boucle_infini > LIMITE_BOUCLE)
			break;
	}
	while (DistanceV2(nouveau, (Vector2) { .x = joueur_pos->x, .y = joueur_pos->y }) > 800);

	return nouveau;
}

void BougerEnnemi(Ennemi* ennemi) {

	const float LIM_MIN_Z_ENNEMI = 1.0f;
	const float ACCELERATION_ENNEMI_Z0 = 1.01f;
	const float MAX_PITCH = 0.25f;

	if (ennemi->vitesse_z != 0 && ennemi->self.position.z != 0) {

		ennemi->self.position.z -= ennemi->vitesse_z / G_FPS;

		if (ennemi->self.position.z < LIM_MIN_Z_ENNEMI) {

			ennemi->self.position.z = 0;
		}
	}

	if (ennemi->vitesse != 0) {

		if (ennemi->self.position.x < ennemi->cible.x)
			ennemi->velocite.x += ennemi->vitesse;
		else if (ennemi->self.position.x > ennemi->cible.x)
			ennemi->velocite.x -= ennemi->vitesse;

		if (ennemi->self.position.y < ennemi->cible.y)
			ennemi->velocite.y += ennemi->vitesse;
		else if (ennemi->self.position.y > ennemi->cible.y)
			ennemi->velocite.y -= ennemi->vitesse;

		ennemi->velocite.x *= ENNEMI_FRICTION;
		ennemi->velocite.y *= ENNEMI_FRICTION;
		ennemi->self.position.x += ennemi->velocite.x;
		ennemi->self.position.y += ennemi->velocite.y;
	}

	if (ennemi->self.position.z == 0) {

		ennemi->vitesse *= ACCELERATION_ENNEMI_Z0;
	}

	ennemi->self.pitch = ((ennemi->self.position.y - W_SEMI_HAUTEUR) / W_SEMI_HAUTEUR) * MAX_PITCH;

	if (ennemi->type == TYPEENNEMI_DUPLIQUEUR || ennemi->type == TYPEENNEMI_DUPLIQUEUR_DUR || ennemi->type == TYPEENNEMI_BOSS) {

		ennemi->self.roll = ennemi->velocite.x / 15;
	}

	if (ennemi->type == TYPEENNEMI_BOSS) {

		ennemi->self.pitch -= 0.5f;
	}
}

void TirEnnemi(Ennemi* ennemi) {

	if (ennemi->intervale_tir <= 0) {

		return;
	}

	if (SDL_fmodf(ennemi->self.timer, ennemi->intervale_tir) >= 1) {

		return;
	}

	if (ennemi->self.position.z < MAX_PROFONDEUR / 4) {

		return;
	}

	float data_ligne[4];
	Vector3* pos_joueur = &ennemi->self.jeu->joueur->self.position;
	for (int i = 0; i < sizeof(ennemi->self.indexs_de_tir) / sizeof(float); i++) {

		PositionLigneModele(&ennemi->self, ennemi->self.indexs_de_tir[i], data_ligne);

		Vector3 cible = *pos_joueur;

		if (ennemi->type == TYPEENNEMI_BOSS) {

			cible = (Vector3){
				.x = pos_joueur->x + ennemi->velocite.x * ennemi->self.position.z,
				.y = pos_joueur->y + ennemi->velocite.y * ennemi->self.position.z,
				.z = pos_joueur->z
			};
		}

		CreerProjectile(
			ennemi->self.jeu,
			(Vector3) { .x = data_ligne[0], .y = data_ligne[1], .z = ennemi->self.position.z },
			cible,
			PROPRIETAIREPROJ_ENNEMI,
			i
		);
	}
}

void RenderEnnemi(Ennemi* ennemi) {

	if (ennemi->type == TYPEENNEMI_ENERGIE || ennemi->type == TYPEENNEMI_ENERGIE_DUR) {

		DessinerBombePulsar(
			(Vector2) { ennemi->self.position.x, ennemi->self.position.y },
			(u8)(40 * SDL_powf(0.95f, ennemi->self.position.z)),
			ennemi->self.couleure,
			SDL_FALSE
		);

		return;
	}

	RenderSprite(&ennemi->self);
}