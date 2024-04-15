#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// Fonction pour mélanger les lettres d'un mot
void melangerMot(char *mot) {
    int longueur = strlen(mot);
    for (int i = 0; i < longueur - 1; i++) {
        int j = i + rand() % (longueur - i);
        char temp = mot[i];
        mot[i] = mot[j];
        mot[j] = temp;
    }
}

int main() {
    char mots[][20] = {"ordinateur", "programmation", "developpeur", "algorithmes", "intelligence", "informatique"};
    int nombreMots = sizeof(mots) / sizeof(mots[0]);

    srand(time(NULL));
    int indexMot = rand() % nombreMots;
    char motSecret[20];
    strcpy(motSecret, mots[indexMot]);

    melangerMot(motSecret);

    printf("Devinez le mot : %s\n", motSecret);

    char reponse[20];
    printf("Votre réponse : ");
    scanf("%s", reponse);

    // Conversion de la réponse en minuscules pour la comparaison
    for (int i = 0; reponse[i]; i++) {
        reponse[i] = tolower(reponse[i]);
    }

    if (strcmp(reponse, mots[indexMot]) == 0) {
        printf("Bravo ! Vous avez deviné le mot correctement.\n");
    } else {
        printf("Désolé, le mot correct était : %s\n", mots[indexMot]);
    }

    return 0;
}
