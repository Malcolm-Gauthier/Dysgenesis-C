#include "Dysgenesis.h"

extern SDL_bool son_cree;
extern const TypeEnnemi* liste_niveaux[21];
extern const i32 liste_niveaux_longueures[21];

const i32 CODE_ARCADE[7] = { 0, TOUCHE_A, TOUCHE_R, TOUCHE_C, TOUCHE_A, TOUCHE_D, TOUCHE_E };
const i32 TOUCHES_VALIDES_ARCADE = TOUCHE_A | TOUCHE_R | TOUCHE_C | TOUCHE_A | TOUCHE_D | TOUCHE_E;

int main(int argc, char* argv[]) {

	Jeu* jeu = SDL_malloc(sizeof(Jeu));

	int i = 0;
	if (i = Init(jeu)) {

		FreeMem(jeu);
		Mix_Quit();
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
	Mix_Quit();
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
	jeu->event = SDL_malloc(sizeof(SDL_Event));
	jeu->etoiles = SDL_malloc(sizeof(Vector2) * DENSITE_ETOILES);
	jeu->curseur = SDL_malloc(sizeof(Curseur));

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

	CreerJoueur(jeu, jeu->joueur);
	for (int i = 0; i < NB_ENNEMIS; i++) jeu->ennemis[i].self.afficher = SDL_FALSE;
	for (int i = 0; i < NB_EXPLOSIONS; i++) jeu->explosions[i].timer = 0;
	for (int i = 0; i < NB_PROJECTILES; i++) jeu->projectiles[i].self.afficher = SDL_FALSE;
	for (int i = 0; i < NB_ITEMS; i++) jeu->items[i].self.afficher = SDL_FALSE;
	CreerCurseur(jeu);
	jeu->vague_electrique->self.afficher = SDL_FALSE;
	jeu->bombe->jeu = jeu;
	jeu->bombe->HP = BOMBE_PULSAR_MAX_HP;
	jeu->gTimer = 0;
	jeu->gamemode = GAMEMODE_SCENE_INITIALIZATION;
	jeu->en_cours = SDL_TRUE;

	return 0;
}

void Controlles(Jeu* jeu) {

	while (SDL_PollEvent(jeu->event)) {

		if (jeu->event->type == SDL_QUIT) {

			jeu->en_cours = SDL_FALSE;
			return;
		}
		else if (jeu->event->type == SDL_KEYDOWN) {

			switch (jeu->event->key.keysym.sym) {

			case SDLK_ESCAPE:
				jeu->en_cours = SDL_FALSE;
				return;

			case SDLK_w:
				jeu->touches_pesees |= TOUCHE_W;
				break;

			case SDLK_a:
				jeu->touches_pesees |= TOUCHE_A;
				break;

			case SDLK_s:
				jeu->touches_pesees |= TOUCHE_S;
				break;

			case SDLK_d:
				jeu->touches_pesees |= TOUCHE_D;
				break;

			case SDLK_j:
				jeu->touches_pesees |= TOUCHE_J;
				break;

			case SDLK_k:
				jeu->touches_pesees |= TOUCHE_K;
				break;

			case SDLK_r:
				jeu->touches_pesees |= TOUCHE_R;
				break;

			case SDLK_c:
				jeu->touches_pesees |= TOUCHE_C;
				break;

			case SDLK_e:
				jeu->touches_pesees |= TOUCHE_E;
				break;

			case SDLK_MINUS:
				jeu->touches_pesees |= TOUCHE_MOINS;
				break;

			case SDLK_EQUALS:
				jeu->touches_pesees |= TOUCHE_PLUS;
				break;

			case SDLK_m:
				jeu->touches_pesees |= TOUCHE_M;
				break;
			}
		}
		else if (jeu->event->type == SDL_KEYUP) {

			switch (jeu->event->key.keysym.sym) {

			case SDLK_ESCAPE:
				jeu->en_cours = SDL_FALSE;
				return;

			case SDLK_w:
				jeu->touches_pesees &= ~TOUCHE_W;
				break;

			case SDLK_a:
				jeu->touches_pesees &= ~TOUCHE_A;
				break;

			case SDLK_s:
				jeu->touches_pesees &= ~TOUCHE_S;
				break;

			case SDLK_d:
				jeu->touches_pesees &= ~TOUCHE_D;
				break;

			case SDLK_j:
				jeu->touches_pesees &= ~TOUCHE_J;
				break;

			case SDLK_k:
				jeu->touches_pesees &= ~TOUCHE_K;
				break;

			case SDLK_r:
				jeu->touches_pesees &= ~TOUCHE_R;
				break;

			case SDLK_c:
				jeu->touches_pesees &= ~TOUCHE_C;
				break;

			case SDLK_e:
				jeu->touches_pesees &= ~TOUCHE_E;
				break;

			case SDLK_MINUS:
				jeu->touches_pesees &= ~TOUCHE_MOINS;
				break;

			case SDLK_EQUALS:
				jeu->touches_pesees &= ~TOUCHE_PLUS;
				break;

			case SDLK_m:
				jeu->touches_pesees &= ~TOUCHE_M;
				break;
			}
		}
	}
}

void Code(Jeu* jeu) {

	ChangerVolume(jeu);

	if (jeu->gamemode == GAMEMODE_SCENE_INITIALIZATION || jeu->gamemode == GAMEMODE_SCENE_GENERIQUE) {

		return;
	}

	if (jeu->bouger_etoiles) {

		BougerEtoiles(jeu->etoiles, DENSITE_ETOILES);
	}

	if (jeu->gamemode == GAMEMODE_MENU_PRINCIPAL) {

		if (!jeu->arcade_debloque) {

			if (!TouchePesee(jeu, CODE_ARCADE[jeu->arcade_etapes]) && TouchePesee(jeu, CODE_ARCADE[jeu->arcade_etapes + 1])) {

				jeu->arcade_etapes++;
				JouerEffet(jeu, EFFET_EXPLOSION_ENNEMI);

				if (jeu->arcade_etapes >= sizeof(CODE_ARCADE) / sizeof(CODE_ARCADE[0]) - 1) {

					jeu->arcade_debloque = SDL_TRUE;
					jeu->debug_lvl_select = SDL_TRUE;
					jeu->curseur->max_selection = 3;
				}
			}
			else if (TouchePesee(jeu, TOUCHES_VALIDES_ARCADE - CODE_ARCADE[jeu->arcade_etapes] - CODE_ARCADE[jeu->arcade_etapes + 1])) {

				jeu->arcade_etapes = 0;
			}
		}

		if (jeu->debug_lvl_select && jeu->gTimer % 10 == 0) {

			if (TouchePesee(jeu, TOUCHE_A) && jeu->niveau_continue > 1) {

				jeu->niveau_continue--;
			}
			else if (TouchePesee(jeu, TOUCHE_D) && jeu->niveau_continue < 20) {

				jeu->niveau_continue++;
			}
		}

		if (ExistCurseur(jeu->curseur)) {

			switch (jeu->curseur->selection) {

			case CURSEUR_NOUVELLE_PARTIE:
				Mix_HaltMusic();
				jeu->niveau = 0;
				InitializerJoueur(jeu->joueur);
				jeu->joueur->self.afficher = SDL_TRUE;
				jeu->gamemode = GAMEMODE_AVENTURE;
				jeu->gTimer = 0;
				break;

			case CURSEUR_CONTINUER:
				Mix_HaltMusic();
				jeu->niveau = jeu->niveau_continue - 1;
				jeu->ennemis_tues = liste_niveaux_longueures[jeu->niveau];
				jeu->ennemis_restant = 0;

				InitializerJoueur(jeu->joueur);
				jeu->joueur->self.afficher = SDL_TRUE;

				jeu->joueur->HP = JOUEUR_MAX_HP / 2;
				jeu->joueur->vagues_electriques = 0;

				jeu->gamemode = GAMEMODE_AVENTURE;
				jeu->gTimer = 0;
				break;

			case CURSEUR_ARCADE:
				jeu->niveau = 0;
				InitializerJoueur(jeu->joueur);
				jeu->joueur->self.afficher = SDL_TRUE;
				JouerMusique(jeu, MUSIQUE_DCQBPM, SDL_TRUE);
				jeu->gamemode = GAMEMODE_ARCADE;
				jeu->gTimer = 0;
				break;

			default:
				break;
			}
		}

		if (TouchePesee(jeu, TOUCHE_M)) {

			jeu->timer_changement_niveau++;

			if (jeu->timer_changement_niveau >= 120) {

				Mix_HaltMusic();
				jeu->gamemode = GAMEMODE_SCENE_GENERIQUE;
				jeu->gTimer = 0;
				jeu->timer_changement_niveau = 0;
			}
		}
		else {

			jeu->timer_changement_niveau = 0;
		}
	}

	if (!GamemodeAction(jeu)) {

		return;
	}

	for (int i = 0; i < NB_ENNEMIS; i++) {

		if (jeu->ennemis[i].self.afficher == SDL_FALSE) {

			continue;
		}

		ExistEnnemi(&jeu->ennemis[i]);
	}

	if (jeu->ennemis[0].type == TYPEENNEMI_BOSS) {

		ExistBombe(jeu->bombe);
	}

	if (ExistJoueur(jeu->joueur)) {

		return;
	}

	if (TouchePesee(jeu, TOUCHE_K)) {

		CreerVagueElectrique(jeu->vague_electrique);
	}
	ExistVagueElectrique(jeu->vague_electrique);

	if (jeu->niveau == -1)
		jeu->niveau = 0;

	int timer_enemy_spawn = 400 / (jeu->niveau + 1);
	if (jeu->gTimer % timer_enemy_spawn == timer_enemy_spawn - 1 &&
		jeu->ennemis_restant > 0 && !JoueurMort(jeu->joueur)) {

		int verif = NbEnnemis(jeu);

		if (jeu->gamemode == GAMEMODE_AVENTURE) {

			CreerEnnemi(jeu, (liste_niveaux[jeu->niveau])[jeu->ennemis_restant - 1], STATUSENNEMI_INITIALIZATION, NULL);
		}
		else {

			CreerEnnemi(jeu, jeu->liste_ennemis_arcade[jeu->ennemis_restant - 1], STATUSENNEMI_INITIALIZATION, NULL);
		}

		if (NbEnnemis(jeu) > verif) {

			jeu->ennemis_restant--;
		}
	}

	if (NbEnnemis(jeu) == 0) {

		if ((jeu->gamemode == GAMEMODE_AVENTURE && jeu->ennemis_tues >= liste_niveaux_longueures[jeu->niveau]) ||
			(jeu->gamemode == GAMEMODE_ARCADE && jeu->ennemis_tues >= jeu->liste_ennemis_arcade_len)) {

			ChangerNiveau(jeu);
		}
	}

	for (int i = 0; i < NB_PROJECTILES; i++) {

		if (jeu->projectiles[i].self.afficher == SDL_FALSE) {

			continue;
		}

		ExistProjectile(&jeu->projectiles[i]);
	}
	son_cree = SDL_FALSE;

	for (int i = 0; i < NB_ITEMS; i++) {

		if (jeu->items[i].self.afficher == SDL_FALSE) {

			continue;
		}

		ItemExist(&jeu->items[i]);
	}

	for (int i = 0; i < NB_EXPLOSIONS; i++) {

		if (jeu->explosions[i].timer == 0) {

			continue;
		}

		ExistExplosion(&jeu->explosions[i]);
	}

	CLAMP(jeu->joueur->HP, 0, JOUEUR_MAX_HP);
	CLAMP(jeu->joueur->vagues_electriques, 0, JOUEUR_MAX_VAGUES);
}

void Render(Jeu* jeu) {

	if (jeu->gamemode >= GAMEMODE_SCENE_INITIALIZATION) {

		switch (jeu->gamemode) {

		case GAMEMODE_SCENE_INITIALIZATION:
			Scene0(jeu, jeu->gTimer);
			break;

		case GAMEMODE_SCENE_GENERIQUE:
			Scene4(jeu, jeu->gTimer);
			break;

		default:
			break;
		}

		return;
	}

	if (GamemodeAction(jeu)) {

		RenderEtoiles(jeu->render, jeu->etoiles, DENSITE_ETOILES);

		if (jeu->ennemis[0].type == TYPEENNEMI_BOSS) {

			DessinerBombePulsar(jeu->render,
				(Vector2) {
				W_SEMI_LARGEUR, W_SEMI_HAUTEUR / 2
			},
				(u8)(25 - jeu->ennemis[0].self.position.z / 4),
				COULEUR_BOMBE,
				SDL_TRUE
			);
		}

		for (int i = 0; i < NB_ENNEMIS; i++) {

			if (jeu->ennemis[i].self.afficher == SDL_FALSE) {

				continue;
			}

			RenderEnnemi(&jeu->ennemis[i]);
		}

		for (int i = 0; i < NB_PROJECTILES; i++) {

			if (jeu->projectiles[i].self.afficher == SDL_FALSE) {

				continue;
			}

			RenderProjectile(&jeu->projectiles[i]);
		}

		for (int i = 0; i < NB_ITEMS; i++) {

			if (jeu->items[i].self.afficher == SDL_FALSE) {

				continue;
			}

			RenderItem(&jeu->items[i]);
		}

		for (int i = 0; i < NB_EXPLOSIONS; i++) {

			if (jeu->explosions[i].timer == 0) {

				continue;
			}

			RenderExplosion(&jeu->explosions[i]);
		}

		RenderSprite(&jeu->joueur->self);

		if (jeu->bombe->HP <= 0) {

			return;
		}

		RenderVagueElectrique(jeu->vague_electrique);

		const SDL_Rect BARRE_HP = { 125, 15, 10, 20 };
		const SDL_Rect BARRE_VAGUE = { 125, 40, 100, 20 };

		SDL_Rect barre_hud = BARRE_HP;
		for (int i = 0; i < jeu->joueur->HP; i++)
		{
			if (i <= 20)
				SDL_SetRenderDrawColor(jeu->render, 255, 0, 0, 255);
			else if (i <= 50)
				SDL_SetRenderDrawColor(jeu->render, 255, 255, 0, 255);
			else if (i <= 100)
				SDL_SetRenderDrawColor(jeu->render, 64, 255, 64, 255);
			else
				SDL_SetRenderDrawColor(jeu->render, 0, 0, 255, 255);
			SDL_RenderFillRect(jeu->render, &barre_hud);
			barre_hud.x += barre_hud.w + 1;
		}

		barre_hud = BARRE_VAGUE;
		int vagues_entiers = (int)SDL_floorf(jeu->joueur->vagues_electriques);
		SDL_SetRenderDrawColor(jeu->render, 0, 255, 255, 255);
		for (int i = vagues_entiers; i > 0; i--)
		{
			SDL_RenderFillRect(jeu->render, &barre_hud);
			barre_hud.x += barre_hud.w + 5;
		}
		
		float vagues_reste = SDL_fmodf(jeu->joueur->vagues_electriques, 1.0f);
		barre_hud.w = (int)(SDL_fmodf(vagues_reste, 0.01f) * 100);
		SDL_RenderFillRect(jeu->render, &barre_hud);

		DisplayText(jeu, "    hp:\nvagues:", (Vector2) { 10, 15 }, 2, BLANC, OPAQUE, NO_SCROLL);

		if (jeu->ennemis[0].type == TYPEENNEMI_BOSS) {

			DisplayText(jeu, "    hp:\nennemi:", (Vector2) { 10, 85 }, 2, BLANC, OPAQUE, NO_SCROLL);
			barre_hud = BARRE_HP;
			barre_hud.y += 70;
			for (int i = 0; i < jeu->ennemis[0].HP; i++)
			{
				if (i <= 20)
					SDL_SetRenderDrawColor(jeu->render, 255, 0, 0, 255);
				else if (i <= 50)
					SDL_SetRenderDrawColor(jeu->render, 255, 255, 0, 255);
				else
					SDL_SetRenderDrawColor(jeu->render, 64, 255, 64, 255);

				SDL_RenderFillRect(jeu->render, &barre_hud);
				barre_hud.x += barre_hud.w + 1;
			}
		}

		if (jeu->gamemode == GAMEMODE_AVENTURE && jeu->niveau < 2) {

			DisplayText(jeu,
				"controles:\n\
				wasd pour bouger\n\
				j pour tirer\n\
				k pour activer une vague électrique",
				(Vector2) {W_SEMI_LARGEUR - 300, W_SEMI_HAUTEUR + 100}, 2, BLANC, OPAQUE, NO_SCROLL
			);
		}

		return;
	}

	if (jeu->gamemode == GAMEMODE_MENU_PRINCIPAL) {

		RenderEtoiles(jeu->render, jeu->etoiles, DENSITE_ETOILES);

		DisplayText(jeu, "dysgenesis",
			(Vector2) { CENTRE, CENTRE }, 5, BLANC, OPAQUE, NO_SCROLL );

		DisplayText(jeu, "nouvelle partie",
			(Vector2) {
			W_SEMI_LARGEUR - 114, W_SEMI_HAUTEUR + 75
		}, 2, BLANC, OPAQUE, NO_SCROLL);

		DisplayText(jeu, "controles menu: w et s pour bouger le curseur,\
			j pour sélectionner\n\ncontroles globaux: esc. pour quitter,\
			+/- pour monter ou baisser le volume",
			(Vector2) {
			10, W_HAUTEUR - 40
		}, 1, BLANC, OPAQUE, NO_SCROLL);

		DisplayText(jeu, "v 0.3c (beta)",
			(Vector2) {
			CENTRE, W_HAUTEUR - 30
		}, 2, BLANC, OPAQUE, NO_SCROLL);

		if (jeu->curseur->max_selection >= 2) {

			char nb[3];
			SDL_itoa(jeu->niveau_continue, nb, 10);
			char txt[25] = "continuer: niveau ";
			SDL_strlcat(txt, nb, 25);

			DisplayText(jeu, txt,
				(Vector2) {
				W_SEMI_LARGEUR - 114, W_SEMI_HAUTEUR + 125
			}, 2, BLANC, OPAQUE, NO_SCROLL);
		}
		
		if (jeu->curseur->max_selection >= 3) {

			DisplayText(jeu, "arcade",
				(Vector2) {
				W_SEMI_LARGEUR - 114, W_SEMI_HAUTEUR + 175
			}, 2, BLANC, OPAQUE, NO_SCROLL);
		}

		RenderSprite(&jeu->curseur->self);
	}
}

void SDLRender(Jeu* jeu) {

	RenderVolume(jeu);

	SDL_SetRenderDrawColor(jeu->render, jeu->couleure_fond_ecran.r, jeu->couleure_fond_ecran.g, jeu->couleure_fond_ecran.b, jeu->couleure_fond_ecran.a);
	SDL_RenderPresent(jeu->render);
	SDL_RenderClear(jeu->render);
}

void FreeMem(Jeu* jeu) {

	SDL_free(jeu->joueur);
	for (int i = 0; i < NB_ENNEMIS; i++) SDL_free(jeu->ennemis[i].modele_en_marche);
	SDL_free(jeu->ennemis);
	SDL_free(jeu->projectiles);
	SDL_free(jeu->explosions);
	SDL_free(jeu->items);

	SDL_free(jeu->vague_electrique);
	SDL_free(jeu->bombe);
	SDL_free(jeu->son);
	SDL_free(jeu->curseur);
	SDL_free(jeu->event);
	SDL_free(jeu->etoiles);

	SDL_DestroyRenderer(jeu->render);
	SDL_DestroyWindow(jeu->fenetre);
	Mix_FreeMusic(jeu->son->musique);
	for (int i = 0; i < NB_CHAINES_SFX; i++) Mix_FreeChunk(jeu->son->effets_sonnores[i]);
	Mix_CloseAudio();

	SDL_free(jeu);
}