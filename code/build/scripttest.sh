#!/bin/bash

echo bonjour !
echo bienvenue dans le script de test Nachos !
echo premier programme testé : putchar

read -p "Appuyez sur une touche pour continuer"

./nachos-userprog -x putchar

echo "nous allons ensuite tester SynchGetChar !"

read -p "Appuyez sur une touche pour continuer" 

./nachos-userprog -x getChar

echo "Re essayons GetChar avec une autre lettre pour être sur..."
read -p "Appuyez sur une touche pour continuer"

./nachos-userprog -x getChar

echo Nous allons maintenant tester SynchPutString !
read -p "Appuyez sur une touche pour continuer"

./nachos-userprog -x putstring

echo Testons maintenant SynchGetString !
read -p "Appuyez sur une touche pour continuer"

./nachos-userprog -x getString

echo Testons avec une autre phrase !
read -p "Appuyez sur une touche pour continuer"

./nachos-userprog -x getString

echo "L'étape suivante consiste à tester PutInt et GetInt !"
read -p "Appuyez sur une touche pour afficher le nombre 623"

./nachos-userprog -x putint

echo "Maintenant getint !"
./nachos-userprog -x getint

echo "Nous avons terminé les tests de l'étape 2, nous allons maintenant parler des threads"
echo "il y a deux possibilités d'exécution, avec ou sans -rs"
echo "nous allons commencer par exécuter le programme makethread avec puis sans -rs"
read -p "Appuyez sur une touche pour continuer"

./nachos-userprog -x makethreads

read -p "Version avec -rs 131"

./nachos-userprog -rs 131 -x makethreads

echo "Mais que se passe-t-il si nous mettons un UserThreadJoin sur le premier processus, puis sur le deuxième, puis sur les deux, en gardant la même option pour -rs"
read -p "Lançons le programme !"

./nachos-userprog -rs 131 -x makethreadsUTJ1

read -p "Seulement sur le thread 2 maintenant !"

./nachos-userprog -rs 131 -x makethreadsUTJ2

read -p "Et terminons sur un UserThreadJoin sur les deux threads !"

./nachos-userprog -rs 131 -x makethreadsUTJ

read -p "Et maintenant que se passe-t-il si on souhaite créer trop de threads?"

./nachos-userprog -x makethreadserror

echo "Nous allons maintenant tester la création de nouveaux processus"
read -p "Appuyez sur une touche pour continuer"

./nachos-userprog -x forkexectest

read -p "Et enfin créons beaucoup de processus !"

./nachos-userprog -x forkExec

read -p "Démonstration terminée !"