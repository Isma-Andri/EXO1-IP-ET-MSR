# Connexion Utilisateur avec Vérification d'IP

Ce simple programme en C utilise la bibliothèque `ncurses` pour créer une interface en ligne de commande où les utilisateurs peuvent se connecter, s'inscrire et vérifier leur adresse IP.

## Fonctionnalités

- **Menu principal** : Options: se connecter, s'inscrire ou quitter le programme.
- **Connexion** : Si l'utilisateur est deja inscrit.
- **Inscription** : Permet d'ajouter un nouvel utilisateur sauvegardé dans un fichier.
- **Vérification d'IP** : Valide et classe les adresses IP entrées.

## Fichiers

- `main.c` : Contient la fonction principale.
- `utils.c` : Implémentation des fonctions utils.
- `utils.h` : Déclare les fonctions utils.

## Compilation

Pour le compiler, voici la commande à utiliser :

```bash
gcc -o ip main.c utils.c -lncurses
```

## Execution

Après la compilation, taper cette commande pour l'executer:

```bash
./ip
```
