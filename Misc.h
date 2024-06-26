#pragma once
#include "Definitions.h"

void DessinerBombePulsar(SDL_Renderer* render, Vector2 position, u8 rayon, SDL_Color couleure, SDL_bool hyperbole_bleue);

int AnimationExplosionBombe(BombePulsar* bombe);

int CollisionBombeProjectile(BombePulsar* bombe, Jeu* jeu);

void ExistBombe(BombePulsar* bombe);


int CreerEtoiles(Vector2* etoiles, i32 limite, SDL_Rect bounds);

void BougerEtoiles(Vector2* etoiles, i32 nb_etoiles);

void RenderEtoiles(SDL_Renderer* render, Vector2* etoiles, i32 nb_etoiles);


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


void Scene0(Jeu* jeu, i32 gTimer);

void Scene4(Jeu* jeu, i32 gTimer);


Curseur* CreerCurseur(Jeu* jeu);

SDL_bool ExistCurseur(Curseur* curseur);


Explosion* CreerExplosion(Jeu* jeu, Vector3 position);

SDL_bool ExistExplosion(Explosion* explosion);

void RenderExplosion(Explosion* explosion);


int InitSDLMixer(Son* son);

int JouerMusique(Jeu* jeu, Musique musique_a_jouer, SDL_bool boucle);

int JouerEffet(Jeu* jeu, EffetAudio effet_a_jouer);

void ChangerVolume(Jeu* jeu);

void RenderVolume(Jeu* son);