#pragma once
#include "Definitions.h"

Projectile* CreerProjectile(Jeu* jeu, Vector3 position, Vector3 destination, ProprietaireProjectile proprietaire, u8 ID);

SDL_bool ExistProjectile(Projectile* projectile);

void TrouverCible(Projectile* projectile);

int PositionsSurEcran(Projectile* projectile, float* output);

int CollisionProjectileJoueur(Projectile* projectile);

void RenderProjectile(Projectile* projectile);


int CreerVagueElectrique(VagueElectrique* vague);

void ExistVagueElectrique(VagueElectrique* vague);

void RenderVagueElectrique(VagueElectrique* vague);