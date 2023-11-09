#pragma once
#pragma once
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <map>

const int nmax = 30;                       //le nombre de pieces maximal
const int mmax = 20;					   //le nombre de machines maximal		
const int C_infini = 9999;
const int nb_itr_max = 100;
const int nb_voisin_max = 10;
const int limite_hachage = 99999;

using namespace std;

typedef struct T_instance {
	int n;
	int m;
	int machine[nmax][mmax];
	int p[nmax][mmax]; //duree 
}T_instance;

typedef struct T_vecteur {
	int v[nmax * mmax];						//vecteur de Bierwith
	int st[nmax + 1][mmax + 1];				//les dates de debut ( starting time )
	int cout;								//le cout de cette solution
	int pere[nmax * mmax + 1][2];
}T_vecteur;

void lecture(string nom, T_instance& nom_instance);
void generer_vecteur_aleatoire(T_instance& nom_instance, T_vecteur& nom_vecteur);
void evaluer(T_instance& nom_instance, T_vecteur& nom_vecteur);
void recherche_locale(T_instance& nom_instance, T_vecteur& nom_vecteur, int nbmax_itr);
void permutation(T_vecteur& nom_vecteur, int i, int j);
void Grasp(T_instance& nom_instance, T_vecteur& vecteur, int nbmax_itr);
bool tester_double(T_vecteur& nom_vecteur, T_instance nom_instance);// utile pour la verification si on a deja passé par ce vecteur de Bierwith
