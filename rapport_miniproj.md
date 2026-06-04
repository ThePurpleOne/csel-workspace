# Rapport du mini-projet CSEL : ventilateur simulé pilotable

## Objectif

Le projet simule un système de contrôle de ventilateur sur un Linux embarqué. La « vitesse » du ventilateur est représentée par la fréquence de clignotement d'une LED. Le système fonctionne soit en mode automatique (la fréquence dépend de la température du CPU), soit en mode manuel (la fréquence est choisie par l'utilisateur). L'utilisateur peut interagir avec le système de deux manières : via trois boutons physiques sur la carte, ou via un serveur réseau qui accepte des commandes depuis n'importe quel client. Un écran OLED affiche en permanence l'état courant du système.

L'architecture se compose de trois composants distincts qui communiquent via des interfaces standard : un **module noyau** qui gère le clignotement de la LED et la lecture de la température, un **daemon userspace** qui fait l'intermédiaire entre l'humain (boutons, écran) et le module, et un **client en ligne de commande** qui dialogue avec le daemon par TCP.

## Module noyau

Le module est le seul à toucher directement au matériel : il pilote la sortie GPIO sur laquelle est connectée la LED, et lit la zone thermique du CPU exposée par le sous-système thermique du noyau. Il maintient en mémoire trois informations : le mode courant (auto ou manuel), la fréquence courante en hertz, et la dernière mesure de température en millidegrés Celsius.

La LED clignote grâce à un timer noyau. À chaque expiration, le timer inverse l'état du GPIO puis se réarme pour une demi-période plus tard. Cette demi-période est recalculée depuis la fréquence courante à chaque tick, ce qui permet de changer la fréquence à chaud sans devoir détruire et recréer le timer.

Un deuxième mécanisme, indépendant du premier, est planifié à 1 Hz. Il interroge le sous-système thermique pour récupérer la température et la stocke dans l'état du module. Si le mode courant est automatique, la fréquence est recalculée selon une grille de paliers : 2 Hz tant que la température est inférieure à 35 °C, puis 5 Hz, 10 Hz, et enfin 20 Hz au-delà de 45 °C.

L'interface vers l'extérieur passe entièrement par sysfs. Le module crée un répertoire dans `/sys/class/` contenant trois fichiers : un pour le mode, un pour la fréquence, un pour la température (en lecture seule). Lire un de ces fichiers retourne la valeur courante ; écrire dans le mode accepte les chaînes « auto » ou « manual » ; écrire dans la fréquence accepte un entier entre 1 et 20. Les valeurs hors plage ou mal formées sont rejetées avec une erreur que le shell rapporte. Cette interface est volontairement triviale pour qu'on puisse l'utiliser avec `cat` et `echo` directement, sans client dédié.

Lors du déchargement, le timer et la tâche périodique sont arrêtés proprement, la LED est éteinte, le GPIO est libéré et les fichiers sysfs sont retirés. L'ordre des opérations est important : si on libérait le GPIO avant d'arrêter le timer, le timer pourrait essayer d'écrire sur un GPIO déjà rendu et provoquer un crash du noyau.

## Daemon userspace

Le daemon n'a aucun lien direct avec le matériel. Il passe par les interfaces sysfs offertes par le noyau pour les boutons (GPIO exportés en userspace) et par l'I2C standard pour l'écran OLED. Son rôle principal est de coordonner toutes les sources d'événements du système (boutons, réseau, rafraîchissement de l'écran) dans une seule boucle d'exécution.

Toute son activité est articulée autour d'une boucle d'événements unique. Cette boucle surveille en parallèle : les trois boutons (qui notifient quand leur état change), la socket TCP en écoute pour de nouvelles connexions, les sockets des clients déjà connectés (quand ils envoient des données), et un timer qui se déclenche périodiquement pour rafraîchir l'écran. Le mécanisme d'attente fourni par le noyau dort jusqu'à ce qu'une de ces sources soit prête, ce qui économise du CPU sans nécessiter de threads.

Quand un bouton change d'état, le daemon lit la nouvelle valeur, met à jour son état interne, puis recalcule la LED « Power ». Cette LED suit une logique simple : un état de repos par défaut, inversé tant qu'au moins un bouton est pressé. Cela donne à l'utilisateur un retour visuel immédiat sur ses appuis. Quand un appui correspond à une vraie transition (relâché vers pressé), le daemon agit : S1 incrémente la fréquence et force le mode manuel, S2 décrémente la fréquence et force le mode manuel, S3 bascule entre auto et manuel. Tous ces changements sont propagés au module en écrivant dans son sysfs.

Le rafraîchissement de l'écran OLED suit un timer périodique réglé à 500 ms. À chaque tick, le daemon lit l'état complet du module (mode, fréquence, température) et redessine les trois lignes correspondantes. Comme l'écran n'a pas besoin d'être mis à jour si rien n'a changé, le daemon garde en mémoire la dernière vue rendue et court-circuite l'écriture I2C si la valeur lue est identique. Cela évite à la fois le scintillement et le trafic I2C inutile.

Le serveur TCP écoute sur le port 1337 et accepte plusieurs clients simultanément. Le protocole est volontairement très simple : chaque ligne envoyée est une commande, et la réponse tient sur une ligne. Les commandes disponibles permettent de lire l'état complet, de changer le mode, de changer la fréquence, ou de fermer la connexion. La validation est doublée côté daemon et côté module, le daemon refuse les valeurs hors plage avant même de toucher au sysfs, ce qui donne un message d'erreur plus parlant au client.

Un point important côté réseau : TCP ne respecte aucune frontière de message. Une commande envoyée en un bloc par le client peut très bien arriver en plusieurs paquets, ou inversement plusieurs commandes peuvent arriver collées. Le daemon résout cela en allouant un petit buffer par client : à chaque réception, les octets sont accumulés dans ce buffer, puis le buffer est scanné à la recherche de retours à la ligne. Chaque ligne trouvée est traitée comme une commande complète, et le reste est gardé pour le prochain événement.

L'ensemble du daemon est découpé en cinq petits modules de code, chacun avec une responsabilité claire : gestion des boutons et de la LED Power, serveur TCP et parsing du protocole, rendu OLED, accès au sysfs du module noyau, et orchestration principale. Cette séparation rend le code plus lisible et permet de modifier ou tester chaque partie indépendamment des autres.

## Client en ligne de commande

Le CLI est le plus petit des trois composants. Il joue le rôle d'un client TCP générique pour le protocole du daemon. À l'exécution, il prend tous ses arguments en ligne de commande, les concatène en une seule chaîne séparée par des espaces, ouvre une connexion TCP vers le daemon, envoie la chaîne suivie d'un retour à la ligne, lit la réponse jusqu'au retour à la ligne suivant, affiche cette réponse, et termine.

Le code de retour de l'exécutable suit la convention shell : 0 quand la réponse est un succès, 1 quand elle commence par « ERR ». Cela permet d'utiliser le CLI dans des scripts en chaînant avec `&&` ou `||` sans devoir parser la sortie. L'adresse du serveur et le port sont localhost:1337.

## Communication entre les composants

Le découpage en trois couches a un effet utile : chaque composant ne dépend que de l'interface immédiatement en dessous. Le module noyau ne sait rien du daemon. Le daemon ne sait rien du CLI. Le CLI ne sait rien du module : il ne parle qu'au daemon, qui traduit les commandes texte en lectures/écritures sysfs.

La conséquence pratique : la même interface sysfs offerte par le module peut être utilisée directement avec `cat` et `echo` pour le debug, par le daemon pour le runtime normal, ou par un futur outil graphique sans changer une ligne du noyau. De la même façon, le protocole TCP du daemon peut être utilisé par le CLI fourni ou par n'importe quel autre client (un script Python, `netcat`, un dashboard web), sans modification du daemon.

L'état du système (le mode courant, la fréquence en cours, la dernière température mesurée) est stocké uniquement dans le module noyau. Cette source unique évite les incohérences : il n'y a pas deux endroits où ces valeurs pourraient se désynchroniser. Le daemon, l'écran OLED et tous les clients TCP lisent et écrivent au même endroit, ce qui rend le comportement du système prévisible et facile à comprendre.

## Conclusion

Ce mini-projet a permis de mettre en pratique l'ensemble des notions abordées pendant le cours, notamment :

- l'écriture d'un **module noyau** avec ses fonctions d'init et d'exit, et la gestion propre des ressources (timer, GPIO, fichiers sysfs) lors du déchargement
- l'exposition d'une interface utilisateur propre via **sysfs**, avec validation des entrées et gestion d'erreurs
- la modification du **device tree** pour activer le bus I2C nécessaire à l'écran OLED, et la reconstruction du DTB via **buildroot**
- la cross-compilation et le déploiement sur la cible (kernel, DTB, rootfs, scripts init)
- la programmation système en userspace : **epoll**, **timerfd**, gestion non-bloquante des sockets et des GPIO en sysfs, logging via **syslog**
- la mise en place d'un protocole **TCP** simple
- l'écriture d'un script init pour démarrer automatiquement le daemon au boot.

Au-delà des aspects techniques, le projet montre l'intérêt d'une architecture en couches avec des interfaces standard : chaque composant reste petit, indépendant, et testable séparément.
