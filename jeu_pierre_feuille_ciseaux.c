#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    enum { PIERRE, PAPIER, CISEAUX };
    const char *choix[] = { "Pierre", "Papier", "Ciseaux" };

    srand(time(NULL));

    printf("Choisissez : 0 (Pierre), 1 (Papier), 2 (Ciseaux) : ");
    int choixUtilisateur;
    scanf("%d", &choixUtilisateur);

    if (choixUtilisateur < 0 || choixUtilisateur > 2) {
        printf("Choix invalide.\n");
        return 1;
    }

    int choixOrdinateur = rand() % 3;

    printf("Vous avez choisi %s. L'ordinateur a choisi %s.\n", choix[choixUtilisateur], choix[choixOrdinateur]);

    if (choixUtilisateur == choixOrdinateur) {
        printf("Égalité !\n");
    } else if ((choixUtilisateur == PIERRE && choixOrdinateur == CISEAUX) ||
               (choixUtilisateur == PAPIER && choixOrdinateur == PIERRE) ||
               (choixUtilisateur == CISEAUX && choixOrdinateur == PAPIER)) {
        printf("Vous avez gagné !\n");
    } else {
        printf("L'ordinateur a gagné !\n");
    }

    return 0;
}
