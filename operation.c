#include <stdio.h>

int main() {
    double num1, num2;
    char operation;

    // Demande à l'utilisateur de saisir deux nombres et l'opération souhaitée
    printf("Entrez le premier nombre : ");
    scanf("%lf", &num1);
    printf("Entrez le deuxième nombre : ");
    scanf("%lf", &num2);
    printf("Entrez l'opération (+, -, *, /) : ");
    scanf(" %c", &operation);

    // Effectue l'opération demandée et affiche le résultat
    switch (operation) {
        case '+':
            printf("%.2lf + %.2lf = %.2lf\n", num1, num2, num1 + num2);
            break;
        case '-':
            printf("%.2lf - %.2lf = %.2lf\n", num1, num2, num1 - num2);
            break;
        case '*':
            printf("%.2lf * %.2lf = %.2lf\n", num1, num2, num1 * num2);
            break;
        case '/':
            if (num2 != 0) {
                printf("%.2lf / %.2lf = %.2lf\n", num1, num2, num1 / num2);
            } else {
                printf("Erreur : division par zéro.\n");
            }
            break;
        default:
            printf("Opération non valide.\n");
    }

    return 0;
}
