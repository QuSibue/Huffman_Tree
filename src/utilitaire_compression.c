#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"



/**
* lire_symbole : lit un charactère (8bits) dans le fichier source
* renvoit un int, pour prendre en compte la valeur EOF (-1)
* Aucun traitement sur l'entrée lue
**/
int lire_symbole(FILE* f){
	char c;
	if (fscanf(f, "%c", &c)){
		return (int)c;
	}
	else return -1;
}

/**
* encoder_symbole : renvoit le code d'un symbole (char ascii) dans un arbre quelconque
* On renvoit un octet (char).
* Le code du symbole est placé dans les bits de poids faible.
* Le pointeur sur entier lg contient la longueur (en nombre de bit) du symbole codé. 
* Attention : Le resultat peut donc etre plus court qu'un octet.
* Convention : le poid d'un fils_gauche est mis à 0 et celui d'un fils_droit à 1. 
**/
char* encoder_symbole(tree* tree, char symbole, int* lg){
	noeud *n;
	char res[32];
	int i=0;
	int j=0;
	
	*lg =0;

	//on initialise le tableau
	for(int k=0; k<32; k++){
		res[k]=0;
	}

	//parcours de l'arbre en recherchant sym
	n=recherche_symbole_arbre(tree,symbole);

	//ecriture du code de sym
	//tant qu'on ne pointe pas sur la tete, c'est à dire qu'on a fini de remonter l'arbre
	while ( n != tree ){
		//si la branche vaut 1 (fils droit)
		if (n == n->parent->fils_droit)
			res[i] +=  1 << j; //on ecrit 1 (bit) décallé en fonction de la profondeur 
		//si la branche vaut 0  (fils gauche) decalage inutile

		//on incremente
		n = n->parent;
		(*lg)++;
		j++;
		//si on commence un nouvel octet
		if((*lg) != 0 && (*lg)%8 == 0){
			i++;
			j=0;
		}
	}

	return (char*)res;

}

/**
* Parcours d'un arbre en recherchant un symbole (char ascii)
* fonction recursive
renvoit NULL si le symbole n'est pas trouvé
* renvoit le noeud correspondant au symbole
**/
noeud* recherche_symbole_arbre(tree* tree, char symbole){
	noeud* n;

	//si on arrive en fin d'arbre
	if(tree == NULL){
		return NULL;
	}

	//si on a trouvé le charactère
	if(tree->caractere == symbole){
		return tree;
	}
	//sinon on parcours le fils droit pour le trouver
	else if(n = recherche_symbole_arbre(tree->fils_droit, symbole) != NULL){
		return n;
	}
	//et si il n'est pas dans le fils droit on parcours le fils gauche
	else return recherche_symbole_arbre(tree->fils_gauche, symbole);

}

/**
* écrit dans un fichier un octet codé stocké dans chaine
**/ 
void ecrire_octet(FILE* fichier_ecriture, char chaine){
	fprintf(fichier_ecriture, "%c", chaine);
}
