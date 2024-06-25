#include "Niveaux.h"

i32 GenererListeArcade(Jeu* jeu) {

	const i32 VARIABILITE_ENNEMI_CHOISI = 2;
	const float RAPIDITE_DE_DIFFICULTE = 8.0f;

	if (jeu->niveau < 0) {

		return 0;
	}

	i32 longueure_liste_arcade = (i32)SDL_sqrtf(jeu->niveau * 10) + 2;

	if (longueure_liste_arcade < 0) {

		return 0;
	}

	if (jeu->liste_ennemis_arcade) {

		SDL_free(jeu->liste_ennemis_arcade);
	}

	jeu->liste_ennemis_arcade = SDL_malloc(sizeof(TypeEnnemi) * longueure_liste_arcade);

	i32 next_entry;
	i32 formule = (i32)(ARRAY_LEN(ennemis_valide_arcade) * jeu->niveau / (jeu->niveau + RAPIDITE_DE_DIFFICULTE));

	for (i32 i = 0; i < longueure_liste_arcade; i++) {

		next_entry = formule + RNG(-VARIABILITE_ENNEMI_CHOISI, VARIABILITE_ENNEMI_CHOISI + 1);

		if (next_entry < 0)
			next_entry = 0;
		else if (next_entry > longueure_liste_arcade)
			next_entry = longueure_liste_arcade;

		jeu->liste_ennemis_arcade[i] = next_entry;
	}

	return longueure_liste_arcade;
}

void ChangerNiveau(Jeu* jeu) {

	const i32 TEMPS_AVANT_TEXTE = (i32)(3.33f * G_FPS);
	const i32 TEMPS_TEXTE_SUR_ECRAN = (i32)(2.0f * G_FPS) + TEMPS_AVANT_TEXTE;
	const i32 TEMPS_APRES_TEXTE_PARTI = (i32)(0.5f + G_FPS) + TEMPS_TEXTE_SUR_ECRAN;

	if (jeu->gamemode == GAMEMODE_ARCADE && jeu->timer_changement_niveau < TEMPS_AVANT_TEXTE) {

		jeu->timer_changement_niveau = TEMPS_AVANT_TEXTE;
	}

	if (jeu->timer_changement_niveau == TEMPS_AVANT_TEXTE) {

		JouerEffet(jeu, EFFET_NIVEAU);
	}

	if (jeu->timer_changement_niveau > TEMPS_AVANT_TEXTE && jeu->timer_changement_niveau < TEMPS_APRES_TEXTE_PARTI) {

		char num[10];
		char txt[20] = "niveau ";
		SDL_itoa(jeu->niveau + 1, num, 10);
		SDL_strlcat(txt, num, 20);

		DisplayText(jeu, txt, (Vector2){ CENTRE, CENTRE }, 5, BLANC, OPAQUE, NO_SCROLL);
	}

	if (jeu->timer_changement_niveau >= TEMPS_APRES_TEXTE_PARTI) {

		jeu->niveau++;
		jeu->timer_changement_niveau = 0;

		if (jeu->gamemode == GAMEMODE_AVENTURE) {

			//jeu->ennemis_restant = ARRAY_LEN(liste_niveaux[jeu->niveau]);
		}
		else {

			jeu->ennemis_restant = GenererListeArcade(jeu);
		}

		jeu->ennemis_tues = 0;
	}

	jeu->timer_changement_niveau++;
}