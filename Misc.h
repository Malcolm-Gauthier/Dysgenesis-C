#pragma once
#include "Dysgenesis.h"

typedef struct BombePulsar {
    i32 HP;
    i32 timer;
    SDL_bool frapee;
} BombePulsar;

const u8 QUANTITE_RAYONS_BOMBE_PULSAR = 50;
const i32 BOMBE_PULSAR_MAX_HP = 50;
#define COULEUR_BOMBE (SDL_Color){ .r = 150, .g = 255, .b = 255, .a = 255 }

const float hyperbole_bleue_bombe_pulsar_data[72] = {
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

void DessinerBombePulsar(Vector2 position, u8 rayon, SDL_Color couleure, SDL_bool hyperbole_bleue);

int AnimationExplosionBombe(BombePulsar* bombe);

int CollisionBombeProjectile(BombePulsar* bombe, Jeu* jeu);

SDL_bool ExistBombe(BombePulsar* bome);


const i32 DENSITE_ETOILES = 100;
const i32 RAYON_DEBUT_ETOILES = 100;
const float VITESSE_ETOILES = 1.02f;

int CreerEtoiles(Vector2* etoiles, i32 limite);

void BougerEtoiles(Vector2* etoiles, i32 nb_etoiles);

void RenderEtoiles(Vector2* etoiles, i32 nb_etoiles);


// documentation texte
// 
//   text: le texte qui sera affiché à l'écran
//         charactères supportés: a-z, 0-9, +, -, é, è, ê, à,  , ., ,, ', :, \, /, ", (, ), \n
//         lettres sont majuscule seuelement, mais le texte qui rentre dans la fonction doit être minuscule, les majuscules seront automatiquement
//         convertis en minuscules avant d'êtres déssinés.
//         \n fonctionne et est la seule facon de passer à une prochaine ligne dans le même appel de texte, et quand la ligne est sautée, il revient
//         au x de départ.
//
//   x, y: position haut gauche du premier charactère affiché.
//         mettre Text.CENTRE (ou -2147483648) pour centrer le texte au millieu de l'écran.
//
//   size: nombre qui donne le multiple de la largeure et hauteure.
//         la largeur d'un charactère sera de 5 * size px, et la hauteur de 10 * size px.
//
//  color: couleure RGB du texte, où blanc est la valeure par défaut
//         R, G et B attachés ensemble en un chiffre, où les bits 23 à 16 sont pour le rouge, 15 à 8 pour le vert et 7 à 0 pour le bleu.
//
//  alpha: transparence du texte, 100% opaque par défaut. Ceci sera la valeure A du RGBA de sdl, et va automatiquement
//         arrondir à une valeure de byte si dépassement.
//
// scroll: le nb. de charactères que seront affichés à l'écran, peu importe la longeure du texte.
//         int.MaxValue par défaut. si scroll est négatif, aucun texte n'est affiché.
//
//
#define CENTRE INT32_MAX
#define NO_SCROLL INT32_MAX
#define BLANC 0xFFFFFF
#define ROUGE 0xFF0000
#define VERT 0x00FF00
#define BLEU 0x0000FF
#define NOIR 0x000000
#define OPAQUE 255

const float LARGEUR_DEFAUT = 5.0f;
const float HAUTEUR_DEFAUT = 10.0f;
const float ESPACE_DEFAUT = 3.0f;

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
    else if (alpha > UINT8_MAX)
        alpha = UINT8_MAX;

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

        while (current_info_index < CHAR_DRAW_INFO_LEN)
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


void Scene0(i32 gTimer);

void Scene4(i32 gTimer);

typedef enum OptionsCurseur {
    CURSEUR_NOUVELLE_PARTIE,
    CURSEUR_CONTINUER,
    CURSEUR_ARCADE,
    CURSEUR_AUCUN
} OptionsCurseur;
const int NB_OPTIONS = 4;

typedef struct Curseur {
    Sprite self;

    i32 max_selection;
    OptionsCurseur selection;
    OptionsCurseur option_selectionee;
} Curseur;

const int CURSEUR_DAS = G_FPS / 4;
const int CURSEUR_X_INIT;
const int CURSEUR_Y_INIT;
const int CURSEUR_ESPACE = 50;
const Vector3 curseur_modele[] =
{
    { .x=-15, .y=-15, .z=0},
    { .x=15, .y=0, .z=0 },
    { .x=-15, .y=15, .z=0},
    { .x=-12, .y=0, .z=0},
    { .x=-15, .y=-15, .z=0}
};

Curseur* CreerCurseur(Jeu* jeu);

SDL_bool ExistCurseur(Curseur* curseur);


typedef struct Explosion {
    Jeu* jeu;
    Vector3 position;
    i32 timer;
} Explosion;

Explosion* CreerExplosion(Jeu* jeu, Vector3 position);

SDL_bool ExistExplosion(Explosion* explosion);

void RenderExplosion(Explosion* explosion);


typedef enum EffetAudio {
    EFFET_PRESENTE,
    EFFET_NIVEAU,
    EFFET_TIR,
    EFFET_EXPLOSION_ENNEMI,
    EFFET_EXPLOSION_JOUEUR,
    EFFET_POWERUP,
    EFFET_VAGUE,
    EFFET_DOTV_ENTREE
} EffetAudio;

typedef enum Musique {
    MUSIQUE_ATW,
    MUSIQUE_CRTP,
    MUSIQUE_DYSGENESIS,
    MUSIQUE_TBOT,
    MUSIQUE_DOTV,
    MUSIQUE_EUGENESIS,
    MUSIQUE_DCQBPM,
} Musique;

#define NB_CHAINES_SFX 20
typedef struct Son {
    Mix_Music* musique;
    Mix_Chunk* effets_sonnores[NB_CHAINES_SFX];

    i32 timer;
    i32 index_prochain_chunk;
    u8 volume;
    i32 volume_sdl;
    SDL_bool render;
} Son;

#define ALL_CHUNKS -1
static SDL_Rect boite_volume =
{
    .x = W_LARGEUR - 360,
    .y = 10,
    .w = 350,
    .h = 100
};

int InitSDLMixer(Son* son);

int JouerMusique(Jeu* jeu, Musique musique_a_jouer, SDL_bool boucle);

int JouerEffet(Jeu* jeu, EffetAudio effet_a_jouer);

void ChangerVolume(Jeu* jeu);

void RenderVolume(Son* son);