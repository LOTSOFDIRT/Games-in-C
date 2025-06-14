#include "Pion.h"
#include "Grille.h"
#include <stdio.h>
#include <stdlib.h>

Pion * Pion_initialiser(int x, int y)
{
  Pion * res = malloc(sizeof(Pion));
  res->x = x;
  res->y = y;
  res->old_x = x;
  res->old_y = y;
  return res;
}
void Pion_desallouer(Pion * P)
{
  free(P);
}
void Placer_Pion(Grille * G,Pion * P)
{
  Placer_nouv_Pion(G,P->y,P->x);
}
void Deplacer_Pion(Pion * P, enum evenement mouv)
{
  switch (mouv) 
  {
    case HAUT:
      P->y--;  /* Affiche un carré rien */
      break;      
    case BAS:
      P->y++;  /* Affiche un carré noir */
      break;
    case DROITE:
      P->x++;  /* Affiche un carré bleu */                                     
      break;
    case GAUCHE:
      P->x--;  /* Affiche un carré rouge */ 
      break;  
    case DIAG_BAS_GAUCHE : 
      P->x--;
      P->y++;
      break;
    case DIAG_BAS_DROITE : 
      P->x++;
      P->y++;
      break;
    case DIAG_HAUT_GAUCHE : 
      P->x--;
      P->y--;
      break;
    case DIAG_HAUT_DROITE : 
      P->x++;
      P->y--;
      break;
    default:
   }    
}

Pion * Pion_de_grille(Grille * G)
{
  int i, j;
  Pion * P;
  for(i=0; i < G->n; i++)
  {
    for(j=0; j < G->m; j++)
    {
      if(G->tab[i][j] == PION)
      {
        P = Pion_initialiser(j,i);
        return P;
      }
    }
  }
  return NULL;
}

Projectile * Projectile_initialiser(int x, int y, enum evenement dir, enum element elmt)
{
  Projectile * res = malloc(sizeof(Projectile));
  res->pion = Pion_initialiser(x,y);
  res->direction = dir;
  res->de_qui = elmt;
  return res;
}

void Projectile_desallouer(Projectile * P)
{
  Pion_desallouer(P->pion);
  free(P);
}
void Placer_Projectile(Grille * G, Projectile * P)
{
  if(P->pion->y < G->n && P->pion->x < G->m && P->pion->y > -1 && P->pion->x > -1) //evite les seg fault, placer un pion hors de la grille est impossible
    G->tab[P->pion->y][P->pion->x] = P->de_qui;
}
void Placer_SMILEY(Grille * G, Projectile * P)
{
  Placer_nouv_SMILEY(G,P->pion->y,P->pion->x);
}
void Placer_SMILEY_FIN(Grille * G, Projectile * P)
{
  Placer_nouv_SMILEY_FIN(G,P->pion->y,P->pion->x);
}
void Enlever_Projectile(Grille * G, Projectile * P)
{
  G->tab[P->pion->old_y][P->pion->old_x] = VIDE;
}
