# chevalDeTroieSimple
Présentation d'un cas simple d'infection simple : un petit cheval de Troie

## Pourquoi ? Présentation des infections simples
J’aimerais vous présenter le cas des infections simples.
Ces programmes s’installent simplement dans le système.
L’installation se fait généralement et simultanément :
- en mode résident : le processus est actif en mémoire de façon permanente ;
- en mode furtif : par exemple le processus n’est pas visible lors de l’affichage des processus en cours (à l’aide des commandes ps -aux ou Ctrl+Alt+Suppr) ;
- en mode persistant : en cas d’effacement ou désinstallation, le programme est capable de se réinstaller indépendamment d’un dropper.

## Exemple
Sous Windows, plusieurs copies de ce programme sont cachées dans les répertoires systèmes et une ou plusieurs clefs sont ajoutées dans la base de registres par le programme lors de l’installation initiale.
C’est le cas du cheval de Troie "Back Orifice 2000" qui ajoute dans la base de registres la clef contenant le nom du fichier infecté, à savoir HKLM\Software\Microsoft\Windows\CurrentVersion\RunServices.
De cette manière à chaque démarrage de la machine, le module serveur est ainsi réactivé.

## Les bombes logiques
Une bombe logique est un programme infectant simple, s’installant dans le système informatique cible et qui attend un événement (une date système, une action, des données particulières, …) appelé en général "gâchette", afin d’exécuter sa fonction offensive.
Les anti-virus détectent mal ces bombes logiques (en effet tant qu’elles n’ont pas été activées, leur signature est inconnue).
Ce sont en apparence de simples programmes. Mais les techniques évoluées de lutte antivirale (comme l’analyse heuristique, l’émulation de code) sont condamnées à être constamment mises en défaut, face à des bombes logiques inconnues.

## Les chevaux de Troie
Un cheval de Troie est un programme simple, composé de deux parties, un module serveur et un module client, un peu comme un RAT (Remote Access Tool). Le module serveur, installé dans l’ordinateur de la victime, donne discrètement à l’attaquant accès à tout ou partie de ses ressources, qui en dispose via le réseau (en général), grâce à un module client. 
Le module serveur est un programme dissimulé dans un autre programme anodin et attractif. Une seule exécution de ce programme suffit à installer le module serveur sur la machine de la victime.
Le module client, une fois installé dans la machine de l’attaquant recherche sur le réseau, à l’aide de ping, les machines ayant été infectées par le module serveur puis en prend le contrôle, lorsqu’il a obtenu en retour l’adresse IP et le port (TCP ou UDP) des machines accessibles.

Remarque : 
Ces logiciels sont détectés de manière inégale comme les bombes logiques.
C’est là qu’interviennent les pare-feux pour le filtrage des ports.

Les leurres (fausses bannière de connexion) ainsi que les espions de claviers (keyloggers) ne sont que des cas particuliers de chevaux de Troie.

L’action "offensive" d’un cheval de Troie consiste, pratiquement toujours, à récupérer une ou plusieurs informations et s’effectue passivement par analyse (le sniffing) des paquets IP transitant par le réseau ou envoi à des adresses fixées.

## Explications de mon code : 
C'est en quelque sorte du reverse TCP.
La partie serveur est codée en C, une fois compilée, on a un binaire pour Windows (Pour faire les tests, on pourra créer une machine virtuelle Windows 10 par exemple, nommée "victime", sur laquelle sera installée la partie serveur).
La partie cliente est codée en python (Pour faire les tests, on pourra créer une machine virtuelle Linux, deb ou rpm, nommée "attaquant", sur laquelle on exécutera notre script python).
L'interface utilise des sockets.


