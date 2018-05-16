#include "utilitaire_compression.h"
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

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
  if (tete != NULL) {
    afficher_arbre(tete->fils_droite, niveau + 1);
    for (i = 0; i < niveau; i++) {
      printf("\t");
    }
    if (tete->caractere != -1) {
      printf("%d (%c)\n\n", (int)tete->poid, tete->caractere);
    } else {
      printf("%d (NULL)\n\n", (int)tete->poid);
    }

    afficher_arbre(tete->fils_gauche, niveau + 1);
  }
  return;
}

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

  parcours_arbre(tree, table, 1);

  return table;
}

char lire_symbole(FILE* f) {
  char c;
  fscanf(f, "%c", &c);
  return c;
}

void ecrire_symbole(FILE* f, char c) { fprintf(f, "%c", c); }

char encoder_symbole(canonical_tree* tree, char symbole, int* lg) {
  noeud* n;
  int res=0;
  *lg = 0;

  // parcours de l'arbre en recherchant sym
  n = recherche_symbole_arbre(tree, symbole);

  // ecriture du code de sym
  // tant qu'on ne pointe pas sur la tete, c'est à dire qu'on a fini de remonter
  // l'arbre
  while (n != tree) {
    // si la branche vaut 1 (fils droit)
    if (n == n->pere->fils_droite)
      res +=
          1 << (*lg);  // on ecrit 1 (bit) décallé en fonction de la profondeur
    // si la branche vaut 0  (fils gauche)
    else
      res +=
          0 << (*lg);  // on ecrit 0 (bit) décallé en fonction de la profondeur

    // on incremente
    n = n->pere;
    (*lg)++;
  }

  return (char)res;
}

noeud* recherche_symbole_arbre(canonical_tree* tree, char symbole) {
  // si on arrive en fin d'arbre
  if (tree == NULL) {
    return NULL;
  }

  // si on a trouvé le charactère
  if (tree->caractere == symbole) {
    return tree;
  }
  // sinon on parcours le fils droit pour le trouver
  else if (recherche_symbole_arbre(tree->fils_droite, symbole) != NULL) {
    return recherche_symbole_arbre(tree->fils_droite, symbole);
  }
  // et si il n'est pas dans le fils droit on parcours le fils gauche
  else
    return recherche_symbole_arbre(tree->fils_gauche, symbole);
}