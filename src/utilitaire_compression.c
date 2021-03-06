#include "utilitaire_compression.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "huffman.h"

int profondeur(tree* tree, char symbole, int p) {
  int k;
  if (tree == NULL) {
    return 0;
  }

  // si on a trouvé le charactère
  if (tree->caractere == symbole) {
    return p;
  }
  // sinon on parcours le fils droit pour le trouver
  else if ((k = profondeur(tree->fils_gauche, symbole, p + 1)) != 0) {
    return k;
  }
  // et si il n'est pas dans le fils droit on parcours le fils gauche
  else {
    return profondeur(tree->fils_droite, symbole, p + 1);
  }
}
/* JULIETTE */
/**
 * encoder_symbole : renvoit le code d'un symbole (char ascii) dans un arbre
 *quelconque On renvoit une chaine de caractère. Le code du symbole est placé
 *dans l'ordre Le pointeur sur entier lg contient la longueur (en nombre de bit)
 *du symbole codé. Attention : Le resultat peut donc etre plus court qu'un
 *octet. Convention : le poid d'un fils_gauche est mis à 0 et celui d'un
 *fils_droite à 1.
 **/
char* encoder_symbole(tree* tree, char symbole, int* lg) {
  noeud* n;
  char* res = malloc(32);

  int i = 0;
  int j = 0;

  int prof = 0;

  // on initialise le tableau
  for (int k = 0; k < 32; k++) {
    res[k] = 0;
  }

  // parcours de l'arbre en recherchant sym
  n = recherche_symbole_arbre(tree, symbole);
  assert(n->caractere == symbole);
  // le nombre de bit necessaire pour ecrire le symbole codé
  prof = profondeur(tree, symbole, 0);
  (*lg) = prof;

  // i indique dans quelle case du tableau on doit commencer à écrire
  i = (prof) / 8 - 1;
  if (prof % 8 != 0) i++;
  // on indique a partir de quel rang on doit ecrire les bits
  j = (7 - ((prof % 8) - 1)) % 8;
  // ecriture du code de sym
  // tant qu'on ne pointe pas sur la tete, c'est à dire qu'on a fini de remonter
  // l'arbre
  while (n != tree) {
    assert(i >= 0);
    assert(j >= 0);
    assert(j < 8);
    // si la branche vaut 1 (fils droit)
    if (n == n->pere->fils_droite) {
      res[i] += 1 << j;  // on ecrit 1 (bit) décallé en fonction de la
                         // profondeur
    }

    // si la branche vaut 0  (fils gauche) decalage inutile

    // on incremente
    n = n->pere;
    // longueur du code du symbole restant à ecrire
    j++;

    // si on commence un nouvel octet
    if (j == 8) {
      i--;
      j = 0;
    }
  }

  return res;
}

/**
 * Parcours d'un arbre en recherchant un symbole (char ascii)
 * fonction recursive
 * renvoit NULL si le symbole n'est pas trouvé
 * renvoit le noeud correspondant au symbole
 **/
noeud* recherche_symbole_arbre(tree* tree, char symbole) {
  noeud* n;

  // si on arrive en fin d'arbre
  if (tree == NULL) {
    return NULL;
  }

  // si on a trouvé le charactère
  if (tree->caractere == symbole) {
    return tree;
  }
  // sinon on parcours le fils droit pour le trouver
  else if ((n = recherche_symbole_arbre(tree->fils_droite, symbole)) != NULL) {
    return n;
  }
  // et si il n'est pas dans le fils droit on parcours le fils gauche
  else {
    return recherche_symbole_arbre(tree->fils_gauche, symbole);
  }
}

/* ARIANE */

void parcours_arbre(canonical_tree* tree, char* tableau, int profondeur) {
  if (tree == NULL) {
    return;
  } else {
    if (tree->caractere != -1) {
      tableau[tree->caractere] = profondeur;
    }
    parcours_arbre(tree->fils_gauche, tableau, profondeur + 1);
    parcours_arbre(tree->fils_droite, tableau, profondeur + 1);
  }
}

char* tree_to_length_table(canonical_tree* tree) {
  char* table = malloc(sizeof(char) * 256);
  for (int i = 0; i < 256; i++) {
    table[i] = 0;
  }
  parcours_arbre(tree, table, 0);
  return table;
}

/* QUENTIN */

void tricroissant(int** tab, int tab_size) {
  int i = 0;
  int tmp0 = 0;
  int tmp1 = 0;
  int j = 0;

  for (i = 0; i < tab_size; i++)  // On veut remplir la case i du tableau
  {
    for (j = i + 1; j < tab_size;
         j++)  // On vérifie s'il n'y a pas de nombre inférieur
    {          // Dans les cases suivantes
      if (tab[j][1] < tab[i][1]) {
        tmp0 = tab[i][0];  // Si c'est le cas on intervertit les cases
        tab[i][0] = tab[j][0];
        tab[j][0] = tmp0;

        tmp1 = tab[i][1];  // Si c'est le cas on intervertit les cases
        tab[i][1] = tab[j][1];
        tab[j][1] = tmp1;
      }
    }
  }
}

noeud** tableau_noeud(int** tabCara, int nbCara) {
  int i;
  noeud** tabNoeud = malloc(nbCara * sizeof(noeud*));

  for (i = 0; i < nbCara; i++) {
    noeud* temp = malloc(sizeof(noeud));
    temp->poid = tabCara[i][1];
    temp->caractere = tabCara[i][0];
    temp->fils_droite = NULL;
    temp->fils_gauche = NULL;
    temp->pere = NULL;
    tabNoeud[i] = temp;
  }

  return tabNoeud;
}

void tricroissantNoeud(noeud** tab, int tab_size) {
  int i = 0;
  noeud* tmp;
  int j = 0;

  for (i = 0; i < tab_size; i++)  // On veut remplir la case i du tableau
  {
    for (j = i + 1; j < tab_size;
         j++)  // On vérifie s'il n'y a pas de nombre inférieur
    {          // Dans les cases suivantes
      if (tab[j]->poid < tab[i]->poid) {
        tmp = tab[i];  // Si c'est le cas on intervertit les cases
        tab[i] = tab[j];
        tab[j] = tmp;
      }
    }
  }
}

void afficher_arbre(noeud* tete, int niveau) {
  int i;
  fflush(stdin);
  if (tete != NULL) {
    afficher_arbre(tete->fils_droite, niveau + 1);
    for (i = 0; i < niveau; i++) {
      printf("\t");
    }
    if (tete->caractere != -1) {
      printf(" %d (%c)\n\n", tete->poid, tete->caractere);
    } else {
      printf(" %d (NULL)\n\n", tete->poid);
    }

    afficher_arbre(tete->fils_gauche, niveau + 1);
  }
  return;
}

/* RAPHAEL */

/* NICOLAS */

/* Ergi */

void construction_par_niveau(huffman_tree* tree, int level, int longueur,
 int* p_indice, tableau_constructif* tab) {
  if (tree == NULL) return;

  if (level == 0) {
    if (tree->fils_gauche == NULL && tree->fils_droite == NULL) {
      tab[*p_indice].caractere = tree->caractere;
      tab[*p_indice].longueur = longueur;
      (*p_indice)++;
    }
  }

  else if (level > 0) {
    construction_par_niveau(tree->fils_gauche, level - 1, longueur, p_indice,
      tab);
    construction_par_niveau(tree->fils_droite, level - 1, longueur, p_indice,
      tab);
  }
}

void tri_tableau(tableau_constructif* tab, int nbf) {
  int indice_car_min;
  int temp;
  int inf = 0;
  int sup = 0;
  int longueur;

  tableau_constructif tmp;
  int indice_min;
  for(int i = 0; i<nbf; i++){
    indice_min = i;
    tmp.longueur = tab[i].longueur;
    for(int j=i; j < nbf; j++){
      if(tab[j].longueur < tmp.longueur){
        indice_min =j;
      }
    }
    tmp.longueur = tab[indice_min].longueur;
    tmp.caractere = tab[indice_min].caractere;
    tab[indice_min].longueur = tab[i].longueur;
    tab[indice_min].caractere = tab[i].caractere;
    tab[i].longueur = tmp.longueur;
    tab[i].caractere = tmp.caractere;
  }



  while (sup < nbf) {
    longueur = tab[inf].longueur;
    while (longueur == tab[sup].longueur) sup++;

    while (inf < sup) {
      indice_car_min = inf;
      int i = inf;
      while (i < sup) {
        if (tab[i].caractere < tab[indice_car_min].caractere)
          indice_car_min = i;
        i++;
      }
      temp = tab[inf].caractere;
      tab[inf].caractere = tab[indice_car_min].caractere;
      tab[indice_car_min].caractere = temp;
      inf++;
    }
  }
}

int traitement_caractere(int* cmp, int lg, char* octet, char* buffer,
 FILE* dst) {
  char masque = 1;
  if (*cmp + lg < 8) {
    for (int i = 0; i < lg - 1; i++) {
      masque = (masque * 2) + 1;
    }
    *buffer = (*buffer >> (8 - lg)) & masque;
    *octet = (*octet << lg) | *buffer;
    *cmp = *cmp + lg;
  } else if (*cmp + lg == 8) {
    for (int i = 0; i < 8 - *cmp - 1; i++) {
      masque = (masque * 2) + 1;
    }
    *buffer = (*buffer >> *cmp) & masque;
    *octet = (*octet << lg) | *buffer;
    ecrire_octet(dst, *octet);
    *octet = 0;
    *cmp = 0;
  } else {
    char temp = *buffer;
    for (int i = 0; i < (8 - *cmp - 1); i++) {
      masque = (masque * 2) + 1;
    }
    temp = (temp >> *cmp) & masque;
    *octet = (*octet << (8 - *cmp)) | temp;
    ecrire_octet(dst, *octet);
    *octet = 0;
    *octet = (*buffer >> (8 - lg));
    *cmp = lg - (8 - *cmp);
    masque = 1;
    for (int i = 0; i < *cmp; i++) {
      masque = (masque * 2) + 1;
    }
    *octet = *octet & masque;
  }
  return lg;
}
