#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


#include "agent.h"

#define ASSOUPLISSANT 
#define GRANDU 
#define PETITU 
#define PETITPE 
#define NOMBRE 
#define DIVISION 
#define MAXDERIVCROISSANCE 
#define TOUR 
#define DUREEMANDAT 

#define MAXOPI 



int main(int argc, char ** argv)
{
	cout << "================== SIMULATION POLITIQUE ====================\n"<<endl;
	
	
	
	
	////////////////////////////////////////////////////////////////////////
	/////////////dÈfinition des variables globales par defaut///////////////
	////////////////////////////////////////////////////////////////////////
	int g_hasard = time(NULL) ; // graine de hasard
	float g_assouplissant = 0.5; // param√®tre mu de contr√¥le de la vitesse dynamique
	float g_petitPE = 0.1; //proportion d'extrÈmistes dans la population (positifs et n√©gatifs confondus)
	float g_grandU = 1.0; //incertitude des non extrÈmistes
	float g_petitU = 0.1; //incertitude des extrÈmistes
	unsigned int g_nombre = 300; // nombre de sujets
	unsigned int g_division = 50 ;//nombre de divisions de l'intervalle opinions ; l'int√©r√™t de diviser cet intervalle est de d√©gager des tendances d'o√π on pourra tirer des candidats
	float g_maxDerivCroissance = 1.5 ;// limite les variations de l'√©tat de sant√© du pays
	unsigned int g_tour = 400; // nombre de tours
	unsigned int g_dureeMandat = 8 ;// nombre de tours
	float g_maxOpi = 1.0; // valeur absolue de l'opinion extrÍme
	float g_impactSante = 0.1; // paramËtre l'importance de l'influence de la santÈ
	
	///////////////////////////////////////////////////////////////////////
	///////////// Interfacage de prÔøΩinitialisation ////////////////////////
	///////////////////////////////////////////////////////////////////////

	char entree;
	
	do
	{
		//affichage des variables globales
		cout << "\n======= affichage des variables globales ===" << endl;
		cout << "\t a . Nombre d'agents = \t" << g_nombre << endl;  
		cout << "\t b . Nombre de tours = \t" << g_tour << endl;  
		cout << "\t c . Nb de Division d'opinions = \t" << g_division << endl;  
		cout << "\t d . Croissance max pays = \t" << g_maxDerivCroissance << endl;
		cout << "\t e . DurÈe mandat (en tout) = \t" << g_dureeMandat << endl;
		cout << "\t f . Petit pe = \t" << g_petitPE << endl;  
		cout << "\t g . Petit u = \t" << g_petitU << endl; 
		cout << "\t h . Grand U = \t" << g_grandU << endl;  
		cout << "\t i . Assouplissant = \t" << g_assouplissant << endl;   
		cout << "\t j . Maximum Absolu de l'opinion = \t" << g_maxOpi << endl;  
		cout << "\t k . Impact de la santÈ sur Incertitude (entre 0 et 1)= \t" << g_impactSante << endl;  
		cout << "\t l . Graine du hasard = \t" << g_hasard << endl;  
		
		cout << "======= Tappez la lettre d'une variable pour la modifier ===" << endl;
		cout << "======= Tappez 'S' pour lancer la simulation avec ces valeurs ===" << endl;
		
		cin>>entree;
		
		switch(entree)
		{
			case 'A' :
			case 'a' : 	cout << " Nombre d'agents ?";
					cin >> g_nombre;
					break;
			case 'B' :
			case 'b' : 	cout << " Nombre de tour ?";
					cin >> g_tour;	
					break;
			case 'c' :
			case 'C' : 	cout << " Nombre de divisions d'opinions ?";
					cin >> g_division;	
					break;
			case 'D' :
			case 'd' : 	cout << " Croissance max pays ? (0 pour non influence sant√© du pays)";
					cin >> g_maxDerivCroissance;	
					break;
			case 'e' :
			case 'E' : 	cout << " DurÈe mandat (en tout) ?";
					cin >> g_dureeMandat;	
					break;
			case 'F' :
			case 'f' : 	cout << " Petit pe ?";
					cin >> g_petitPE;	
					break;
			case 'G' :
			case 'g' : 	cout << " Petit u ?";
					cin >> g_petitU;	
					break;
			case 'H' :
			case 'h' : 	cout << " Grand U ?";
					cin >> g_grandU;	
					break;
			case 'I' :
			case 'i' : 	cout << " Assouplissant ?";
					cin >> g_assouplissant;	
					break;
			case 'J' :
			case 'j' : 	cout << " Maximum Absolu de l'opinion ?";
					cin >> g_maxOpi;	
					break;

			case 'K' :
			case 'k' : 	cout << " Impact de la sant√© (entre 0 et 1) ?";
					cin >> g_impactSante;	
					break;
			
			case 'L' :
			case 'l' : 	cout << "Graine du hasard ?";
					cin >> g_hasard;	
					break;

			default: ;
		}
	}while(entree != 'S' && entree != 's');	
	
	///////////////////////////////////////////////////////////
	/////////////// Initialisation ////////////////////////////
	///////////////////////////////////////////////////////////

	Agent ** tAg = creeTabAgent(g_nombre, g_maxOpi, g_petitPE, g_petitU, g_grandU);
	
	srand(g_hasard);
	int i;
	int j;
	int gi;
	float tf;
	int * trand;

	//Le president initial est le plus impartial possible
	float opiPresident = 0;
	
	//on gÈnËre la santÈ du pays
	float * tabSante = genererSante(g_tour,g_maxDerivCroissance);

	float santeMax = fabs(tabSante[0]);
	//on cherche la sante max
	for(int yo = 1; yo < g_tour ; yo ++)
	{
		if(santeMax < fabs(tabSante[yo]))
			santeMax = fabs(tabSante[yo]);
	}
	
	//on va Ècrire dans 3 fichiers
	ofstream f_opposition ;
	ofstream f_pouvoir ;
	ofstream f_sante ;


	f_opposition.open("opposition.txt", ios::out | ios::trunc) ;
	f_pouvoir.open("pouvoir.txt", ios::out | ios::trunc) ;
	f_sante.open("sante.txt", ios::out | ios::trunc) ;


	///////////////////////////////////////////////////////////
	/////////////// Simulation ////////////////////////////////
	///////////////////////////////////////////////////////////
	for(gi = 0 ; gi < g_tour ; gi++)
	{
		//influence de la sant√© du pays
		//calcul de la croissance
		float croissance = 0;
		if(gi > 0)
			croissance = tabSante[gi] - tabSante[gi -1];

		//influence de la croissance sur agents
		if(gi > g_dureeMandat)
			for(i = 0 ; i < g_nombre ; i++)
				influenceSante(tAg[i],croissance,opiPresident,g_maxDerivCroissance, g_impactSante);

		//rencontres
		trand = tabRand(g_nombre);
		
		for(i = 0 ; i < g_nombre -1 ; i+=2)
		{
			rencontre(tAg[trand[i]] , tAg[trand[i+1]], g_assouplissant);
		}

		//affichage Opinion
		for(j = 0; j < 200 ; j++)
		{
			tf = getOpi(tAg[j]);
			if(tAg[j]->dernierVote != opiPresident)
				f_opposition << gi << "\t" << tf << endl ;
			else
				f_pouvoir << gi << "\t" << tf << endl ;
		}

		
		//affichage SantÈ
		float currentSante = (((float)(tabSante[gi]))/santeMax);
		f_sante << currentSante << endl;
		
		
		
		if(gi % g_dureeMandat == 0 && gi > 0)
		{
			printf("# ======= > ELECTION !! Ann√©e %i \n", 2000 + gi/2);
			vector <float> vCand1 = trouverCandidats(tAg, g_division,g_nombre,g_maxOpi);

			printf("#=== > 1er Tour \n");
			vector <float> vCand2 = election(tAg,vCand1,2,g_nombre);

			printf("#=== > 2em Tour \n");
			vector <float> prez = election(tAg,vCand2,1,g_nombre);


			printf("#=== > le nouveau prez est ... !!! ... : candidat %f \n",prez[0]);
			opiPresident = prez[0];
			
			// lancement gnuplot
			//system("gnuplot sante.gp");
			system("gnuplot opi.gp");
			// passage au porchain mandat
			//cout << "== Sante actuelle : " << tabSante[gi] 
				//<< "Sante a la fin du prochain mandat : " << tabSante[gi + g_dureeMandat] << endl;
			//cout << "======== entrez S pour modifier cette derniere valeur ======" << endl;
			cout << "======== entrez C pour continuer ======" << endl;
			
			char continuer;
			//cin >> continuer;
		}
	}

	//////////////////////////////////////////
	////////    Fermeture ////////////////////
	//////////////////////////////////////////
	
	f_opposition.close();
	f_pouvoir.close();
	
	f_sante.close() ;
	
	

	
}

float * genererSante(int tour,float maxderiv)
{
	//On g√©n√®re le tableau de sant√© du pays
	//on trouve sa taille, puisseance de 2 imm√©diatement sup√©rieur au nombre de tours
	int tailleTabSante = 2;
	while(tailleTabSante < tour)
		tailleTabSante*=2;
	//on cherche un valeur finale en accaord avec la d√©riv√©e
	float vMaxSanteFinale = maxderiv * tailleTabSante;
	//
	float vSanteFinale = ((((2*(float)rand()) / RAND_MAX) - 1) * vMaxSanteFinale);
	cout << "============= sante finale " << vSanteFinale << endl << endl << endl;
	//printf("vmaxsante = %f", vSanteFinale);
	float * tabSante = (float*)malloc((tailleTabSante+1)*sizeof(float));

	tabSante[0] = 0;
	tabSante[tailleTabSante] = vSanteFinale;
	
	courbeSante(tabSante, 0 , tailleTabSante, maxderiv);
	
	return tabSante;
}


void influenceSante(Agent * agent, float  croissance, float opiPresident, float maxderiv, float g_impactSante)
{
	//si l'agent a vot√© pour le president actuel
	float newIncert = getInc(agent);
	if(agent->dernierVote == opiPresident)
	{
		newIncert -= (croissance /(0.2+20*pow(agent->ecartVote,2))) / (maxderiv * (10 / g_impactSante)) ;
	}
	//si il fait partie de l'opposition
	else
	{
		float opiVote = agent->dernierVote + agent->ecartVote;
		float ecartPrez = fabs(opiPresident - opiVote);
		float nimpact = ecartPrez / (0.1+fabs(agent->ecartVote)) - 1;
		newIncert += (croissance /(0.2+20*pow(agent->ecartVote,2))) / (maxderiv * (10 / g_impactSante)) ;
	}
	
	if(newIncert > 0 && newIncert < 3)
		addState(agent, getOpi(agent), newIncert);
}

void courbeSante(float * tabSante, int dep, int arri, float maxderiv)
{
	// arri - dep doit √™tre une puissance de 2 !!
	// initialement les extr√©mit√©s du tableau doivent √™tre remplies
	if(arri - dep <= 1)
	{	
		return;
	}

	int inter = dep + (((arri) - dep)/2);
	tabSante[inter] = pointIntermediaire(tabSante[dep] , tabSante[arri] , dep, arri , maxderiv); 

	courbeSante(tabSante,dep,inter,maxderiv);
	courbeSante(tabSante,inter,arri,maxderiv);
}

float pointIntermediaire(float vDepart, float vArrive, float tDepart, float tArrive, float maxderiv)
{
	float max;
	float min;
	float tInter = (tArrive - tDepart) /2;

	float maxFromDep = vDepart + (tInter * maxderiv) ;
	float minFromDep = vDepart - (tInter * maxderiv) ;

	float maxFromArri = vArrive + (tInter * maxderiv) ;
	float minFromArri = vArrive - (tInter * maxderiv) ;
	
	max = fmin(maxFromDep,maxFromArri);
	min = fmax(minFromDep,minFromArri);

	return min + ((float)rand() / RAND_MAX) * (max - min) ;
	
}


vector <float> trouverCandidats(Agent ** tag, int div, unsigned int nombre, float maxopi)
{
	ofstream f_election ;
	f_election.open("election.txt", ios::out | ios::trunc);

	// Le RETOUR de cette fonction sera un vecteur de candidats repr√©sent√©s par leurs opinions
	vector <float> retour;

	//tableau de classes d'opinions
	int tOpi[div];
	int j;
	float lastPop;
	bool descente; //la courbe descend-elle ?
	float curOpi;

	//
	float fPas = (maxopi * 2) / div; 

	//on met le tableau d'opinions √† z√©ro
	for(j = 0; j <div; j++)
		tOpi[j] = 0;

	//on remplit le tableau d'opinions avec les effectifs relatifs √† un intervalle d'opinions
	//cet intervalle est regl√© par la variable globale DIVISION qui indique .le nombre de classes d'opinions
	for(j = 0 ; j < nombre ; j++)
	{
		curOpi = getOpi(tag[j]);
		tOpi[(int)((curOpi + maxopi) / fPas)] += 1;
	}

	//On cherche maintenant les maximums locaux des classes d'opinions 
	lastPop = tOpi[0]; // effectif de la derniËre classe

	if(tOpi == 0)
		descente = false; 
	else
		descente = true;

	for(j = 1 ; j < div ; j++)
	{
		if(tOpi[j] < lastPop && !descente) //si le dernier effectif est plus grand que l'effectif actuel alors que nous Ètions en phase de mont√©e, on a un maximum
		{
			retour.push_back(((j-1) - 0.5)*fPas - maxopi); // on rajoute alors la valeur de l'opinion  moyenne de la derniËre classe au tabeau de retour => on a trouv√© un candidat
			descente = true;
		}
		
		if(tOpi[j] > lastPop && descente) //si le dernier effectif est plus petit que l'actuel => minimum local=>on remonte
		{
			descente = false;
		} 
		
		lastPop = tOpi[j];
	}

	// affichage
	for(int yi = 0; yi < div; yi++)
		f_election << (yi*fPas)- maxopi << "\t" << tOpi[yi] << endl ;

	f_election.close();

	return retour;
	
}


// renvoi les x (nombre d'Èlus desir√©) premiers candidats
vector<float> election(Agent ** tag, vector<float> vCand, int nbElu, int nombre)
{
	

	vector <float> retour;

	//fichier pour affichage gnuplot
	ofstream f_election ;
	if(nbElu == 2)
		f_election.open("election1.txt", ios::out | ios::trunc);
	else
		f_election.open("election2.txt", ios::out | ios::trunc);	

	//on compte les voix pour chaque candidat
	int tUrne[vCand.size()];
	for(int i = 0 ; i < vCand.size() ; i++)
		tUrne[i] = 0;
	
	//on fait dÈfiler les agents
	for(int i=0; i<nombre; i++)
	{
		//on cherche le candidat pour lequel l'Ècart de la valeur qu'il repr√©sente est minimal avec l'opi de l'agent courant
		int indiceVote = vCand[0];
		float plusPetitEcart = getOpi(tag[i]) - vCand[0];
		
		//on fait dÈfiler tous les candidats
		for(int j = 1; j < vCand.size() ; j++)
		{
			//on calcule l'√©cart d'opinions
			float ecart = getOpi(tag[i]) - vCand[j];
			
			//si la valeur de l'√©cart avec le candidat courant est plus petite que le plus petit √©cart trouv√© jusqu'√† present alors :
			if(fabs(ecart) < fabs(plusPetitEcart))
			{
				plusPetitEcart = ecart;
				indiceVote = j;
			}
			
		}

		//on a ici determin√© le vote de l'agent courant
		tUrne[indiceVote]+=1; //le vote de l'agent

		//on met a jour l'agent 
		tag[i]->dernierVote = vCand[indiceVote]; //on indique pour quel candidat il a vot√©.
		//on rappelle qu'un cadidat est design√© par sa valeur d'opinion
		tag[i]->ecartVote = plusPetitEcart; //on sauvegarde √©galement l'√©cart d'opinion avec ce candidat
	}

	//Le depouillement !!
	printf("#Resulats:\n");
	

	//Affichage des rÈsultats
	for(int j = 0; j<vCand.size(); j++)
	{
		printf("#\t candidat %f : %f pourcent des voix\n", vCand[j] , (((float)tUrne[j])/nombre)*100);
		f_election << vCand[j] << "\t" << tUrne[j] <<endl;
	}
	

	//sÈlection des nbElu meilleurs
	for(int i = 0 ; i<nbElu ; i++)
	{
		int elu = 0;
		for(int j = 0 ;  j<vCand.size(); j++)
		{
			if(tUrne[j] > tUrne[elu])
			{
				elu = j;
			}	
		}
		retour.push_back(vCand[elu]);
		tUrne[elu] = 0;
		
	}

	//on retourne l'opinion des candidat √©lus (ce qui est notre moyen de les d√©signer)
	
	return retour;

}


float monMin(float a, float b)
{
	if(a<b)
		return a;
	return b;
}

float monMax(float a, float b)
{
	if(a>b)
		return a;
	return b;
}


int * tabRand(int taille)
{
	
	int i;
	int * tabRetour;
	bool tabVerif[taille];
	int parite;
	int num;

	

	tabRetour = (int*)malloc(taille * sizeof(int));
	
	for(i = 0; i< taille; i ++)
		tabVerif[i] = false;
	
	for(i = 0 ; i < taille; i ++)
	{	
		
		
		num = rand() % taille ;
		
		if(tabVerif[num] == true)
		{
			parite = rand() % 2;
			if(parite == 0)
			{
				do
				{
					num--;
					
					if(num < 0)
						num = taille -1;

				}while(tabVerif[num] == true);
			}
			else
			{
				do
				{
					num++;
					
					if(num >= taille)
						num = 0;

				}while(tabVerif[num] == true);	
			}
			
		}
		tabRetour[i] = num;
		tabVerif[num] = true;
		
	}

	return tabRetour;		

}


Agent ** creeTabAgent(int nombre, float maxopi, float petitPE, float petitU, float grandU)
{
	int i;
	int j;

	Agent ** tab;
	tab = (Agent**)malloc(nombre * sizeof(Agent*));
	
	for(i=0 ; i < nombre ; i++)
	{
		tab[i] = creerAgent( i , distribOpi(nombre , i, maxopi), distribInc(nombre, i, petitPE, petitU, grandU));
	}
	
	return tab;
	
}

float distribOpi(int nb, int num, float maxopi)
{
	float plage = maxopi - (-maxopi);
	
	return (-maxopi) + (plage/nb) * num;
}

float distribInc(int nb,int num,float petitPE, float petitU, float grandU)
{
	if((num < (nb * (petitPE / 2))) || (num > (nb - nb * (petitPE / 2))))
		return petitU;
	else
		return grandU;
}


Agent * creerAgent(int num, float opi, float inc )
{

	Agent * retour = (Agent*)malloc(sizeof(Agent));

	retour->num = num;

	retour->dernierVote = 0;
	
	retour->histoire = creerNoeud(opi,inc);

	return retour;

}

void addState(Agent * ag, float opi, float inc)
{

	if(!ag && !ag->histoire) return;

	//printf("addState(%i,%f,%f)\n",ag->num,opi,inc);

	ajouterFin(creerNoeud(opi,inc), &ag->histoire);

	//knot * ya = dernier(ag->histoire);

	//printf("-> %i , %f, %f \n",ag->num, ya->opi, ya->inc);
	
	return;

}

void voirAgent(Agent * pAgent)
{
	printf("%i \t %f \t %f " , pAgent->num , getOpi(pAgent), getInc(pAgent));
	
}

float getInc(Agent* ag)
{
	knot * derniere;

	if(!ag && !ag->histoire) return -1;

	derniere = dernier(ag->histoire);

	if(!derniere) return -1;  

	return derniere->inc;
}

float getOpi(Agent* ag)
{
	knot * derniere;

	if(!ag && !ag->histoire) return -1;

	derniere = dernier(ag->histoire);

	if(!derniere) return -1;  

	return derniere->opi;
}

float getOpiByI(Agent* ag, int i)
{
	knot * rech;

	if(!ag && !ag->histoire) return -1;

	rech = recherche(i,ag->histoire);

	if(!rech) return -1;

	return rech->opi;
}

float getIncByI(Agent* ag, int i)
{
	knot * rech;

	if(!ag && !ag->histoire) return -1;

	rech = recherche(i,ag->histoire);

	if(!rech) return -1;

	return rech->inc;
}

//fonction de rencontre binaire entre 2 agents
void rencontre(Agent * pAg1,  Agent * pAg2, float assouplissant)
{
	//if(pAg1->num == 166 || pAg2->num == 166)
		//printf("%i -> %i \n" , pAg1->num, pAg2->num);

	float newOpiAg1 = getOpi(pAg1);
	float newIncAg1 = getInc(pAg1);

	float newOpiAg2 = getOpi(pAg2);
	float newIncAg2 = getInc(pAg2);

	float rec = recouvrement(pAg1, pAg2);
	
	if(rec > getInc(pAg2))
	{
		float acc = ( rec / getInc(pAg2)) - 1; //accord relatif
	
		newOpiAg1 +=  assouplissant * acc * (getOpi(pAg2) - getOpi(pAg1));
		
		newIncAg1 +=  assouplissant * acc * (getInc(pAg2) - getInc(pAg1));

	}
	
	if(rec > getInc(pAg1))
	{
		float acc = ( rec / getInc(pAg1)) - 1; //accord relatif
	
		newOpiAg2 +=  assouplissant * acc * (getOpi(pAg1) - getOpi(pAg2));
		
		newIncAg2 +=  assouplissant * acc * (getInc(pAg1) - getInc(pAg2));
		

	}

	/*if(newIncAg2 > 1)
	{
		printf("rencontre ");
		voirAgent(pAg1);
		printf(" - ");
		voirAgent(pAg2);
		
		printf(" rec %f - newAg1(%f , %f) - newAg2(%f , %f) \n",rec,newOpiAg1,newIncAg1,newOpiAg2,newIncAg2);
	}*/
	
	
	addState(pAg1, newOpiAg1, newIncAg1);
	
	addState(pAg2, newOpiAg2, newIncAg2);
}

float recouvrement(Agent* pAg1, Agent* pAg2)
{

	//printf("ag1 %f %f ag2 %f %f ",getOpi(pAg1),getInc(pAg1),getOpi(pAg2),getInc(pAg2));
	//hij = min(xi + ui , xj + uj) - max(xi - ui , xj - uj)

	float a,b;

	a = monMin(getOpi(pAg1) + getInc(pAg1) , getOpi(pAg2) + getInc(pAg2));
	b = monMax(getOpi(pAg1) - getInc(pAg1), getOpi(pAg2) - getInc(pAg2));
	//printf("rec %f \n",a-b);

	return a-b;	
}

