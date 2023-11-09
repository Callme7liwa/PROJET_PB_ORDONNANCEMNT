#include "JobShop.h";


map<int, int> couts;

void lecture(string nom_fichier, T_instance& nom_instance) {
	ifstream fichier(nom_fichier, ifstream::in);
	if (fichier) {
		fichier >> nom_instance.n;
		fichier >> nom_instance.m;
		for (int i = 0; i < nom_instance.n; i++) {
			for (int j = 0; j < nom_instance.m; j++) {
				fichier >> nom_instance.machine[i][j];
				fichier >> nom_instance.p[i][j];
			}
		}
		fichier.close();
	}
	else {
		cout << "Impossible d'ouvrir le fichier : " << nom_fichier << endl;
	}
}

void generer_vecteur_aleatoire(T_instance& nom_instance, T_vecteur& nom_vecteur) {
	int p[nmax];   //contient les numéros des pièces
	int occ[nmax];   //n[i] represente le nombre de fois que la piece i doit encore apparaitre
	//initialisation de v et n
	for (int i = 0; i < nom_instance.n; i++) {
		p[i] = i;
		occ[i] = nom_instance.m;
	}
	int nbPiece = nom_instance.n; // le nombre de pièce qui doit encore apparaitre
	for (int j = 0; j < nom_instance.m * nom_instance.n; j++) {
		int aleatoire = rand() % nbPiece;   //generer aléatoirement un numero de piece
		nom_vecteur.v[j] = p[aleatoire];    //ajouter cette piece dans le vesteur de bierwith
		occ[aleatoire]--;
		if (occ[aleatoire] == 0) {          //si la piece apparait m fois
			p[aleatoire] = p[nbPiece - 1];  //supprimer cette piece de p et son occurence de occ
			occ[aleatoire] = occ[nbPiece - 1];
			nbPiece--;
		}
	}
}

void evaluer(T_instance& nom_instance, T_vecteur& nom_vecteur) {
	// np[i] signifie combien de fois je suis passé par la pièce i
	int np[nmax];
	for (int i = 0; i < nom_instance.n; i++) {
		np[i] = 0;
	}
	//mp[i] signifie la dernière pièce traité sur cette machine et son rang
	int mp[nmax][2];
	for (int i = 0; i < nom_instance.m; i++) {
		mp[i][0] = -1;
		mp[i][1] = -1;
	}
	//intiatiation des dates de début
	for (int i = 0; i < nom_instance.n; i++) {
		for (int j = 0; j < nom_instance.m; j++) {
			nom_vecteur.st[i][j] = -C_infini;
		}
	}
	nom_vecteur.st[0][0] = 0;
	nom_vecteur.st[1][0] = 0;
	nom_vecteur.st[2][0] = 0;
	//initialiser les peres
	for (int i = 0; i < nom_instance.n * nom_instance.m + 1; i++) {
		nom_vecteur.pere[i][0] = -1;
		nom_vecteur.pere[i][1] = -1;
	}
	//initialiser le temps total de production
	nom_vecteur.cout = 0;
	int debut_piece, fin_piece, pc, rc;
	for (int i = 0; i < nom_instance.n * nom_instance.m; i++) {
		int j = nom_vecteur.v[i];
		np[j]++; //nous avons visité enconre une fois cette pièce
		//arc horizontal
		if (np[j] >= 1) {
			debut_piece = nom_vecteur.st[j][np[j] - 1];                 //date de début de cette operation
			fin_piece = debut_piece + nom_instance.p[j][np[j] - 1];     //date de fin de cette operation
			if (fin_piece > nom_vecteur.st[j][np[j]]) {	              //si la date de fin de cette operation > date de debut de l'operation suivante
				if (np[j] < nom_instance.m) {						  //on marque l'operation qui precede cette operation	
					nom_vecteur.pere[j * (nom_instance.n) + (np[j]) % (nom_instance.m)][0] = j;
					nom_vecteur.pere[j * (nom_instance.n) + (np[j]) % (nom_instance.m)][1] = np[j] - 1;
				}
				nom_vecteur.st[j][np[j]] = fin_piece;                 //on actualise la date de début de l'opération suivante
				if (nom_vecteur.st[j][np[j]] > nom_vecteur.cout) {
					nom_vecteur.cout = nom_vecteur.st[j][np[j]];       //actualiser le temps total de production
					if (np[j] == nom_instance.m) {                    //marquer la dernière opération effectué
						nom_vecteur.pere[nom_instance.n * nom_instance.m][0] = j;
						nom_vecteur.pere[nom_instance.n * nom_instance.m][1] = np[j] - 1;
					}
				}
			}
		}
		//arc diagonal
		int m = nom_instance.machine[j][np[j] - 1];					//la machine ou s'execute cette operation
		if (mp[m][0] != -1 and mp[m][1] != -1) {					//on verifie qu'une pièce a passé sur cette machine
			pc = mp[m][0];
			rc = mp[m][1];
			if (nom_vecteur.st[pc][rc] + nom_instance.p[pc][rc] > nom_vecteur.st[j][np[j] - 1]) {

				nom_vecteur.st[j][np[j] - 1] = nom_vecteur.st[pc][rc] + nom_instance.p[pc][rc];
				nom_vecteur.st[j][np[j]] = nom_vecteur.st[j][np[j] - 1] + nom_instance.p[j][np[j] - 1];
				if (np[j] <= nom_instance.m) {
					nom_vecteur.pere[j * (nom_instance.n) + (np[j] - 1) % (nom_instance.m)][0] = pc;
					nom_vecteur.pere[j * (nom_instance.n) + (np[j] - 1) % (nom_instance.m)][1] = rc;
				}
				// nom_vecteur.st[j][np[j]] = nom_vecteur.st[j][np[j-1]] + nom_insance.p[j,np[j]-1]
				if (nom_vecteur.st[j][np[j]] > nom_vecteur.cout) {
					nom_vecteur.cout = nom_vecteur.st[j][np[j]];
				}
			}
			if (np[j] == nom_instance.m and nom_vecteur.st[j][np[j]] + nom_instance.p[j][np[j]] > nom_vecteur.cout) {
				nom_vecteur.pere[nom_instance.n * nom_instance.m][0] = pc;
				nom_vecteur.pere[nom_instance.n * nom_instance.m][1] = rc;
			}
		}
		mp[nom_instance.machine[j][np[j] - 1]][0] = j;           //marquer la derniere piece qui a passe par cette machine
		mp[nom_instance.machine[j][np[j] - 1]][1] = np[j] - 1;
	}
}

void recherche_locale(T_instance& nom_instance, T_vecteur& nom_vecteur, int nbmax_itr) {
	int iter = 0, r;
	evaluer(nom_instance, nom_vecteur);
	if (!tester_double(nom_vecteur, nom_instance)) {
		int iprec = nom_vecteur.pere[nom_instance.n * nom_instance.m][0];
		int jprec = nom_vecteur.pere[nom_instance.n * nom_instance.m][1];
		int position = -1;//la piece que s'execute
		while (iprec != -1 and jprec != -1 and iter < nbmax_itr) {
			int i = iprec * nom_instance.n + jprec % nom_instance.m;
			//charcher la position de cette operation dans le vecteur de bierwith
			int occ = -1;
			for (int h = 0; h < nom_instance.n * nom_instance.m; h++) {
				if (nom_vecteur.v[h] == iprec) {
					occ++;
					if (occ == jprec) {
						r = h;
					}
				}
			}
			//si cest un arc diagonal
			if (nom_vecteur.v[r] != nom_vecteur.v[position] and position != -1) {
				T_vecteur nom_vecteursuivant = nom_vecteur;
				permutation(nom_vecteursuivant, position, r);
				if (!tester_double(nom_vecteur, nom_instance)) {
					evaluer(nom_instance, nom_vecteursuivant);
					if (nom_vecteur.cout > nom_vecteursuivant.cout) {
						nom_vecteur = nom_vecteursuivant;
					}
				}
			}
			iprec = nom_vecteur.pere[i][0];
			jprec = nom_vecteur.pere[i][1];
			position = r;
			iter++;
		}
	}
}

void permutation(T_vecteur& nom_vecteur, int i, int j) {
	int inter = nom_vecteur.v[i];
	nom_vecteur.v[i] = nom_vecteur.v[j];
	nom_vecteur.v[j] = inter;
}

void Grasp(T_instance& nom_instance, T_vecteur& vecteur, int nbmax_itr) {
	int o1, o2;
	T_vecteur mon_vecteur, meuilleur_vecteur, voisin;
	for (int i = 0; i < nbmax_itr; i++) {
		generer_vecteur_aleatoire(nom_instance, mon_vecteur);
		vecteur = mon_vecteur;
		meuilleur_vecteur.cout = 999999;
		//generer les voisins
		for (int j = 0; j < nb_voisin_max; j++) {
			voisin = mon_vecteur;
			do {
				o1 = rand() % (nom_instance.n * nom_instance.m);
				o2 = rand() % (nom_instance.n * nom_instance.m);
			} while (o1 == o2); //pour s'assurer que les sommets sont différents
			permutation(voisin, o1, o2);
			evaluer(nom_instance, voisin);
			recherche_locale(nom_instance, voisin, nb_itr_max);
			if (voisin.cout < meuilleur_vecteur.cout) {
				meuilleur_vecteur = voisin;
			}
			mon_vecteur = meuilleur_vecteur;
		}
	}
	cout << "- Le cout d'apres GRASP est : " << mon_vecteur.cout;
}

bool tester_double(T_vecteur& nom_vecteur, T_instance nom_instance) {
	//calculer la valeur du fonction du hashage 
	int hash = 0;
	for (int i = 0; i < nom_instance.n; i++) {
		for (int j = 0; j < nom_instance.m; j++) {
			hash += nom_vecteur.st[i][j] * nom_vecteur.st[i][j];
		}
	}
	auto result = couts.find(hash % limite_hachage);
	if (result != couts.end()) {
		return true;
	}
	else {
		couts[hash % limite_hachage] = 1;
		return false;
	}
}

int main() {
	T_instance mon_instance;
	cout << "******* TP realisé par SEDDIKI AYOUB && MOUAD KABOURI ******* " <<endl;
	cout << endl << "---------------------------------------" << endl;
	for (int i = 1; i <= 10; i++) {
		clock_t start = clock();
		lecture("la" + to_string(i) + ".txt", mon_instance);
		T_vecteur mon_vecteur;
		cout << "-> Instance " << i << " : " <<endl;
		Grasp(mon_instance, mon_vecteur, nb_itr_max);
		clock_t end = clock();
		double time_taken = double(end - start) / CLOCKS_PER_SEC;
		cout << endl << "- Temps d'execution : " << fixed << time_taken << " sec";
		cout << endl << "---------------------------------------" << endl;
	}
	return 0;
}

