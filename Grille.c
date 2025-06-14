#include "Grille.h"
#include <stdio.h>
#include <stdlib.h>

Grille * Grille_initialiser(int n, int m)
{
  int i;
  Grille * res = malloc(sizeof(Grille));
  res->n = n;
  res->m = m;
  res->tab = malloc(sizeof(enum element *)*n);
  for(i=0; i<n; i++)
    res->tab[i] = malloc(sizeof(enum element)*m);
  Grille_vider(res);
  return res;
}



void Grille_desallouer(Grille ** G)
{
  int i;
  for(i=0; i<(*G)->n; i++)
    free((*G)->tab[i]);
  free((*G)->tab);
  free(*G);
  *G = NULL;
}




void Grille_vider(Grille * G)
{
  int i,j;
  for(i=0; i<G->n; i++)
  {
    for(j=0; j<G->m; j++)
      G->tab[i][j] = VIDE;
  }
}
void Grille_redessiner(Grille * G)
{
  int i,j;
  printf("\33[2J"); /* Efface tout l'ecran */
  printf("\33[H");  /* Deplace le curseur en position (0,0) */
  
  for(i=0; i<G->n; i++)
  {
    for(j=0; j<G->m; j++)
    {
      switch (G->tab[i][j])
      {
        case VIDE:
           printf("%s","\33[00m  ");  /* Affiche un carré rien */
           break;      
        case MUR:
           printf("%s", "\33[47m  ");  /* Affiche un carré noir */
           break;
        case PION:
           printf("%s", "\33[44m  ");  /* Affiche un carré bleu */                                     
           break;
        case PROJECTILE_JOUEUR:
           printf("%s", "\33[42m  ");  /* Affiche un carré bleu */                                     
           break; 
        case DANGER:
          printf("%s", "\33[41m  ");  /* Affiche un carré rouge */ 
          break; 
        default:
      }    
    }
    printf("%s","\33[1E\33[00m");  
  }
  fflush(stdout);  /* Force l'affichage complet des commandes précédentes */
}


void Placer_Mur(Grille * G, int n, int m)
{
  if(n < G->n && m < G->m && n > -1 && m > -1) //evite les seg fault
    G->tab[n][m] = MUR;
}

void Placer_Danger(Grille * G, int n, int m)
{
  if(n < G->n && m < G->m && n > -1 && m > -1) //evite les seg fault
    G->tab[n][m] = DANGER;
}
void Vider_case(Grille * G, int n, int m)
{
  if(n < G->n && m < G->m && n > -1 && m > -1) //evite les seg fault
    G->tab[n][m] = VIDE;
}

void Placer_nouv_Pion(Grille * G, int n, int m) //n'utilise pas le struct Pion 
{
  if(n < G->n && m < G->m && n > -1 && m > -1) //evite les seg fault
    G->tab[n][m] = PION;
}
void Placer_nouv_SMILEY(Grille * G, int n, int m)
{
  int i,j;
  if(n + 7 < G->n && m + 7 < G->m && n > -1 && m > -1) //evite les seg fault
  {
    for(i=0; i<8; i++)
    {
      for(j=0; j<8; j++)
        Vider_case(G, n + i, m + j);
    }
    for(int i=1; i<=6; i++)
    {
      Placer_Danger(G, n + i, m);
      Placer_Danger(G, n + i, m + 7);
      Placer_Danger(G, n, m + i);
      Placer_Danger(G, n + 7, m + i);
    }
    Placer_Danger(G, n + 2, m + 2);
    Placer_Danger(G, n + 2, m + 5);
    Placer_Danger(G, n + 4, m + 2);
    Placer_Danger(G, n + 4, m + 5);
    Placer_Danger(G, n + 5, m + 3);
    Placer_Danger(G, n + 5, m + 4);
  }
}
void Placer_nouv_SMILEY_FIN(Grille * G, int n, int m)
{
  int i,j;
  if(n + 7 < G->n && m + 7 < G->m && n > -1 && m > -1) //evite les seg fault
  {
    for(i=0; i<8; i++)
    {
      for(j=0; j<8; j++)
        Vider_case(G, n + i, m + j);
    }
    for(int i=1; i<=6; i++)
    {
      Placer_Danger(G, n + i, m);
      Placer_Danger(G, n + i, m + 7);
      Placer_Danger(G, n, m + i);
      Placer_Danger(G, n + 7, m + i);
    }
    Placer_Danger(G, n + 2, m + 2);
    Placer_Danger(G, n + 2, m + 5);
    Placer_Danger(G, n + 4, m + 3);
    Placer_Danger(G, n + 4, m + 4);
    Placer_Danger(G, n + 5, m + 2);
    Placer_Danger(G, n + 5, m + 5);
  }
}



void Erreur_chargement(char * nom_fich)
{
  fprintf(stderr,"Erreur de format du fichier %s : écrire en première ligne du fichier juste le nombre de grilles\n puis il faut que chaque grille s'écrive\nn m\nXXXXXX\nXXXXXX\nXXXXXX\nXXXXXX\n où n m sont les dimensions et X des caractères représentant les cases de la grille: parmi 'M'(Mur), ' '(Vide), 'B'(But), 'P'(pion)\n",nom_fich);
  exit(2);
}

void lecture_ligne(char * ligne, FILE * f,char * nom_fich)
{
  if(fgets(ligne,MAX_LINE,f) == NULL)
    Erreur_chargement(nom_fich);
} 

Grille * Grille_charger_fichier(char * nom_fich)
{
/*
  FORMAT:
  écrire en première ligne du fichier juste le nombre de grilles
  puis il faut que chaque grille s'écrive
  n m
  XXXXXX
  XXXXXX
  XXXXXX
  XXXXXX
  où n m sont les dimensions et X des caractères représentant les cases de la grille: parmi 'M'(Mur), ' '(Vide), 'B'(But), 'P'(pion)
  
  RENVOIE : un tableau contenant les pointeurs vers les grilles
*/
  Grille * grille;
  int n, m;
  int i, j;
  int elmt; //convertira les caractères en leur code ascii pour utiliser switch()
  char ligne[MAX_LINE]; 
  FILE * f = fopen(nom_fich,"r");
  if(f == NULL)
  {
    fprintf(stderr,"Erreur d'ouverture du fichier %s",nom_fich);
    exit(2);
  }
  
  lecture_ligne(ligne, f, nom_fich);  
  sscanf(ligne,"%d %d",&n, &m); //on enregistre les dimensions
  grille = Grille_initialiser(n,m);
  
  //Remplissage des grille

  for(i=0; i < grille->n; i++)
  {
    lecture_ligne(ligne, f, nom_fich); //pour chaque ligne de la grille on lit une ligne du fichier

    for(j=0; j < grille->m; j++)
    {
      elmt = ligne[j];// int n = 'c' met le code ascii de 'c' dans n
      switch(elmt)
      {
        case 'M' : Placer_Mur(grille,i,j); break;
        case 'P' : Placer_nouv_Pion(grille,i,j); break;
        default : 
      }
    }
  }
  return grille;
}
  



