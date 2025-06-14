all: main_jeu

Fonctions_Jeu.o: Fonctions_Jeu.c Fonctions_Jeu.h Grille.h Pion.h
	gcc -g -c -Wall Fonctions_Jeu.c
		
main_jeu.o: main_jeu.c Fonctions_Jeu.h Grille.h
	gcc -g -c -Wall main_jeu.c
	
Grille.o: Grille.c Grille.h
	gcc -g -c -Wall Grille.c
	
Pion.o: Pion.c Pion.h Grille.h
	gcc -g -c -Wall Pion.c
	
main_jeu: main_jeu.o Fonctions_Jeu.o Grille.o Pion.o
	gcc -g -o main_jeu main_jeu.o Fonctions_Jeu.o Grille.o Pion.o -lncurses

clean:
	rm -fr *.o main_jeu
