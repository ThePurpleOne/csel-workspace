Start by adding binutils to the buildroot packages

```
make menuconfig
```

bla bla bla


# ex01

```
# perf stat ./ex1 

 Performance counter stats for './ex1':

          38374.88 msec task-clock                #    1.000 CPUs utilized          
                20      context-switches          #    0.521 /sec                   
                 0      cpu-migrations            #    0.000 /sec                   
             48866      page-faults               #    1.273 K/sec                  
       31313624898      cycles                    #    0.816 GHz                    
        1669023051      instructions              #    0.05  insn per cycle         
         269482446      branches                  #    7.022 M/sec                  
            995880      branch-misses             #    0.37% of all branches        

      38.392579352 seconds time elapsed

      37.705882000 seconds user
       0.288476000 seconds sys

```

> Ce programme contient une erreur triviale qui empêche une utilisation optimale du cache. De quelle erreur s’agit-il ?

Le problème vient de l’ordre de parcours du tableau en mémoire. 

Dans le code actuel, on parcourt le tableau colonne par colonne. Le processeur doit alors charger continuellement de nouvelles lignes de cache, ce qui provoque beaucoup de *cache misses* et ralentit l’exécution.

Si on inverse l’ordre des boucles pour parcourir le tableau ligne par ligne, les accès mémoire deviennent contigus. Les données déjà chargées dans le cache sont alors réutilisées immédiatement, ce qui réduit fortement les accès mémoire et améliore les performances.

> Corrigez l’erreur, recompilez et mesurez à nouveau le temps d’exécution (soit avec perf stat, soit avec la commande time). Quelle amélioration constatez-vous ?

avec la nouvelle version, on est ~15x plus rapide.

```
# perf stat ./ex1 

 Performance counter stats for './ex1':

           2440.66 msec task-clock                #    0.991 CPUs utilized          
                16      context-switches          #    6.556 /sec                   
                 0      cpu-migrations            #    0.000 /sec                   
             48867      page-faults               #   20.022 K/sec                  
        1991465721      cycles                    #    0.816 GHz                    
        1381039328      instructions              #    0.69  insn per cycle         
         265156001      branches                  #  108.641 M/sec                  
            650234      branch-misses             #    0.25% of all branches        

       2.461985334 seconds time elapsed

       2.169523000 seconds user
       0.249727000 seconds sys
```


> Relevez les valeurs du compteur L1-dcache-load-misses pour les deux versions de l’application. Quel facteur constatez-vous entre les deux valeurs ?

```
# perf stat -e L1-dcache-load-misses ./ex1

 Performance counter stats for './ex1':

           1251307      L1-dcache-load-misses

       2.457056959 seconds time elapsed

       2.216524000 seconds user
       0.202376000 seconds sys

# perf stat -e L1-dcache-load-misses ./ex1_old 

 Performance counter stats for './ex1_old':

         407283904      L1-dcache-load-misses

      41.003656145 seconds time elapsed

      40.315958000 seconds user
       0.268853000 seconds sys

```


> Décrivez brièvement ce que sont les évènements suivants :
- instructions
- cache-misses
- branch-misses
- L1-dcache-load-misses
- cpu-migrations
- context-switches


TODO flemme

> mesurez le temps d’exécution de notre application ex1 avec et sans la commande perf stat.

```
# time ./ex1
real	0m 2.42s
user	0m 2.15s
sys	0m 0.23s
# time perf stat ./ex1

 Performance counter stats for './ex1':

           2447.19 msec task-clock                #    0.994 CPUs utilized          
                17      context-switches          #    6.947 /sec                   
                 0      cpu-migrations            #    0.000 /sec                   
             48867      page-faults               #   19.969 K/sec                  
        1996773333      cycles                    #    0.816 GHz                    
        1381182602      instructions              #    0.69  insn per cycle         
         265157848      branches                  #  108.352 M/sec                  
            651566      branch-misses             #    0.25% of all branches        

       2.461889210 seconds time elapsed

       2.207966000 seconds user
       0.217502000 seconds sys


real	0m 2.68s
user	0m 2.22s
sys	0m 0.26s
```

# ex02

> Décrivez en quelques mots ce que fait ce programme.

le programme génère des valeurs aléatoires entre 0 et 511, puis il fait 10'000 fois la somme des valeurs plus grandes ou égales à 256

> Mesurez le temps d’exécution

```
# perf stat ./ex2
sum=125454290000

 Performance counter stats for './ex2':

          26177.75 msec task-clock                #    0.999 CPUs utilized          
                17      context-switches          #    0.649 /sec                   
                 0      cpu-migrations            #    0.000 /sec                   
                74      page-faults               #    2.827 /sec                   
       21360932793      cycles                    #    0.816 GHz                    
       14768697496      instructions              #    0.69  insn per cycle         
         988554201      branches                  #   37.763 M/sec                  
         327876971      branch-misses             #   33.17% of all branches        

      26.197301971 seconds time elapsed

      26.118607000 seconds user
       0.007923000 seconds sys

```

> Compilez et mesurez le temps d’exécution de la version modifiée.

Avec le sort avant, nous gagnons environ 3 secondes d'execution.

avec les stats on voit qu'il n'y a beaucoup moins de branch-misses, et c'est normal. Dans la version ou le tableau n'est pas trié, le processeur essaie de prédire si la prochaine valeur sera plus >= 256 ou non, mais puisque les valeurs sont aléatoires, ce n'est pas vraiment possible. Dans la version avec le tableau triée, on prédit la branch correctement presque à chaque fois

```
# perf stat ./ex2
sum=125454290000

 Performance counter stats for './ex2':

          23429.85 msec task-clock                #    0.999 CPUs utilized          
                17      context-switches          #    0.726 /sec                   
                 0      cpu-migrations            #    0.000 /sec                   
               108      page-faults               #    4.610 /sec                   
       19118643675      cycles                    #    0.816 GHz                    
       14818393578      instructions              #    0.78  insn per cycle         
         997843161      branches                  #   42.589 M/sec                  
            813780      branch-misses             #    0.08% of all branches        

      23.450767387 seconds time elapsed

      23.381436000 seconds user
       0.003901000 seconds sys

```

## ex03

> Compilez l’application et profilez l’application avec perf record

```
# perf record --call-graph dwarf -e cpu-clock -F 75 ./read-apache-logs access_log_NASA_Jul95_samples
Couldn't synthesize bpf events.
Processing log file access_log_NASA_Jul95_samples
Found 14867 unique Hosts/IPs
[ perf record: Woken up 334 times to write data ]
[ perf record: Captured and wrote 83.432 MB perf.data (10238 samples) ]
```

> Avec les instructions précédentes, déterminez quelle fonction de notre application fait (indirectement) appel à std::operator==<char>

```
-   25.99%  read-apache-log  /root/read-apache-l  0x3080             d [.] std::operator==<char>
     std::operator==<char> read-apache-logs
     ...
     HostCounter::isNewHost read-apache-logs
     HostCounter::notifyHost read-apache-logs
     ApacheAccessLogAnalyzer::processFile read-apache-logs
     main read-apache-logs
     ...
```

la fonction `isNewHost` est celle qui utilise `std::operator==<char>` et qui cause des problèmes de performance. En remplacant le vector par un set, on peut optimiser la fonction. 

```
# time ./read-apache-logs access_log_NASA_Jul95_samples 
Processing log file access_log_NASA_Jul95_samples
Found 14867 unique Hosts/IPs
real	0m 1.56s
user	0m 1.41s
sys	0m 0.07s

# time ./read-apache-logs_old  access_log_NASA_Jul95_samples 
Processing log file access_log_NASA_Jul95_samples
Found 14867 unique Hosts/IPs
real	2m 15.75s
user	2m 14.78s
sys	0m 0.12s
```

on passe de 2 minutes 15 à 2 secondes.


> Décrivez comment devrait-on procéder pour mesurer la latence et la gigue d’interruption, ceci aussi bien au niveau du noyau (kernel space) que de l’application (user space).

Pas compris ?? Je sais pas si c'est pour le meme exercice ou pas, ou si c'est juste une question random. 