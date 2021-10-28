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