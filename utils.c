#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

// Fonction qui affiche un simple menu pour l'accueil et la connexion 
void mainMenu() {
    int choice;
    char *options[] = {
        "1. Se connecter",
        "2. S'inscrire",
        "3. Quitter"
    };

    while (VRAI) {
        clear();  // Efface l'ecran avc ncurses puis affiche les choix
        printw("Bienvenue !\n");
        for (int i = 0; i < 3; i++) {
            printw("%s\n", options[i]);
        }
        printw("Choisissez une option: ");
        refresh();
		echo();
        scanw("%d", &choice);
		noecho();
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                addUser();
                break;
            case 3:
                return;
            default:
                printw("Option invalide. Appuyez sur une touche pour continuer...\n");
                getch();
                break;
        }
    }
}

// Fonction pour la connexion si un ou plusieurs users sont enregistrees 
void login() {
    char username[STR_MAX_LENGHT], password[STR_MAX_LENGHT];
    char usernameF[STR_MAX_LENGHT], passwordF[STR_MAX_LENGHT];

    clear();
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printw("Aucun utilisateur enregistré ! Veuillez d'abord vous inscrire\n");
        printw("Appuyez sur une touche pour continuer...");
        getch();
        return;
    }

    printw("Nom d'utilisateur: ");
    refresh();
    echo();
    scanw("%s", username);
    noecho();

    printw("Mot de passe: ");
    refresh();
    scanw("%s", password);

    int status = FAUX;
    char buffer[256];

	// Comparaison des identifiants venant de l'user et ceux dans le fichier logindata.txt
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (sscanf(buffer, "%s %s", usernameF, passwordF) == 2) {
            if (strcmp(username, usernameF) == 0 && strcmp(password, passwordF) == 0) {
                status = VRAI;
                break;
            }
        }
    }

    fclose(file);

    if (status == VRAI) {
        printw("Connecté ! Appuyez sur une touche pour vérifier l'IP...\n");
        getch();
        checkIP();
    } else {
        printw("Nom d'utilisateur ou mot de passe incorrect.\n");
        printw("Appuyez sur une touche pour continuer...");
        getch();
    }
}

// Fonction qui ajoute un user
void addUser() {
    char username[STR_MAX_LENGHT], password[STR_MAX_LENGHT];

    clear();
    printw("Nom d'utilisateur: ");
    refresh();
    echo();
    scanw("%s", username);
    noecho();

    printw("Mot de passe: ");
    refresh();
    scanw("%s", password);

    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        printw("Erreur lors de l'ouverture du fichier.\n");
        printw("Appuyez sur une touche pour continuer...");
        getch();
        return;
    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printw("Utilisateur ajouté avec succès ! Appuyez sur une touche pour continuer...\n");
    getch();
}

// Determine la classe d'une adresse IP entrée
char get_class(int ip1) {
    if (ip1 >= 0 && ip1 <= 127) {
        return 'A';
    } else if (ip1 >= 128 && ip1 <= 191) {
        return 'B';
    } else if (ip1 >= 192 && ip1 <= 223) {
        return 'C';
    } else if (ip1 >= 224 && ip1 <= 239) {
        return 'D';
    } else {
        return 'E';
    }
}

// Verifie si l'adresse est valide  ou non
int verify(int ip1, int ip2, int ip3, int ip4, int ipERR) {
    if (ipERR != -1) {
        return 1;
    } else if ((ip1 >= 0 && ip1 <= 255) &&
               (ip2 >= 0 && ip2 <= 255) &&
               (ip3 >= 0 && ip3 <= 255) &&
               (ip4 >= 0 && ip4 <= 255)) {
        return 0; // L'adresse IP est valide
    } else {
        return 1; // L'adresse IP est invalide
    }
}

// Verifie si le masque de sous reseua correspond a un masque de sous reseau par defaut valide
int verifyMSK(int msk1, int msk2, int msk3, int msk4, int mskERR) {
    if (mskERR != -1) {
        return 1;
    } else if ((msk1 == 255) &&
               (msk2 == 0 || msk2 == 255) &&
               (msk3 == 0 || msk3 == 255) &&
               (msk4 == 0)) {
        return 0; // Le netmask est valide
    } else {
        return 1; // le netmasl est invalide
    }
}

// determine la classe par defaut du masque de sr
char getMSKclass (int msk1, int msk2, int msk3) {
	if (msk1 == 255 && msk2 == 0 && msk3 == 0) return 'A';
	else if (msk1 == 255 && msk2 == 255 && msk3 == 0) return 'B';
	else if (msk1 == 255 && msk2 == 255 && msk3 == 255) return 'C';
	else return 'U';
}

// Renvoie l'adresse de reseau par defaut selon la MSR
void getNetAddress(int *msk1, int *msk2, int *msk3, int *msk4,int ip1, int ip2, int ip3, int ip4, char class){
//
	char classMSK = getMSKclass(*msk1 , *msk2, *msk3);
	if (class != classMSK){		// si la classe de l'IP et la classe du MSR ne corresponde pas
		printw("La classe du masque de sous reseau ne correspond pas avec la classe de l'addresse IP.\n");
		switch (class){
			case 'A':
				printw("Suggestion de masque de sous reseau par defaut: 255.0.0.0\n");
				break;
			case 'B':
				printw("Suggestion de masque de sous reseau par defaut: 255.255.0.0\n");
				break;
			case 'C':
				printw("Suggestion de masque de sous reseau par defaut: 255.255.255.0\n");
				break;
			default:
				printw("Fonction non implémentée.\n");
				break;
		}
	}

	*msk1 = ip1;
	switch(class){
		case 'A':
			*msk2 = 0;
			*msk3 = 0;
			*msk4 = 0;
			break;
		case 'B':
			*msk2 = ip2;
			*msk3 = 0;
			*msk4 = 0;
			break;
		case 'C':
			*msk2 = ip2;
			*msk3 = ip3;
			*msk4 = 0;
			break;
		default:
			printw("Fonction non implementée.\n");
			break;
			
	}
	refresh();
}

// renvoi l adresse de diffusion
void getBroadcastAddress(int *tmpmsk1, int *tmpmsk2, int *tmpmsk3, int *tmpmsk4,int ip1, int ip2, int ip3, int ip4,char class) {
	*tmpmsk1 = ip1;
	switch(class){
		case 'A':
			*tmpmsk2 = 255;
			*tmpmsk3 = 255;
			*tmpmsk4 = 255;
			break;
		case 'B':
			*tmpmsk2 = ip2;
			*tmpmsk3 = 255;
			*tmpmsk4 = 255;
			break;
		case 'C':
			*tmpmsk2 = ip2;
			*tmpmsk3 = ip3;
			*tmpmsk4 = 255;
			break;
		default:
			printw("Fonction non implementée.\n");
			break;
			
	}
	refresh();
}

// nombre d adresse IP pouvant etre utilsée
int getAvailableIP(char class){
	switch (class){
		case 'A':
			return 16777214;			// 2^(nb bit hôte) - 2
			break;
		case 'B':
			return 65534;			// 2^(nb bit hôte) - 2
			break;
		case 'C':
			return 254;			// 2^(nb bit hôte) - 2
			break;
		default:
			return 0;
			break;
	}
}

void checkIP() {
    int ip1, ip2, ip3, ip4, ipERR = -1;
    int msk1, msk2, msk3, msk4, mskERR = -1;

    clear();
    printw("Veuillez entrer une adresse IP (IPv4) valide: ");
    refresh();
    echo();
    scanw("%d.%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4, &ipERR);
    
    printw("Veuillez entrer un masque de sous-reseau valide: ");
    refresh();
    scanw("%d.%d.%d.%d.%d", &msk1, &msk2, &msk3, &msk4, &mskERR);
    refresh();
    clear();

	int tmpmsk1 = msk1, tmpmsk2 = msk2, tmpmsk3 = msk3, tmpmsk4 = msk4;		// sauvegarder les octets dans des variabkes temporaires
	
    char class = get_class(ip1);
    int valide = verify(ip1, ip2, ip3, ip4, ipERR);
    int MSKvalide = verifyMSK(msk1, msk2, msk3, msk4, mskERR);

    if (valide == 0 && MSKvalide == 0) {
        printw("Classe de l'adresse IP: %c\n", class);
        
        getNetAddress(&msk1, &msk2, &msk3, &msk4, ip1, ip2, ip3, ip4, class);
        getBroadcastAddress(&tmpmsk1, &tmpmsk2, &tmpmsk3, &tmpmsk4, ip1, ip2, ip3, ip4, class);
        
        int availableIP = getAvailableIP(class);
        
        printw("Adresse reseau: %d.%d.%d.%d\n", msk1, msk2, msk3, msk4);
        printw("Adresse de diffusion: %d.%d.%d.%d\n",tmpmsk1, tmpmsk2, tmpmsk3, tmpmsk4);
        
        if (availableIP != 0){
			printw("Nombre d'adresse IP utilisable: %d\n\n",availableIP);
		} else {
			printw("Nombre d'adresse IP utilisable: inconnue\n\n");
		}
    } 

    else if (valide == 1){
        printw("Addresse IPv4 invalide.\n");
    }
    else if (MSKvalide == 1){
        printw("Masque de sous reseau invalide.\n");
    }
    else {
		printw("Masque de sous reseau et adresse IPv4 invalide.\n");
	}

    printw("Appuyez sur une touche pour retourner au menu principal...");
    getch();
}


