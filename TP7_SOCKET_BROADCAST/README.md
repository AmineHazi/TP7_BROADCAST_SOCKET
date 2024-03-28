# TP N°7 : Communication UDP avec Diffusion

Ce projet illustre une application simple de communication en C utilisant des sockets UDP pour la diffusion de messages sur un réseau local. Il envoie une chaîne de caractères à intervalles réguliers à l'ensemble du réseau sur un port spécifique et écoute les réponses sur le même port. Les adresses IP des clients qui répondent avec le même message sont enregistrées, et une liste de ces adresses est affichée toutes les 30 secondes.

## Commencer

Suivez ces instructions pour configurer et exécuter une copie du projet sur votre système local à des fins de développement et de test.

### Prérequis

Pour compiler et exécuter ce projet, vous aurez besoin de :

- GCC (GNU Compiler Collection)
- GNU Make

### Compilation

Le projet peut être compilé en utilisant GNU Make. Un `Makefile` est fourni à la racine du projet pour faciliter ce processus.

Pour compiler le projet, exécutez la commande suivante à la racine du projet :

```bash
make
``` 

Cela générera les exécutables nécessaires dans le dossier ./bin.

### Exécution

Pour démarrer le programme, exécutez l'exécutable généré avec le message à diffuser comme argument :

```bash
./bin/main <message_à_diffuser>
```

Le programme diffusera le message spécifié à l'ensemble du réseau sur le port UDP 6336 et écoutera les réponses sur le même port.


### Organisation du projet

Le projet est organisé de la manière suivante :

    src/ : Contient les fichiers source .c du projet.
    include/ : Contient les en-têtes .h.
    bin/ : Dossier où les exécutables sont placés après la compilation.
    Makefile : Permet de compiler facilement le projet et de nettoyer les fichiers générés.

### Nettoyage

Pour supprimer les exécutables et les fichiers objets générés lors de la compilation, vous pouvez utiliser la commande suivante :

```bash
make clean
```

### Notes supplémentaires

- Assurez-vous que votre réseau permet la diffusion UDP (255.255.255.255 sur le port 6336).
- Le programme nécessite des privilèges suffisants pour utiliser la diffusion UDP.
- Les réponses des clients doivent être exactement le même message que celui diffusé pour que leur adresse IP soit enregistrée.
