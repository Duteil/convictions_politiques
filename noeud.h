#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



typedef struct Noeud{
	float inc;
	float opi;
	struct Noeud * suivant;
}knot;

knot * creerNoeud(float opi, float inc);

void detruire(knot* noeud);

void afficheNoeud(knot * noeud);


void afficheListe(knot* liste);
void ajouterFin(knot* noeud, knot * * liste);

void ajouterDebut(knot * noeud, knot * * liste);

knot * dernier(knot * liste);

int longueurListe(knot * Liste);

//void supprimeValeur(Identite  valeur, knot * * liste);


//bool dansListe(Identite  valeur, knot * liste);

void inserer(knot * noeud, knot * * liste, int rang);



knot * recherche(int rang, knot * Liste);

void inverser(knot * * Liste);



//void trier(knot * * liste);

void avancerElt(knot * * liste);

void tri(int* tab, int l);

//void triListeMathieu(knot** liste);


