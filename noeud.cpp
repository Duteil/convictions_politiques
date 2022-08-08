#include "noeud.h"

knot * creerNoeud(float opi, float inc)
{

	knot * retour = (knot*)malloc(sizeof(knot));
	
	retour->opi = opi;

	retour->inc = inc;

	retour->suivant = NULL;
	
	return retour;

}

void afficheNoeud(knot * noeud)
{
	if(noeud != NULL)
		printf(" - %i - %i -\n",noeud->opi , noeud->inc);
	else
		printf("noeud inexistant !\n");
}


void afficheListe(knot* liste)
{
	knot * elt = liste;

	printf("\n---------- debut liste  ----------\n");
	while(elt != NULL)
	{
		afficheNoeud(elt);
		elt=elt->suivant;
	}
	printf("---------- fin liste  ------------\n");
}

void ajouterFin(knot* noeud, knot * * liste)
{
	if(*liste != NULL)
	{
		knot * dernierElt = dernier(*liste);
		dernierElt->suivant = noeud;
	}
	else
		*liste = noeud;
}

void ajouterDebut(knot * noeud, knot * * liste)
{
	noeud->suivant = *liste;
	*liste = noeud;
}

knot * dernier(knot * liste)
{

	knot * retour = liste;

	if(liste == NULL)
		return NULL;

	

	while(retour->suivant != NULL)
	{
		retour=retour->suivant;
	}

	return retour;
}

int longueurListe(knot * Liste)
{
	int Longueur=0;
	while(Liste != NULL)
	{
		Liste = Liste->suivant;
		Longueur++;
	}
	return Longueur;
}

void detruireListe(knot * noeud)
{
knot * elt = noeud;
while(elt != NULL)
	{
	elt = noeud->suivant;
	detruire(noeud);
	noeud = elt;
	}
}

void detruire(knot * noeud)
{
	free ((knot*)noeud);
}


/*bool dansListe(Identite  valeur, knot * Liste)
{
while(Liste != NULL)
	{
		
		if(Liste->valeur == valeur)
			return true;
		Liste = Liste->suivant;
	}
	return false;
}*/


knot * recherche(int rang, knot * Liste)
{
	int Longueur=0;
	while(Liste != NULL && Longueur !=rang)
	{
		Liste = Liste->suivant;
		Longueur++;
	}
	return Liste;


}

void inserer(knot * noeud, knot * * Liste, int rang)
{
	if(rang == 0)
		ajouterDebut(noeud, Liste);
	else if(rang > longueurListe(*Liste))
		ajouterFin(noeud, Liste);	
	else
	{
		noeud->suivant = recherche(rang, *Liste);
		recherche(rang-1, *Liste)->suivant = noeud;
	}
}

void inverser(knot * * Liste)
{
	knot * un;
	knot * deux;
	knot * trois;
	if(!Liste || !*Liste) return;
	un = *Liste;
	deux = un->suivant;
	un->suivant = NULL;


	while(deux)
	{
		trois = deux->suivant;
		deux->suivant = un;
		un = deux;
		deux = trois;
	}
	
	*Liste = un;	
}

void avancerElt(knot * * liste)
{
	/* Declarationn Variable */
	knot * un;
	knot * deux;
	knot * trois;

	/* On teste qu'il reste encore au moins 2 element dans la liste */
	if(!liste) return; 
	if(!*liste) return;
 	un = *liste;


	if(!un->suivant) return;
	/* on stocke l'adresse des elements +1, +2, +3 */
	
	deux = un->suivant;
	trois = deux->suivant;
	
	/* on inverse l'element +1 et +2*/
	*liste = deux;
	deux->suivant = un;
	un->suivant = trois;

}

/*void trier(knot * * liste)
{
	knot * * pElt;
	knot * fc;
	knot * prec;
	bool premz;

	bool range = false;
	if(!*liste) return; 
	fc = *liste;
	pElt = &fc;
	if(!(*pElt)->suivant) return;

	while(!range)
	{
		range = true;
		prec = NULL;
		premz = true;
		while((*pElt)->suivant)
		{
			if((*pElt)->valeur > (*pElt)->suivant->valeur)
			{
				avancerElt(pElt);
				range = false;
				if(premz)
					*liste = *pElt;
				else
					prec->suivant = *pElt;
			}		
			prec = *pElt;
			*pElt = (*pElt)->suivant;
			premz = false;
		}
		*pElt = *liste;
	}
}*/



void tri(int* tab, int l)
{
int i;
int j;
int t;

for(i=1;i<l;i++)
	{for(j=0;j<i;j++)
		{if(tab[i]<=tab[j])
			{
			t=tab[i];
			tab[i]=tab[j];
			tab[j]=t;
			}
		}
	}
return;
}


/*void triListeMathieu(knot** liste)
{
	int l=longueurListe(*liste);
	int* tab = malloc(l * sizeof(int));
	int i;
	knot noeud=(**liste)
	
	if(l==0) return;
	
	for(i=0;i<l;i++)
		{
		tab[i]=noeud.valeur;
		&noeud=noeud.suivant;
		}
	
	tri(tab,l);
	//corriger à partir d'ici
	for(i=0;i<l;i++)
		{
		(*liste)->valeur=tab[i];
		*liste=(*liste)->suivant;
		}
	return;
}*/
