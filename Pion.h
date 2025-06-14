#ifndef PION_H
#define PION_H
#include "Grille.h"

enum evenement {RIEN, HAUT, BAS, DROITE, GAUCHE, DIAG_BAS_GAUCHE, DIAG_BAS_DROITE, DIAG_HAUT_GAUCHE,DIAG_HAUT_DROITE, TIR_SMILEY, ECHAP, MORT_SMILEY, JOUEUR_TOUCHE, RESPAWN, INVINCIBLE, FIN_VICTOIRE, TESTING};
typedef struct Pion_s
{
  int x;
  int y;
  int old_x;
  int old_y;
} Pion;

typedef struct Projectile_s
{
  Pion * pion;
  enum evenement direction;
  enum element de_qui;
} Projectile;

Pion * Pion_initialiser(int x, int y);
void Pion_desallouer(Pion * P);
void Placer_Pion(Grille * G,Pion * P);
void Deplacer_Pion(Pion * P, enum evenement mouv);
Pion * Pion_de_grille(Grille * G);

Projectile * Projectile_initialiser(int x, int y,enum evenement dir, enum element elmt);
void Projectile_desallouer(Projectile * P);
void Placer_Projectile(Grille * G, Projectile * P);
void Placer_SMILEY(Grille * G, Projectile * P);
void Placer_SMILEY_FIN(Grille * G, Projectile * P);
void Enlever_Projectile(Grille * G, Projectile * P);

#endif
