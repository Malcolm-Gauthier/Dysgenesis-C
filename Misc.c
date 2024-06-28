#include "Misc.h"

#define HYP_BOMBE_DATA_LEN 72
const float hyperbole_bleue_bombe_pulsar_data[HYP_BOMBE_DATA_LEN] = {
    -0.35f, -0.95f, -0.4f, -1.55f,
    -0.4f, -1.55f, -0.6f, -2.15f,
    -0.6f, -2.15f, -0.9f, -2.35f,
    0.2f, -1.0f, 0.25f, -1.55f,
    0.25f, -1.55f, 0.35f, -2.25f,
    0.35f, -2.25f, 0.6f, -2.45f,
    -0.15f, -1.15f, -0.15f, -1.8f,
    0.0f, -1.75f, 0.2f, -2.25f,
    -0.25f, -2.25f, -0.4f, -2.45f,
    -0.3f, 0.95f, -0.45f, 1.5f,
    -0.45f, 1.5f, -0.65f, 1.95f,
    -0.65f, 1.95f, -1.0f, 2.2f,
    0.25f, 0.95f, 0.3f, 1.5f,
    0.3f, 1.5f, 0.5f, 2.0f,
    0.5f, 2.0f, 1.0f, 2.3f,
    0.0f, 1.2f, 0.05f, 1.8f,
    -0.1f, 1.55f, -0.15f, 1.95f,
    -0.4f, 1.75f, -0.25f, 1.3f
};

void DessinerBombePulsar(SDL_Renderer* render, Vector2 position, u8 rayon, SDL_Color couleure, SDL_bool hyperbole_bleue) {

    if (hyperbole_bleue) {

        SDL_SetRenderDrawColor(render, 0, 0, 255, 255);

        for (int i = 0; i < HYP_BOMBE_DATA_LEN - 3; i += 4)
        {
            SDL_RenderDrawLine(render,
                (int)(hyperbole_bleue_bombe_pulsar_data[i + 0] * rayon + position.x),
                (int)(hyperbole_bleue_bombe_pulsar_data[i + 1] * rayon + position.y),
                (int)(hyperbole_bleue_bombe_pulsar_data[i + 2] * rayon + position.x),
                (int)(hyperbole_bleue_bombe_pulsar_data[i + 3] * rayon + position.y)
            );
        }
    }

    SDL_SetRenderDrawColor(render, couleure.r, couleure.g, couleure.b, couleure.a);
    DessinerCercle(render, position, rayon, 50);

    float angle;
    for (int i = 0; i < QUANTITE_RAYONS_BOMBE_PULSAR; i++)
    {
        angle = (RNG(0, RAND_MAX) / (float)RAND_MAX) * M_PI;

        SDL_RenderDrawLine(render,
            RNG(-rayon, rayon) * SDL_cos(angle) + position.x,
            RNG(-rayon, rayon) * SDL_sin(angle) + position.y,
            position.x,
            position.y
        );
    }
}

int AnimationExplosionBombe(BombePulsar* bombe) {

    if (bombe->HP > 0) {

        return 0;
    }

    bombe->timer++;

    if (bombe->timer == 1) {

        Mix_HaltMusic();
        JouerEffet(bombe->jeu, EFFET_EXPLOSION_JOUEUR);

        ClearProjectiles(bombe->jeu);
        if (bombe->jeu->joueur->HP <= 0) {

            bombe->jeu->joueur->HP = 1;
        }
    }
    else if (bombe->timer < 200) {

        i32 opacite = bombe->timer * 4;
        if (opacite >= SDL_MAX_UINT8) {

            opacite = SDL_MAX_UINT8;
        }

        SDL_SetRenderDrawColor(bombe->jeu->render, 255, 255, 255, opacite);
        SDL_RenderFillRect(bombe->jeu->render, NULL);
    }
    else {

        bombe->timer = 0;
        bombe->jeu->gamemode = GAMEMODE_SCENE_BONNE_FIN;
        bombe->HP = BOMBE_PULSAR_MAX_HP;
    }

    return 1;
}

int CollisionBombeProjectile(BombePulsar* bombe, Jeu* jeu) {

    if (jeu->ennemis[0].statut != STATUSENNEMI_BOSS_NORMAL) {

        return 0;
    }

    for (int i = 0; i < NB_PROJECTILES; i++) {

        if (jeu->projectiles[i].self.position.z < MAX_PROFONDEUR - 1) {

            continue;
        }

        float positions[4];
        if (PositionsSurEcran(&jeu->projectiles[i], positions)) {
            return 0;
        }

        if (DistanceV2((Vector2) { .x = positions[2], .y = positions[3] }, (Vector2) { .x = W_SEMI_LARGEUR, .y = W_SEMI_HAUTEUR / 2}) < 20) {

            bombe->HP--;
            CreerExplosion(bombe->jeu, (Vector3) { .x = positions[2], .y = positions[3], .z = MAX_PROFONDEUR / 4 });
            bombe->frapee = SDL_TRUE;
        }
    }

    return bombe->frapee;
}

void ExistBombe(BombePulsar* bombe) {

    if (AnimationExplosionBombe(bombe) == 0) {

        CollisionBombeProjectile(bombe, bombe->jeu);
    }
}


int CreerEtoiles(Vector2* etoiles, i32 limite, SDL_Rect bounds) {

    /*if (etoiles != NULL) {

        SDL_free(etoiles);
    }

    etoiles = SDL_malloc(sizeof(Vector2) * limite);

    if (etoiles == NULL) {

        return 1;
    }*/

    if (bounds.w <= 0 || bounds.h <= 0) {

        return 2;
    }

    for (int i = 0; i < limite; i++) {

        etoiles[i].x = RNG(bounds.x, bounds.x + bounds.w);
        etoiles[i].y = RNG(bounds.y, bounds.y + bounds.h);

        if (etoiles[i].x == W_SEMI_LARGEUR && etoiles[i].y == W_SEMI_HAUTEUR) {

            etoiles[i].x++;
        }
    }

    return 0;
}

void BougerEtoiles(Vector2* etoiles, i32 nb_etoiles) {

    for (i32 i = 0; i < nb_etoiles; i++) {

        etoiles[i].x = (etoiles[i].x - W_SEMI_LARGEUR) * VITESSE_ETOILES + W_SEMI_LARGEUR;
        etoiles[i].y = (etoiles[i].y - W_SEMI_HAUTEUR) * VITESSE_ETOILES + W_SEMI_HAUTEUR;

        if (etoiles[i].x < W_LARGEUR &&
            etoiles[i].y < W_HAUTEUR &&
            etoiles[i].x > 0 &&
            etoiles[i].y > 0
            ) {

            continue;
        }

        etoiles[i].x = RNG(0, W_LARGEUR);
        etoiles[i].y = RNG(0, W_HAUTEUR);

        if (etoiles[i].x == W_SEMI_LARGEUR && etoiles[i].y == W_SEMI_HAUTEUR) {

            etoiles[i].x++;
        }
    }
}

void RenderEtoiles(SDL_Renderer* render, Vector2* etoiles, i32 nb_etoiles) {

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    
    for (int i = 0; i < nb_etoiles; i++) {

        SDL_RenderDrawPoint(render, etoiles[i].x, etoiles[i].y);
    }
}


void Scene0(Jeu* jeu, i32 gTimer) {

#ifdef DEBUG_CUTSCENE_SKIP
    gTimer = 451;
#endif

    if (gTimer == 75)
    {
        JouerEffet(jeu, EFFET_PRESENTE);
    }

    if (gTimer >= 75 && gTimer < 150)
    {
        DisplayText(jeu, "malcolm gauthier", (Vector2){CENTRE, CENTRE}, 2, BLANC, OPAQUE, NO_SCROLL);
        DisplayText(jeu, "\n présente", (Vector2) { CENTRE, CENTRE }, 2, BLANC, OPAQUE, NO_SCROLL);

    }
    else if (gTimer >= 150 && gTimer <= 225)
    {
        DisplayText(jeu, "malcolm gauthier", (Vector2) { CENTRE, CENTRE }, 2, BLANC, (225 - gTimer) * 3.4f, NO_SCROLL);
        DisplayText(jeu, "\n présente", (Vector2) { CENTRE, CENTRE }, 2, BLANC, (225 - gTimer) * 3.4f, NO_SCROLL);
    }

    if (gTimer > 225)
    {
        jeu->gamemode = GAMEMODE_MENU_PRINCIPAL;
        JouerMusique(jeu, MUSIQUE_DYSGENESIS, SDL_TRUE);
    }
}

void Scene4(Jeu* jeu, i32 gTimer) {

    //TODO
}


const i16 char_draw_info_starting_indexes[] = {
    1, 18, 47, 60, 81, 98, 111, 132, 145, 158, 171, 184, 193, 210, 223, 240, // a - p
    257, 278, 299, 320, 329, 342, 351, 368, 377, 386, 399, 420, 433, 454, 471, // q - 4
    484, 513, 534, 543, 564, 585, 590, 599, 604, 609, 630, 651, 676, 697, 706, // 5 - ??
    711, 716, 721, 734, 747
};
#define CHAR_DRAW_INFO_LEN 756
const i8 char_draw_info[CHAR_DRAW_INFO_LEN] = {
    127, // space/unknown

    0, 10, 0, 0, // a
    0, 0, 5, 0,
    5, 0, 5, 10,
    5, 5, 0, 5, 127,

    0, 10, 0, 0, // b
    0, 0, 5, 0,
    5, 0, 5, 3,
    5, 3, 0, 5,
    0, 5, 5, 7,
    5, 7, 5, 10,
    5, 10, 0, 10, 127,

    0, 10, 0, 0, // c
    0, 0, 5, 0,
    5, 10, 0, 10, 127,

    0, 10, 0, 0, // d
    0, 0, 2, 0,
    2, 10, 0, 10,
    2, 10, 5, 5,
    2, 0, 5, 5, 127,

    0, 10, 0, 0, // e
    0, 0, 5, 0,
    5, 10, 0, 10,
    5, 5, 0, 5, 127,

    0, 10, 0, 0, // f
    0, 0, 5, 0,
    5, 5, 0, 5, 127,

    0, 10, 0, 0, // g
    0, 0, 5, 0,
    5, 10, 0, 10,
    5, 10, 5, 5,
    5, 5, 3, 5, 127,

    0, 10, 0, 0, // h
    5, 0, 5, 10,
    5, 5, 0, 5, 127,

    0, 0, 5, 0, // i
    2, 0, 2, 10,
    5, 10, 0, 10, 127,

    5, 0, 5, 10, // j
    5, 10, 0, 10,
    0, 10, 0, 7, 127,

    0, 10, 0, 0, // k
    0, 5, 5, 0,
    0, 5, 5, 10, 127,

    0, 10, 0, 0, // l
    5, 10, 0, 10, 127,

    0, 10, 0, 0, // m
    5, 0, 5, 10,
    0, 0, 2, 5,
    5, 0, 2, 5, 127,

    0, 10, 0, 0, // n
    5, 0, 5, 10,
    0, 0, 5, 10, 127,

    0, 10, 0, 0, // o
    5, 0, 5, 10,
    0, 0, 5, 0,
    5, 10, 0, 10, 127,

    0, 10, 0, 0, // p
    0, 0, 5, 0,
    5, 0, 5, 5,
    5, 5, 0, 5, 127,

    0, 10, 0, 0, // q
    4, 0, 4, 10,
    0, 0, 4, 0,
    4, 10, 0, 10,
    5, 10, 3, 5, 127,

    0, 10, 0, 0, // r
    0, 0, 5, 0,
    5, 0, 5, 5,
    5, 5, 0, 5,
    2, 5, 5, 10, 127,

    0, 0, 5, 0, // s
    5, 10, 0, 10,
    5, 5, 0, 5,
    0, 0, 0, 5,
    5, 10, 5, 5, 127,

    2, 0, 2, 10, // t
    0, 0, 5, 0, 127,

    0, 10, 0, 0, // u
    5, 0, 5, 10,
    5, 10, 0, 10, 127,

    0, 0, 2, 10, // v
    5, 0, 2, 10, 127,

    0, 10, 0, 0, // w
    5, 0, 5, 10,
    5, 10, 0, 10,
    2, 10, 2, 4, 127,

    0, 0, 5, 10, // x
    5, 0, 0, 10, 127,

    5, 0, 0, 10, // y
    0, 0, 2, 5, 127,

    0, 0, 5, 0, // z
    5, 10, 0, 10,
    5, 0, 0, 10, 127,

    5, 0, 0, 10, // 0
    0, 10, 0, 0,
    5, 0, 5, 10,
    0, 0, 5, 0,
    5, 10, 0, 10, 127,

    0, 3, 2, 0, // 1
    2, 0, 2, 10,
    5, 10, 0, 10, 127,

    0, 3, 1, 0, // 2
    1, 0, 4, 0,
    5, 3, 4, 0,
    5, 3, 0, 10,
    5, 10, 0, 10, 127,

    5, 10, 5, 0, // 3
    0, 0, 5, 0,
    5, 10, 0, 10,
    5, 5, 0, 5, 127,

    0, 5, 0, 0, // 4
    5, 0, 5, 10,
    5, 5, 0, 5, 127,

    5, 0, 0, 0, // 5
    0, 5, 0, 0,
    5, 5, 0, 5,
    5, 5, 5, 8,
    5, 8, 4, 10,
    4, 10, 1, 10,
    1, 10, 0, 8, 127,

    0, 10, 0, 0, // 6
    0, 0, 5, 0,
    5, 10, 0, 10,
    5, 10, 5, 5,
    5, 5, 0, 5, 127,

    5, 0, 0, 0, // 7
    5, 0, 0, 10, 127,

    5, 5, 0, 5, // 8
    0, 10, 0, 0,
    5, 0, 5, 10,
    0, 0, 5, 0,
    5, 10, 0, 10, 127,

    5, 10, 5, 0, // 9
    0, 0, 5, 0,
    5, 10, 0, 10,
    0, 0, 0, 5,
    5, 5, 0, 5, 127,

    0, 10, 0, 10, 127, // .

    0, 3, 0, 3, // :
    0, 7, 0, 7, 127,

    2, 8, 0, 10, 127, // ,

    2, 2, 0, 0, 127, // '

    0, 10, 0, 0, // é
    0, 0, 5, 0,
    5, 10, 0, 10,
    5, 5, 0, 5,
    1, -2, 4, -4, 127,

    0, 10, 0, 0, // è
    0, 0, 5, 0,
    5, 10, 0, 10,
    5, 5, 0, 5,
    1, -4, 4, -2, 127,

    0, 10, 0, 0, // ê
    0, 0, 5, 0,
    5, 10, 0, 10,
    5, 5, 0, 5,
    1, -2, 2, -4,
    4, -2, 2, -4, 127,

    0, 10, 0, 0, // à
    0, 0, 5, 0,
    5, 0, 5, 10,
    5, 5, 0, 5,
    1, -2, 4, 0, 127,

    2, 0, 2, 3, // "
    4, 0, 4, 3, 127,

    1, 5, 4, 5, 127, // -

    0, 10, 5, 0, 127, // /

    5, 10, 0, 0, 127, // \

    4, 0, 2, 3, // (
    2, 3, 2, 7,
    4, 10, 2, 7, 127,

    1, 0, 3, 3, // )
    3, 3, 3, 7,
    1, 10, 3, 7, 127,

    0, 5, 4, 5, // +
    2, 3, 2, 7, 127
};

static int GetListEntry(char c)
{
    if (c >= 'a' && c <= 'z')
        return char_draw_info_starting_indexes[c - 'a'];
    else if (c >= '0' && c <= '9')
        return char_draw_info_starting_indexes[c - '0' + 26];
    else
    {
        switch (c)
        {
        case '.':
            return char_draw_info_starting_indexes[36];
        case ':':
            return char_draw_info_starting_indexes[37];
        case ',':
            return char_draw_info_starting_indexes[38];
        case '\'':
            return char_draw_info_starting_indexes[39];
        case 'é':
            return char_draw_info_starting_indexes[40];
        case 'è':
            return char_draw_info_starting_indexes[41];
        case 'ê':
            return char_draw_info_starting_indexes[42];
        case 'à':
            return char_draw_info_starting_indexes[43];
        case '"':
            return char_draw_info_starting_indexes[44];
        case '-':
            return char_draw_info_starting_indexes[45];
        case '/':
            return char_draw_info_starting_indexes[46];
        case '\\':
            return char_draw_info_starting_indexes[47];
        case '(':
            return char_draw_info_starting_indexes[48];
        case ')':
            return char_draw_info_starting_indexes[49];
        case '+':
            return char_draw_info_starting_indexes[50];
        }
    }

    return 0;
}

void DisplayText(Jeu* jeu, char* text, Vector2 position, float size, int color, int alpha, int scroll)
{
    if (scroll <= 0)
        return;

    if (alpha <= 0)
        return;

    i32 strlen = SDL_strlen(text);

    char* working_text = (char*)SDL_malloc(strlen + 1);
    SDL_memcpy(working_text, text, strlen + 1);
    for (int i = 0; working_text[i]; i++) {
        working_text[i] = SDL_tolower(working_text[i]);
    }

    i16 extra_y = 0;
    i32 return_length = 0;

    if (scroll < 0)
        scroll = 0;
    else if (scroll > strlen)
        scroll = strlen;

    if (alpha < 0)
        alpha = 0;
    else if (alpha > SDL_MAX_UINT8)
        alpha = SDL_MAX_UINT8;

    if (position.x == CENTRE)
        position.x = W_SEMI_LARGEUR - ((LARGEUR_DEFAUT + ESPACE_DEFAUT) * size * strlen - 1) / 2;

    if (position.y == CENTRE)
        position.y = W_SEMI_HAUTEUR - (HAUTEUR_DEFAUT * size) / 2;

    SDL_SetRenderDrawColor(jeu->render, (u8)((color >> 16) & 0xFF), (u8)((color >> 8) & 0xFF), (u8)(color & 0xFF), (u8)alpha);

    float x;
    float y;
    int current_info_index;
    for (int i = 0; i < scroll; i++)
    {
        y = position.y + extra_y;
        x = position.x + i * (LARGEUR_DEFAUT + ESPACE_DEFAUT) * size - return_length;

        if (x + size * LARGEUR_DEFAUT * 4 > W_LARGEUR)
        {
            extra_y += (i16)((HAUTEUR_DEFAUT + ESPACE_DEFAUT) * size);
            return_length = (i32)((i + 1) * 8 * size);
        }

        if (working_text[i] == '\0')
            break;

        if (working_text[i] == '\n')
        {
            extra_y += (i16)((HAUTEUR_DEFAUT + ESPACE_DEFAUT) * size);
            return_length = (i32)((i + 1) * (LARGEUR_DEFAUT + ESPACE_DEFAUT) * size);
            continue;
        }

        current_info_index = GetListEntry(working_text[i]);

        while (current_info_index < CHAR_DRAW_INFO_LEN - 3)
        {
            if (char_draw_info[current_info_index] == 127)
                break;

            SDL_RenderDrawLine(jeu->render,
                char_draw_info[current_info_index] * size + x,
                char_draw_info[current_info_index + 1] * size + y,
                char_draw_info[current_info_index + 2] * size + x,
                char_draw_info[current_info_index + 3] * size + y
            );

            current_info_index += 4;
        }
    }

    SDL_free(working_text);
}


#define MODELE_CURSEUR_LONGUEURE 5
const Vector3 curseur_modele[MODELE_CURSEUR_LONGUEURE] = {
    {15, -15, 0},
    {-15, 0, 0 },
    {15, 15, 0},
    {12, 0, 0},
    {15, -15, 0}
};

Curseur* CreerCurseur(Jeu* jeu) {

    if (jeu->curseur != NULL) {

        SDL_free(jeu->curseur);
    }

    Curseur* curseur = SDL_malloc(sizeof(Curseur));

    InitSprite(&curseur->self, jeu);
    curseur->self.modele = curseur_modele;
    curseur->self.modele_longueur = MODELE_CURSEUR_LONGUEURE;
    curseur->max_selection = 1;
    curseur->option_selectionee = CURSEUR_NOUVELLE_PARTIE;
    curseur->selection = CURSEUR_AUCUN;
    curseur->self.afficher = SDL_TRUE;
    curseur->self.position.x = 800;

    return curseur;
}

SDL_bool ExistCurseur(Curseur* curseur) {

    if (curseur->self.timer > CURSEUR_DAS) {

        if (TouchePesee(curseur->self.jeu, TOUCHE_J)) {

            curseur->self.timer = 0;
            curseur->selection = curseur->option_selectionee;

            return SDL_TRUE;
        }

        if (TouchePesee(curseur->self.jeu, TOUCHE_W) && curseur->option_selectionee > 0) {

            curseur->self.timer = 0;
            curseur->option_selectionee--;
        }

        if (TouchePesee(curseur->self.jeu, TOUCHE_S) && curseur->option_selectionee < curseur->max_selection - 1) {

            curseur->self.timer = 0;
            curseur->option_selectionee++;
        }

        curseur->self.position.y = CURSEUR_Y_INIT + CURSEUR_ESPACE * curseur->option_selectionee;
    }
    else if (!TouchePesee(curseur->self.jeu, TOUCHE_W) && !TouchePesee(curseur->self.jeu, TOUCHE_S)) {

        curseur->self.timer = CURSEUR_DAS;
    }

    curseur->self.timer++;
    curseur->selection = CURSEUR_AUCUN;
    return SDL_FALSE;
}


Explosion* CreerExplosion(Jeu* jeu, Vector3 position) {

    Explosion* explosion = NULL;
    for (int i = 0; i < NB_PROJECTILES; i++) {

        if (!jeu->explosions[i].timer > 0) {

            explosion = &jeu->explosions[i];
            break;
        }
    }

    if (explosion == NULL) {

        return NULL;
    }

    explosion->jeu = jeu;
    explosion->position = position;
    explosion->timer = MAX_PROFONDEUR - (i32)explosion->position.z;

    return explosion;
}

SDL_bool ExistExplosion(Explosion* explosion) {

    return --explosion->timer <= 0;
}

void RenderExplosion(Explosion* explosion) {

    const i32 RAYON_MIN_EXPLOSION = 2;
    const i32 DENSITE_EXPLOSION = 50;

    if (JoueurMort(explosion->jeu->joueur)) {

        return;
    }

    if (explosion->timer == MAX_PROFONDEUR) {

        explosion->timer--;
    }

    u8 rayon = (u8)(MAX_PROFONDEUR * 8.0f / (MAX_PROFONDEUR - explosion->timer) + RAYON_MIN_EXPLOSION);

    for (int i = 0; i < DENSITE_EXPLOSION; i++)
    {
        float angle = (RNG(0, RAND_MAX) / (float)RAND_MAX) * M_PI;
        float sin_ang = SDL_sinf(angle);
        float cos_ang = SDL_cosf(angle);

        SDL_SetRenderDrawColor(explosion->jeu->render,
            (u8)RNG(128, 256),
            (u8)RNG(0, 128),
            0,
            255
        );

        SDL_RenderDrawLine(explosion->jeu->render,
            RNG(-rayon, rayon) * cos_ang + explosion->position.x,
            RNG(-rayon, rayon) * sin_ang + explosion->position.y,
            RNG(-rayon, rayon) * cos_ang + explosion->position.x,
            RNG(-rayon, rayon) * sin_ang + explosion->position.y
        );
    }
}


static SDL_Rect boite_volume = {
    W_LARGEUR - 360,
    10,
    350,
    100
};

int InitSDLMixer(Son* son) {

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048)) {

        return 1;
    }

    Mix_AllocateChannels(NB_CHAINES_SFX + 1);
    son->volume = 3;
    son->volume_sdl = (i32)(SDL_MIX_MAXVOLUME * (son->volume / 16.0f));
    Mix_VolumeMusic(son->volume_sdl);
    Mix_Volume(-1, son->volume_sdl / 2);
    son->index_prochain_chunk = 0;
    son->timer = 0;

    return 0;
}

int JouerMusique(Jeu* jeu, Musique musique_a_jouer, SDL_bool boucle) {

#ifdef DEBUG_MUTE_SON
    return 0;
#endif

    if (musique_a_jouer < 0 || musique_a_jouer > NB_MUSIQUE - 1) {

        return -1;
    }

    if (musique_a_jouer != MusiqueData[musique_a_jouer].ID) {

        return -2;
    }

    i32 loupes = boucle ? -1 : 1;

    Mix_FreeMusic(jeu->son->musique);
    jeu->son->musique = Mix_LoadMUS(MusiqueData[musique_a_jouer].fichier);

    if (jeu->son->musique == NULL) {

        return -3;
    }

    if (Mix_PlayMusic(jeu->son->musique, loupes) != 0) {

        return -4;
    }

    Mix_VolumeMusic(jeu->son->volume_sdl);

    return 0;
}

int JouerEffet(Jeu* jeu, EffetAudio effet_a_jouer) {

#ifdef DEBUG_MUTE_SON
    return 0;
#endif

    if (effet_a_jouer < 0 || effet_a_jouer > NB_EFFETS - 1) {

        return -1;
    }

    if (effet_a_jouer != EffetData[effet_a_jouer].ID) {

        return -2;
    }

    Son* son = jeu->son;

    Mix_FreeChunk(son->effets_sonnores[son->index_prochain_chunk]);
    son->effets_sonnores[son->index_prochain_chunk] = Mix_LoadWAV(EffetData[effet_a_jouer].fichier);

    if (son->effets_sonnores[son->index_prochain_chunk] == NULL) {

        return -3;
    }
    
    if (Mix_PlayChannel(son->index_prochain_chunk + 1, son->effets_sonnores[son->index_prochain_chunk], 0) != 0) {

        return -4;
    }

    Mix_Volume(-1, son->volume_sdl / 2);

    son->index_prochain_chunk++;
    son->index_prochain_chunk %= NB_CHAINES_SFX;

    return 0;
}

void ChangerVolume(Jeu* jeu) {

    const i32 MAX_VOLUME_GUI = 16;
    const i32 VOLUME_TEMPS_AFFICHAGE = G_FPS / 2;
    const i32 VOLUME_DAS = G_FPS / 12;

    if (TouchePesee(jeu, TOUCHE_PLUS | TOUCHE_MOINS)) {

        if (jeu->son->timer < VOLUME_TEMPS_AFFICHAGE - VOLUME_DAS) {

            jeu->son->timer = VOLUME_TEMPS_AFFICHAGE;

            if (TouchePesee(jeu, TOUCHE_PLUS) && jeu->son->volume < MAX_VOLUME_GUI)
                jeu->son->volume++;
            if (TouchePesee(jeu, TOUCHE_MOINS) && jeu->son->volume > 0)
                jeu->son->volume--;

            jeu->son->volume_sdl = (i32)(SDL_MIX_MAXVOLUME * (jeu->son->volume / (float)MAX_VOLUME_GUI));
            Mix_VolumeMusic(jeu->son->volume_sdl);
            Mix_Volume(-1, jeu->son->volume_sdl / 2);
        }
    }

    if (jeu->son->timer > 0) {

        jeu->son->timer--;
    }
}

void RenderVolume(Jeu* jeu) {

    if (jeu->son->timer == 0) {

        return;
    }

    SDL_SetRenderDrawColor(jeu->render, 0, 0, 0, 255);
    SDL_RenderFillRect(jeu->render, &boite_volume);
    SDL_SetRenderDrawColor(jeu->render, 255, 255, 255, 255);
    SDL_RenderDrawRect(jeu->render, &boite_volume);

    char vol[20] = "      ";
    char txt[20] = "volume: ";
    SDL_itoa(jeu->son->volume, vol, 10);
    SDL_strlcat(txt, vol, 20);
    DisplayText(jeu, txt, (Vector2) { .x = boite_volume.x + 40, boite_volume.y + 30 }, 3, BLANC, OPAQUE, NO_SCROLL);
}