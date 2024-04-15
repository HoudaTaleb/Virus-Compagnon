#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#define MESSAGE_SIZE 200
// Structure pour stocker les données du jeu
typedef struct {
    int nombreAleatoire;
    int tentative;
    int MAX_TENTATIVES;
    GtkWidget *label_message;
    GtkWidget *entry_nombre; // Ajout de cette variable
} JeuDonnees;

// Fonction appelée lorsque le bouton "Proposer" est cliqué
static void on_button_proposer_clicked(GtkWidget *button, gpointer data) {
    JeuDonnees *jeu_data = (JeuDonnees *)data;

    // Récupération de la saisie de l'utilisateur
    const gchar *saisie = gtk_entry_get_text(GTK_ENTRY(jeu_data->entry_nombre));
    int nombrePropose = atoi(saisie);

    // Vérification de la proposition
    if (nombrePropose < jeu_data->nombreAleatoire) {
        gtk_label_set_markup(GTK_LABEL(jeu_data->label_message), "<span foreground='red'>Trop petit !</span>");
    } else if (nombrePropose > jeu_data->nombreAleatoire) {
        gtk_label_set_markup(GTK_LABEL(jeu_data->label_message), "<span foreground='red'>Trop grand !</span>");
    } else {
        gtk_label_set_markup(GTK_LABEL(jeu_data->label_message), "<span foreground='green'>Bravo, vous avez trouvé le nombre mystère !</span>");
        gtk_widget_set_sensitive(button, FALSE); // Désactiver le bouton après avoir trouvé le nombre
        return;
    }

    // Mise à jour du compteur de tentatives
    jeu_data->tentative++;
    if (jeu_data->tentative >= jeu_data->MAX_TENTATIVES) {
        // Afficher le message de défaite
        char message[MESSAGE_SIZE];
        snprintf(message, MESSAGE_SIZE, "<span foreground='red'>Désolé, vous avez épuisé toutes vos tentatives. Le nombre mystère était : %d</span>", jeu_data->nombreAleatoire);
        gtk_label_set_markup(GTK_LABEL(jeu_data->label_message), message);
        gtk_widget_set_sensitive(button, FALSE); // Désactiver le bouton si toutes les tentatives sont épuisées
    }
}

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Création du générateur de nombres aléatoires
    srand(time(NULL));

    // Création des données du jeu
    JeuDonnees jeu_data;
    jeu_data.nombreAleatoire = rand() % 100 + 1;
    jeu_data.tentative = 0;
    jeu_data.MAX_TENTATIVES = 5;

    // Création de la fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Jeu de devinette");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la mise en page
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Création de l'étiquette pour afficher les messages
    jeu_data.label_message = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(jeu_data.label_message), "<span font_desc='20'>Bienvenue dans le jeu de devinette !</span>");
    gtk_box_pack_start(GTK_BOX(vbox), jeu_data.label_message, TRUE, FALSE, 0);

    // Création de la zone de saisie
    jeu_data.entry_nombre = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), jeu_data.entry_nombre, TRUE, FALSE, 0);

    // Création du bouton "Proposer"
    GtkWidget *button_proposer = gtk_button_new_with_label("Proposer");
    gtk_box_pack_start(GTK_BOX(vbox), button_proposer, TRUE, FALSE, 0);
    g_signal_connect(button_proposer, "clicked", G_CALLBACK(on_button_proposer_clicked), &jeu_data);

    // Affichage de la fenêtre
    gtk_widget_show_all(window);

    // Démarrage de la boucle principale GTK
    gtk_main();

    return 0;
}
