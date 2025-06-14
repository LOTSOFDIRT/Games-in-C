#include<ncurses.h>
#include "Fonctions_Jeu.h"
#include "Grille.h"
#include "Pion.h"
  
  
  
void initialiser_liste_vide(Liste * l)
{
  *l = NULL;
}

Liste maillon_initialiser(Projectile * P)
{
  Liste l = malloc(sizeof(struct maillon));
  l->projectile = P;
  l->suiv = NULL;
  return l;
}
void maillon_desallouer(Liste m)
{
  Projectile_desallouer(m->projectile);
  free(m);
}
void detruire_liste(Liste l)
{
  if(l != NULL)
  {
    detruire_liste(l->suiv);
    free(l);
  }
}

void gaumer_Projectile_liste(Grille * G, Liste l)
{
  Liste m;
  for(m=l; m != NULL; m = m->suiv)
    Enlever_Projectile(G,m->projectile);
}
void ajouter_Projectile_liste(Liste * L, int x, int y, enum evenement dir, enum element elmt)
{
  Projectile * P = Projectile_initialiser(x,y,dir,elmt);
  Liste m = maillon_initialiser(P);
  
  if(*L == NULL)
    m->suiv = NULL;
  else
    m->suiv = *L;
  *L = m;
}


void Mouvement_projectiles_liste(Grille * G, Liste * L, int * PV_S)
{
  Liste m, tmp;
  //Il faudra desallouer et remplacer les pointeurs donc pour un maillon m on traitera le maillon m->suiv
  //on traite donc d'abord le premier séparément parce qu'il n'est pas un "suiv", on prend ainsi l'adresse de L pour traiter le maillon *L
  
  while(*L != NULL && Deplacer_Projectile_maillon(G,*L,PV_S)) //si le projectile du maillon doit disparaitre
  {
    tmp = (*L)->suiv;               //on desalloue et enlève le maillon de la liste
    maillon_desallouer(*L);
    *L = tmp;
  }
  
  if(*L != NULL) //cad. la boucle c'est arrêtée parce que le Deplacement du premier maillon n'a pas nécessité une destruction de projectile
  { 
    //Le premier maillon a été traiter, on fait donc la meme choses pour les autre en prenant m pour traiter m->suiv
    for(m=*L; m->suiv != NULL; m = (m->suiv != NULL) ? m->suiv : m) 
    /*l'opération de boucle m = m->suiv ne se produira que si m->suiv n'est pas NULL pour éviter une seg fault lorsqu'on vérifie m->suiv != NULL*/
    {
      while(m->suiv != NULL && Deplacer_Projectile_maillon(G,m->suiv,PV_S)) //si le projectile du maillon disparait
      {
        tmp = m->suiv->suiv;               //on desalloue et enlève le maillon de la liste
        maillon_desallouer(m->suiv);
        m->suiv = tmp;
      }
    }
    
  }
  //sinon elle s'arrête parce que tous les projectiles devaient être detruits et la liste est donc vide
}
int Deplacer_Projectile_maillon(Grille * G, Liste L, int * PV_S)
{
  return Deplacer_Projectile(G,L->projectile, PV_S);
}
int Deplacer_Projectile(Grille * G, Projectile * P, int * PV_S)
{
  Deplacer_Pion(P->pion,P->direction);  //affecte juste les coordonnées
  switch(G->tab[P->pion->y][P->pion->x])
  {
    case MUR:
      Enlever_Projectile(G,P);
      return 1; //declanche la destruction du maillon voir le if juste en haut
      break;   
    case DANGER:      //Si SMILEY est touché il prend des dégâts
      if(P->de_qui == PROJECTILE_JOUEUR)
      {
        *PV_S -= 1;
        Enlever_Projectile(G,P);
        return 1; 
      }
      else
      {
        G->tab[P->pion->old_y][P->pion->old_x] = VIDE;
        P->pion->old_x = P->pion->x;
        P->pion->old_y = P->pion->y;
        Placer_Projectile(G,P);
        return 0;
      }
      break;
    default:        //Le cas pour le joueur sera traité dans la boucle
      G->tab[P->pion->old_y][P->pion->old_x] = VIDE;
      P->pion->old_x = P->pion->x;
      P->pion->old_y = P->pion->y;
      Placer_Projectile(G,P);
      return 0;
  }
}

void Action_SMILEY(Grille * G, Projectile * S, Liste * L) //retourne la prochaine action de smiley, voir nouvelle_action()
{
  int i;
  //ACTION TIR DE SMILEY
  if(S->direction == TIR_SMILEY)
  {
    for(i=1; i<=6; i++)
    {
      ajouter_Projectile_liste(L,S->pion->x, S->pion->y + i, GAUCHE, DANGER);
      ajouter_Projectile_liste(L,S->pion->x + 7, S->pion->y + i, DROITE, DANGER);
      ajouter_Projectile_liste(L,S->pion->x + i, S->pion->y , HAUT, DANGER);
      ajouter_Projectile_liste(L,S->pion->x + i, S->pion->y + 7, BAS, DANGER);
    }
    S->direction = nouvelle_action();
    Placer_SMILEY(G,S);
  }
  
  //DEPLACEMENT DE SMILEY
  Deplacer_Pion(S->pion,S->direction);    //affecte les coordonnées du coin en haut à gauche du cube de SMILEY
  S->direction = Verifie_mvmt_SMILEY(G,S);
  Placer_SMILEY(G,S);
}

enum evenement nouvelle_action(void)
{
  int nouv_action = rand() % 9;
  switch(nouv_action) //renvoie la prochaine action de SMILEY, une nouvelle s'il touche un MUR
  {
    case 0 : 
      return GAUCHE;
      break;
    case 1 : 
      return DROITE;
      break;
    case 2 : 
      return BAS;
      break;
    case 3 : 
      return HAUT;
      break;
    case 4 : 
      return DIAG_BAS_GAUCHE;
      break;
    case 5 : 
      return DIAG_BAS_DROITE;
      break;
    case 6 : 
      return DIAG_HAUT_GAUCHE;
      break;
    case 7 : 
      return DIAG_HAUT_DROITE;
      break;
    case 8 : 
      return TIR_SMILEY;
      break;
    default:
      return BAS;
  }
}
enum evenement Verifie_mvmt_SMILEY(Grille * G, Projectile * S) //on ajuste les cases qu'on vérifie avec la direction de SMILEY car il est 8x8
{
  enum evenement dir = S->direction;
  if(dir == GAUCHE || dir == HAUT || dir == DIAG_HAUT_GAUCHE)
    return Verifie_case_SMILEY(G,S,S->pion->y,S->pion->x);
    
  if(dir == DROITE || dir == BAS || dir == DIAG_BAS_DROITE)
    return Verifie_case_SMILEY(G,S,S->pion->y + 7,S->pion->x + 7);

  if(dir == DIAG_HAUT_DROITE)
    return Verifie_case_SMILEY(G,S,S->pion->y, S->pion->x + 7);

  if(dir == DIAG_BAS_GAUCHE)
    return Verifie_case_SMILEY(G,S,S->pion->y + 7,S->pion->x);
  return BAS;
}

enum evenement Verifie_case_SMILEY(Grille * G, Projectile * S, int n, int m)
{
  switch(G->tab[n][m])
    {
      case MUR:
        S->pion->y = S->pion->old_y;
        S->pion->x = S->pion->old_x;
        return nouvelle_action();
        break;

      default:
        gaumer_SMILEY(G,S);
        S->pion->old_x = S->pion->x;
        S->pion->old_y = S->pion->y;
        return S->direction;
    }
}

void gaumer_SMILEY(Grille * G, Projectile * S)
{
  int i;
  switch(S->direction)
  {
    case DROITE:
      for(i=1; i<=6; i++)
        Vider_case(G, S->pion->old_y + i, S->pion->old_x);
      break;
    case GAUCHE:
      for(i=1; i<=6; i++)
        Vider_case(G, S->pion->old_y + i, S->pion->old_x + 7);
      break;
    case HAUT:
      for(i=1; i<=6; i++)
        Vider_case(G, S->pion->old_y + 7, S->pion->old_x + i);
      break;
    case BAS:
      for(i=1; i<=6; i++)
        Vider_case(G, S->pion->old_y, S->pion->old_x + i);
      break;
    case DIAG_BAS_GAUCHE:
      for(i=1; i<=6; i++)
      {
        Vider_case(G, S->pion->old_y, S->pion->old_x + i);
        Vider_case(G, S->pion->old_y + i, S->pion->old_x + 7);
      }
      break;
    case DIAG_BAS_DROITE:
      for(i=1; i<=6; i++)
      {
        Vider_case(G, S->pion->old_y, S->pion->old_x + i);
        Vider_case(G, S->pion->old_y + i, S->pion->old_x);
      }
      break;
    case DIAG_HAUT_GAUCHE:
      for(i=1; i<=6; i++)
      {
        Vider_case(G, S->pion->old_y + 7, S->pion->old_x + i);
        Vider_case(G, S->pion->old_y + i, S->pion->old_x + 7);
      }
      break;
    case DIAG_HAUT_DROITE:
      for(i=1; i<=6; i++)
      {
        Vider_case(G, S->pion->old_y + 7, S->pion->old_x + i);
        Vider_case(G, S->pion->old_y + i, S->pion->old_x);
      }
      break;
    default:
  }
}

void dessiner_barre_SMILEY(int PV)
{
  int i;
  printf("%s","\33[47m[\33[41m");
  for(i=0; i<PV/2; i++)
    printf("#");
  printf("%s","\33[43m");
  for(i=PV/2; i<PV_SMILEY_DEBUT/2; i++)
    printf("-");

  printf("%s","\33[47m]\33[1E");
  for(i=0; i<PV_SMILEY_DEBUT/2 + 2; i++)
    printf(" ");
  printf("%s","\33[00m\33[1E");
}



void Jeu_Partie_A(){

  enum evenement event = TESTING; /* Retour des évènements  */
  int ch;          /* Récupération de la saisie clavie, valeur # pour fin de boucle */
  int PV_SMILEY = PV_SMILEY_DEBUT;
  int PV_JOUEUR = 3;
  int wait = 0 ; /*pour faire des temps d'attente, sera decrementé en fin de boucle*/
  
  //Initialisation d'objets
  Grille * G = Grille_charger_fichier("grille_B"); //GRILLE
  Pion * P = Pion_de_grille(G); //JOUEUR
  Projectile * Smiley = Projectile_initialiser(31,1,BAS,SMILEY); //SMILEY l'enemy du JOUEUR
  Liste l;    //Liste chaînée qui contiendra tous les projectiles existant pour les deplacer
  initialiser_liste_vide(&l);
  
  /* Initialisation de ncurses et du clavier(4 lignes)*/
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  halfdelay(1);  /* Temps d'exécution max de getch à 1/10ème de seconde */

  //BOUCLE DE JEU
  do{
    ch = getch(); /* Si aucune touche utilisée, getch renvoie -1 */   

    //INPUT JOUEUR
    if (ch!=-1 && event != RESPAWN)
    {
     switch(ch) 
     {  
        case 'z':   /*Deplacement*/
           Deplacement_jeu(G,P,HAUT);     
            break;
        case 's': 
           Deplacement_jeu(G,P,BAS);    
           break;
        case 'q': 
           Deplacement_jeu(G,P,GAUCHE);
           break; 
        case 'd': 
           Deplacement_jeu(G,P,DROITE);
           break;
        case 'f': 
           event = RIEN;
           break;
        case KEY_UP:   /*Tir de Projectiles*/
           ajouter_Projectile_liste(&l, P->x, P->y, HAUT, PROJECTILE_JOUEUR);   
           break;
        case KEY_DOWN: 
           ajouter_Projectile_liste(&l, P->x, P->y, BAS, PROJECTILE_JOUEUR); 
           break;
        case KEY_RIGHT: 
           ajouter_Projectile_liste(&l, P->x, P->y, DROITE, PROJECTILE_JOUEUR); 
           break; 
        case KEY_LEFT: 
           ajouter_Projectile_liste(&l, P->x, P->y, GAUCHE, PROJECTILE_JOUEUR); 
           break;
        case 27:  /* Code de la touche ESC-Echap */
           event=ECHAP;
           break;
        default:
      }
    }
    
  //DESSINAGE
    if(event == FIN_VICTOIRE)//à la fin on enleve tous les projectiles
    {
      gaumer_Projectile_liste(G,l);
      Placer_Pion(G,P);
      Placer_SMILEY_FIN(G,Smiley);
    }
    else//cas général
    {
      if(event != RESPAWN)//le pion est replacer que s'il n'est pas en attente
        Placer_Pion(G,P);
      
      Mouvement_projectiles_liste(G, &l, &PV_SMILEY); //fait le deplacement de tous les projectiles existants decremente PV_SMILEY si SMILEY est touché
      if(event != TESTING)
        Action_SMILEY(G,Smiley, &l); //execute l'action de SMILEY et le remet sur la grille
    }

    if(PV_SMILEY == 0)
      event = MORT_SMILEY;
    
    if(wait <= 0 && G->tab[P->y][P->x] == DANGER && event != RESPAWN)//le joueur est touché si sa case contient une case danger (ca marche car il est placer avant 
      event = JOUEUR_TOUCHE;                                        //Smiley) et s'il n'est pas déjà en train de revenir

    Grille_redessiner(G); 
    if(event != TESTING)
      dessiner_barre_SMILEY(PV_SMILEY);
    
  //VERIFICATION EVENEMENTS
    switch(event)
    {
      case MORT_SMILEY:
        PV_SMILEY = -1;
        event = FIN_VICTOIRE;//declanche un affichage sympa
        break;
      case FIN_VICTOIRE:
        printf("%s","\33[42mBRAVO vous avez vaincus SMILEY !\33[00m\33[1E");
        event = ECHAP;
        break;
      case JOUEUR_TOUCHE:
        PV_JOUEUR--;
        if(PV_JOUEUR != 0)//s'il reste des vies le joueur est replacé
        {
          P->x = 1;
          P->y = 1;
          P->old_x = 1;
          P->old_y = 1;
          wait = 30; //pour 5 secondes le joueur est immobile et il revient après en haut à gauche
          event = RESPAWN; //event devient RESPAWN en attendant
        }
        else//si ses PV sont à 0 il est MORT et le jeu se termine
        {
          printf("%s","\33[41mMORT PAR SMILEY ! : GAME OVER\33[00m\33[1E");
          event = ECHAP;
        }
        break;
      case RESPAWN: //lorsque event = RESPAWN le pion n'est pasafficher
        if(wait <=0)
        {
          wait = 50;
          event = INVINCIBLE;//après 5sec il est affiché et on ne detecte pas s'il est touché (voir plus haut)
        }
        else
          printf("\33[42mOUCH vous avez été touchés ! Il vous reste %d vies\33[00m\33[1E",PV_JOUEUR);
        break;
      case INVINCIBLE:
        if(wait <=0)
          event = RIEN;
        else
          printf("\33[42mVous ne serez plus invincible dans %dsec\33[00m\33[1E",wait/10);
        break;
      case TESTING:
        printf("%s","(ESC pour Sortir)\33[1EPour bouger : utiliser zqsd, Pour tirer : utiliser les flèches, vous devez vaincre SMILEY, Nombre de vies : "); 
        printf("%d\33[1E\33[1EEntrez la touche 'f' quand vous êtes prets à commencer le combat\33[1E",PV_JOUEUR);
        break;
      default:
        printf("%s","(ESC pour Sortir)\33[1EPour bouger : utiliser zqsd, Pour tirer : utiliser les flèches, vous devez vaincre SMILEY, Nombre de vies : "); 
        printf("%d\33[1E\33[1E",PV_JOUEUR);
    }
    
    wait--;//décremente wait chaque 10e de seconde
    fflush(stdout);  /* Force l'affichage complet des commandes précédentes */
  } while (event!=ECHAP);

  Grille_desallouer(&G);
  Pion_desallouer(P);
  
  Projectile_desallouer(Smiley);
  detruire_liste(l);
  printf("\n\nAu revoir !\n");        

  printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");     
   
  wait = 30; //On attend 5sec pour qu'on puisse voir la fin du jeu
  do{
      ch = getch();
      wait -= 1;
  } while(ch==-1 || wait > 0);
  

  endwin(); /* Doit obligatoirement etre mis en fin de programme pour remettre le terminal en etat */  
  
 }
 
 
void Deplacement_jeu(Grille * G, Pion * P, enum evenement e)
{
  Deplacer_Pion(P,e);
  //cas où on sort de la grille (mais on mettra un countour de murs pour faire joli dans tous les cas
  if(P->x < 0 || P->x >= G->m || P->y < 0 || P->y >= G->n)
  {
    P->y = P->old_y;
    P->x = P->old_x;
  }
  //cas des différentes cases de la grille possibles
  switch(G->tab[P->y][P->x])
  {
    case MUR:
      P->y = P->old_y;
      P->x = P->old_x;
      break;
    case DANGER:
      Vider_case(G, P->old_y, P->old_x);
      P->old_x = -1; //annonce que le joueur est mort
      break;
    default:
      G->tab[P->old_y][P->old_x] = VIDE;
      P->old_x = P->x;
      P->old_y = P->y;
  }
  /*Remarque: le cas pour les BUTs ne sont pas mentionné ca fera naturellement en sorte que les BUT disparaissent lorsque le joueur arrive dessus puis se deplasse, car dans la boucle de jeu on vide la case précédente du pion lorsqu'il se deplace pour donner l'impeventsion qu'il bouge et qu'il ne s'allonge pas*/
}

