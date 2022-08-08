#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "noeud.cpp"


typedef struct agent
{
	int num;
	knot * histoire;
	float dernierVote; //indique le candidat (la valeur d'opinion du..)
	float ecartVote;  //indique l'ecart d'opinion avec le dernier candidat plébiscité par un vote
}Agent;


Agent * creerAgent(int num, float opi, float inc);

vector<float> election(Agent ** tag, vector<float> vCand, int nbElu, int nombre);

float pointIntermediaire(float vDepart, float vArrive, float tDepart, float tArrive, float maxderiv);


float * genererSante(int tour,float maxderiv);

void courbeSante(float * tabSante, int dep, int arri, float maxderiv);

void influenceSante(Agent * agent, float  croissance, float opiPresident, float maxderiv, float g_impactSante);


int * tabRand(int taille);

vector <float> trouverCandidats(Agent ** tag, int div, unsigned int nombre, float maxopi);

void rencontre(Agent *, Agent *, float);

float recouvrement(Agent*, Agent*);

Agent ** creeTabAgent(int nombre, float maxopi, float petitPE, float petitU, float grandU);

float distribOpi(int nb, int num, float maxopi);

float distribInc(int nb,int num,float petitPE, float petitU, float grandU);

float getOpi(Agent* ag);

float getInc(Agent* ag);

void addState(Agent * ag, float opi, float inc);


float getIncByI(Agent* ag, int i);

void voirAgent(Agent *);
