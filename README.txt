Titre: BOSS-FIGHT SMILEY
Auteur: Lamine Sangaret

Installation: Utiliser la commande make pour utiliser le Makefile

Usage: Lancer ./main_jeu 

SYNOPSIS:
Dans ce jeu le joueur contrôle un carré bleu qui peut bouger et tirer des projectiles dans une grille. Il y a un monstre SMILEY qui essaiera d'attaquer le pion du joueur
en se deplacant et en tirant des projectiles de manières imprévisible. Le but du jeu est de vaincre SMILEY avec ses projectiles tout en l'esquivant, si lui ou un de ses
projectiles te touchent, tu meurs, tu reviendra à la vie 3 fois mais si tu perd ta 3e vie tu as perdu.

CODE:
Dans tous le code "SMILEY" voudra toujours dire le carré 8x8 que représente le monstre.

Le deplacement du pion est le même que celui de la partie_A.
Un projectile est représenté par un struct similaire à celui d'un pion, juste avec un champ qui prend sa direction (pour qu'il puisse bouger continuellement) et
un qui prend son "type" (s'il vient du joueur ou de SMILEY).
SMILEY est un projectile car il aura toujours une direction, il sera juste deplacé et dessiné d'une manière particulière avec d'autres fonctions.
Quand un projectile est "lancé" il est en fait mis à la tête d'une liste chaînée qui contient tous les projectiles existants sur la grille. À chaque fin de boucle on
parcours cette liste en déplacant tous les projectiles et en les desallouants (avce leurs maillons) s'ils touchent une cible et en les dessinants sinon.

Des évènements sont vérifiés à la fin de la boucle de jeu et le comportement de la prise de l'entrée de l'utilisateur, et de la mise à jour de la grille et son
redessinage dépendent de certains de ces évènements. Cela permet des affichages sympas, l'invicibilité temporaire du joueur quand il revient à la vie et la fin du jeu.

C'est la structure générale du code.

DIFFICULTES:

Il y a souvent des manières plus intelligentes d'utiliser des fonctions précédentes pour en faire de nouvelles en changeant les arguments, donc une grande partie
de ce jeu à été pensé sur papier et crayon d'abord. Cela dit il y a comme même eu boucoup de réecritures de fonctions dans tous le fichiers du code.
Le debogeur ddd était souvent inutilisable à cause de l'interactivité constante avec l'utilisateur, et je ne pouvait pas faire les changements de variables que 
je voulais. J'ai du placer des "printf("check\n");" souvent pour donc trouver des bugs et avec l'affichage particulier de ncurses ce n'était pas toujours évident. 

Mais sinon le projet était très amusant.

Fin du jeu: touche ESC

PS: Le jeu peut être un peu dur donc n'hesitez pas à changer la constante PV_SMILEY_DEBUT dans Fonctions_jeu.h. c'est le nombre de projectiles à lancer sur SMILEY pour
le vaincre.
