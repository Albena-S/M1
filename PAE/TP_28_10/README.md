## Probleme 1 

Je suppose que l'algorithme de parcours passe par A ligne par ligne et par B - colonne par colonne. Donc le parcours de A est séquentiel et de B non. 

### 1.la complexite en cache dans le meilleur des cas
 Si tout les donnes sont charges dans le cache donc les matrices de taille n*n chaqune. La taille de cache(M) est de 2* n*n dans ce cas. Danc ce cas on aurait que des cold miss donc
-> MT(n) = 2* (n**2//B)
MT(n) = O(n**2//B)
### 2.la complexite en cache dans le pire des cas
Le nombre de lectures de la matrice A ne varie pas, parce que c'est un parcour sequentiel. Par contre si la taille de blocs est plus petite que n (le nombre de colonnes de la matrice B), pour chaque ecriture dans la matrice B un chargement sera necessaire.

-> pour charger la marice A : MT(n) = (n**2//B) + 1
-> pour charger la marice B : MT(n) = n**2
                            MT(n) = (n**2//B) + 1 + n**2
                            MT(n) = O(n**2)

### 3.MT(n) ?= O(n2/B), avec n(1 + B)≤M
Comme le parcours de la matrice A est sequentiel, ce n'est pas necessaire d'avoir tout la matrice dans le cache - cela suffit d'avoir une seule ligne a la fois. Par contre B doit etre charge entierement, comme le parcours n'est pas sequentiel.

### 4.
Comme resultat de la commande j'ai :

LEVEL1_ICACHE_SIZE                 32768
LEVEL1_ICACHE_ASSOC                8
LEVEL1_ICACHE_LINESIZE             64

alors la taille de cache est 32768 octets = M/B.
64/8 = 8 mots par ligne

comme on a M ≥ n(1 + B)
            32768 ≥ n*9
            n ≤ 32768/9
            n ≤ 3640

J'ai testé avec 3 matrices de doubles de taille  m x m = 3620 -> M0
                                                 m x m = 3640 -> M1
                                                 m x m = 3660 -> M2.

Contrairement aux resultats de mes calcules, la taille de matrice le plus performante est celle de 3660.

M0:
nbr seconds: 0.0301318
nbr milliseconds: 30.1318
nbr cycles: 75209068
CPI: 75209.1
M1:
nbr seconds: 0.0114734
nbr milliseconds: 11.4734
nbr cycles: 28637878
CPI: 28637.9
M2:
nbr seconds: 0.0103299
nbr milliseconds: 10.3299
nbr cycles: 25783880
CPI: 25783.9




### 5
Je vais resumer les resulats avec 3 exemples. J'ai testé avec 8, 32 et 128 blocs. Je boucle chaque calcul 1000 fois. Je montre que l'execution en milliseconds.

8 blocs:
k 7
nbr milliseconds: 77.0309
k 8
nbr milliseconds: 243.352
k 9
nbr milliseconds: 1166.69
k 10
nbr milliseconds: 8199.15

32 blocs:
k 7
nbr milliseconds: 74.0687
k 8
nbr milliseconds: 221.098
k 9
nbr milliseconds: 956.765
k 10
nbr milliseconds: 4692.2

128 blocs:
k 7
nbr milliseconds: 72.4822
k 8
nbr milliseconds: 271.098
k 9
nbr milliseconds: 1355.45
k 10
nbr milliseconds: 5998.98

De ces recultats je peux conclure que si la taille de la matrice est assez petite ( <= 2^7), le nombre de blocs n'est pas significatif. Par contre avec une augmentation de la taille je trouve que le nombre de blocs entre 16 et 64 est le plus optimal dans tout les cas.

### 6
Le code de la methode MatrixTrans_obliv_v1(...) se trouve dans le fichier exo20.cpp.
Je vais montrer que les resultats avec une taille de matrice de 2^7 et 2^10. 

k 7
MatrixTrans_naif 
nbr seconds: 0.0783826
nbr milliseconds: 78.3826
nbr cycles: 195631608
CPI: 195632
MatrixTrans_bloc 
nbr seconds: 0.0494798
nbr milliseconds: 49.4798
nbr cycles: 123495358
CPI: 123495
MatrixTrans_obliv 
nbr seconds: 0.106038
nbr milliseconds: 106.038
nbr cycles: 264658636
CPI: 264659


k 10
MatrixTrans_naif 
nbr seconds: 19.0327
nbr milliseconds: 19032.7
nbr cycles: 47503584100
CPI: 4.75036e+07
MatrixTrans_bloc 
nbr seconds: 4.69553
nbr milliseconds: 4695.53
nbr cycles: 11719597844
CPI: 1.17196e+07
MatrixTrans_obliv 
nbr seconds: 7.62086
nbr milliseconds: 7620.86
nbr cycles: 19020965404
CPI: 1.9021e+07


L'algorithme de cache oblivious est beaucoup plus rapid que l'algorithme naif avec des grandes valeurs, mais n'est pas efficace sur les plus petites valeurs.
La fonction qui fait du blocking reste la meilleure.


 ### 7
 Le code de la methode MatrixTrans_obliv_v2(...) se trouve dans le fichier exo20.cpp.