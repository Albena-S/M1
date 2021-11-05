## Probleme 1 
### 1.la complexite en cache dans le meilleur des cas
 Si tout les donnes sont charges dans le cache donc les matrices de taille n*n chaqune. La taille de cache(M) est de 2* n*n dans ce cas. Danc ce cas on aurait que des cold miss donc
-> MT(n) = (n//B) + 1 ou n**2//B
### 2.la complexite en cache dans le pire des cas
Le nombre de lectures de la matrice A ne varie pas, parce que c'est un parcour sequentiel. Par contre si la taille de blocs est plus petite que n (le nombre de colonnes de la matrice B), pour chaque ecriture dans la matrice B un chargement sera necessaire

-> pour charger la marice A : MT(n) = (n//B) + 1
-> pour charger la marice B : MT(n) = n

### 3.MT(n) ?= O(n2/B), avec n(1 + B)≤M
Comme le parcours de la matrice A est sequentiel, ce n'ets pas necessaire d'avoir tout la matrice dans le cache - cela suffit d'avoir une seule ligne a la fois. Par contre B doit etre charge entierement, comme le parcours n'est pas sequentiel.

### 4.
LEVEL1_ICACHE_SIZE                 32768
LEVEL1_ICACHE_ASSOC                8
LEVEL1_ICACHE_LINESIZE             64

alors la taille de cache est 32768 octets.
nb de lignes ->32768/64 = 512 lignes
64/8 = 8 mots par ligne
32768/8 = M
8x512 doubles = M

comme on a M ≤n(1 + B)


### 5
avec une repetition de 1000 calculs par methode
albena@albena-Latitude-E5570:~/Documents/M1 git/M1/PAE/TP_28_10$ ./tp_array 7
k 7
MatrixTrans_naif 
nbr seconds: 0.0762594
nbr milliseconds: 76.2594
nbr cycles: 190340572
CPI: 190341
MatrixTrans_bloc 
nbr seconds: 0.050553
nbr milliseconds: 50.553
nbr cycles: 126178980
CPI: 126179
albena@albena-Latitude-E5570:~/Documents/M1 git/M1/PAE/TP_28_10$ ./tp_array 8
k 8
MatrixTrans_naif 
nbr seconds: 0.299918
nbr milliseconds: 299.918
nbr cycles: 748585602
CPI: 748586
MatrixTrans_bloc 
nbr seconds: 0.211202
nbr milliseconds: 211.202
nbr cycles: 527155084
CPI: 527155
albena@albena-Latitude-E5570:~/Documents/M1 git/M1/PAE/TP_28_10$ ./tp_array 9
k 9
MatrixTrans_naif 
nbr seconds: 3.4864
nbr milliseconds: 3486.4
nbr cycles: 8701959240
CPI: 8.70196e+06
MatrixTrans_bloc 
nbr seconds: 1.24196
nbr milliseconds: 1241.96
nbr cycles: 3099894108
CPI: 3.09989e+06
albena@albena-Latitude-E5570:~/Documents/M1 git/M1/PAE/TP_28_10$ ./tp_array 10
k 10
MatrixTrans_naif 
nbr seconds: 19.9716
nbr milliseconds: 19971.6
nbr cycles: 49848528034
CPI: 4.98485e+07
MatrixTrans_bloc 
nbr seconds: 8.07644
nbr milliseconds: 8076.44
nbr cycles: 20158600226
CPI: 2.01586e+07
albena@albena-Latitude-E5570:~/Documents/M1 git/M1/PAE/TP_28_10$ 

### 7 
non:
à k = 9 l'algo cache oblivious se comporte mieux que l'algo naif, alors la version le plus optimale est d'utiliser le tri naif si la taille de matrices 
est < 2**9 et cache oblivious dans le cas ou la taille est plus grande. Les resultats sont donc

tests:
k = 8
n = 64
MatrixTrans_obliv_v2 
nbr seconds: 0.191527
nbr milliseconds: 191.527
nbr cycles: 478046930
CPI: 478047
n=127
MatrixTrans_obliv_v2 
nbr seconds: 0.198409
nbr milliseconds: 198.409
nbr cycles: 495224104
CPI: 495224

n=32
MatrixTrans_obliv_v2 
nbr seconds: 0.193771
nbr milliseconds: 193.771
nbr cycles: 483646350
CPI: 483646

n=16
MatrixTrans_obliv_v2 
nbr seconds: 0.184906
nbr milliseconds: 184.906
nbr cycles: 461521540
CPI: 461522

n=8
MatrixTrans_obliv_v2 
nbr seconds: 0.199885
nbr milliseconds: 199.885
nbr cycles: 498908936
CPI: 498909

-> entre 8 et 32
