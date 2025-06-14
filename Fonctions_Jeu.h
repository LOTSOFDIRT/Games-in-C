#ifndef _FONCTIONS_JEU_
#define _FONCTIONS_JEU_
#include "Pion.h"
#define PV_SMILEY_DEBUT 100

struct maillon    //maillon d'une liste chaînée de projectiles (voir struct projectile dans Pion.h
{
  Projectile * projectile;
  struct maillon * suiv;
};

typedef struct maillon * Liste;

//Fonction de liste chaînée
void initialiser_liste_vide(Liste * l);
Liste maillon_initialiser(Projectile * P);
void detruire_liste(Liste l);
void maillon_desallouer(Liste m);

void gaumer_Projectile_liste(Grille * G, Liste l); //gaume les projectiles de l de la grille

void ajouter_Projectile_liste(Liste * L, int x, int y, enum evenement dir, enum element de_qui); //ajoute un maillon de projectile en tête de *L

void Mouvement_projectiles_liste(Grille * G, Liste * L, int * PV_S);  /*Deplace les projectilesdes maillons de *L en fonction de leurs directions 
                                                                        respectives, detruit les maillons si leurs projectiles touches un mur ou une cible
                                                                        et les dessine sinon*/
  int Deplacer_Projectile_maillon(Grille * G, Liste l, int * PV_S);
    int Deplacer_Projectile(Grille * G, Projectile * P, int * PV_S);

void Action_SMILEY(Grille * G, Projectile * S, Liste * L);// execute l'action de SMILEY et le redessine
  enum evenement nouvelle_action(void);//génération aléatoire
  enum evenement Verifie_mvmt_SMILEY(Grille * G, Projectile * S);//fait en sorte que SMILEY n sort pas de la grille
    enum evenement Verifie_case_SMILEY(Grille * G, Projectile * S, int n, int m);
    void gaumer_SMILEY(Grille * G, Projectile * P);//gaume les traces de SMILEY de ses anciennes coordonnées

void dessiner_barre_SMILEY(int PV);//dessine la barre de vie de SMILEY

void Deplacement_jeu(Grille * G, Pion * P, enum evenement e);//Deplace le pion sans qu'il sorte de la grille

void Jeu_Partie_A();//Code principal

#endif
