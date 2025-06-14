#ifndef GRILLE_H
#define GRILLE_H
#define MAX_LINE 150
#include <stdlib.h>
#include <stdio.h>
enum element {VIDE, MUR, PION, DANGER, SMILEY, PROJECTILE_JOUEUR};

typedef struct Grille_s
{  
  int n;
  int m;
  enum element ** tab;
} Grille;



Grille * Grille_initialiser(int n, int m);
void Grille_desallouer(Grille ** G);
void Grille_vider(Grille * G); 
void Grille_redessiner(Grille * G);
void Placer_Mur(Grille * G, int n, int m);
void Placer_Danger(Grille * G, int n, int m);
void Vider_case(Grille * G, int n, int m);

void Placer_nouv_Pion(Grille * G, int n, int m);
void Placer_nouv_SMILEY(Grille * G, int n, int m);
void Placer_nouv_SMILEY_FIN(Grille * G, int n, int m);

Grille * Grille_charger_fichier(char * nom_fich);

void Erreur_chargement(char * nom_fich);
void lecture_ligne(char * ligne, FILE * f, char * nom_fich);

#endif
