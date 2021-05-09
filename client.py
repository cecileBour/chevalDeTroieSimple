#!/usr/bin/env python
# coding: utf-8
# pour gerer l'encodage des caracteres accentues au travers du tunnel socket

import socket
import sys

# on cree un objet socket
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# informations du serveur
host = ...
port = 666

# on demarre notre connexion socket on ecoute les requetes
clientsocket.bind((host, port))
clientsocket.listen(5)
print ('serveur demarre et ecoute sur le port '+str(port))

# tant que la connexion socket est acive
while 1:
    # recepetion dune requete cliente
    (serversocket, address) = clientsocket.accept()
    print ("nouvelle connexion a partir de : "+str(address))
    print ("envoie de commandes : ")
    # tant que le client est connecte on peut envoyer des commandes
    while 1:
        # envoi de la commande
        r=str( sys.stdin.readline() )
        serversocket.send(r.encode())
        # reception du resultat
        r = serversocket.recv(9999)
        # on reencode en utf-8 avec le flag "errors=ignore" car certains carac
        client_answer = str( r.decode("utf-8", errors="ignore"))
        print(str(client_answer)+"\n")

