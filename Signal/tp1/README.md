Screenshot from 2021-12-18 20-42-47

filtre pas bas :
signal 1 - original la gamme
signal 2 - filtre pas bas 300
signal 3 - filtre pas bas


Screenshot from 2021-12-18 21-50-48
La genere 
DFT sur la note La
pique - dirac

Screenshot from 2021-12-18 22-03-10
LaMi
DFT sur LaMi


Screenshot from 2021-12-19 17-15-28
gamme 
fourrier

Screenshot from 2021-12-19 17-18-41
gamme forrier piques 

Screenshot from 2021-12-19 23-05-26
gamme
gamme 300
360

Screenshot from 2021-12-20 00-50-50
superposition

## GENERAL / Mise en contexte

Je vais expliquer en quelques phrases le parametrage principale.
La fréquence d'échantillonnage, correspond au nombre d’échantillons  pour une seconde d'enregistrement. Comme valeur de frequence j'ai choisi 44100 en se basant sur le Théorème d'échantillonnage qui dit que La représentation discrète d'un signal exige des échantillons régulièrement espacés à une fréquence d'échantillonnage supérieure au double de la fréquence maximale présente dans ce signal. La frenquence maximale dans mes fichier son est moins que 20 000 Hz parce que l'oreille humaine ne perçois pas des frequences au dela de cette valeur.
Tout compte fait une frenquence de 44100 Hz signifie 44100 échantillons pour une seconde de son mémorisé.
Le debit de mes signaux sonores reste aussi a 44100 ko/s a cause de l'enregistrés des fichiers en mono-voie. 
Le format des donnees est des entiers non signes. Avec un signal 8 bits le point le plus bas vaut zéro et le point le plus haut vaut 255. 
Pour le traitement de ces donnees j'utilise un format de flottants dans l'intervalle [-1,1], avec -1 correspondant a la valeur la plus basse et respectivemenet 1 a la plus haute.

## EXO1 - Creation de la note 'La'
J'ai creer un fichier La.wav en y synthétisant la note "la" (440 Hz) en mono pendant 6 seconde.
La taille de mon échantillon est la frequence d'echantion * 6, car comme dit precedament, la frequence represente le nombre d’échantillons  pour une seconde d'enregistrement.
Pour generer les valuers reeles du son j'ai utilise la fonction de signal[k] = sin(alpha * k), avec k = 0 ... n-1, alpha = 2*M_PI * ( 440 / 44 100) .

Visuellement, la note 'La' ressemble a :
*image de LA dans AUDACITY*


## EXO2
La transformation de Fourier discrètes directe est la discrétisation de la transformée continue théorique. La DFT permet de déterminer la pondération entre différentes fréquences discrètes.
Pour les calules, j'ai utilise la formule, donnee dans le feuille de TP donc pour un signal réel discret représenté par la suite de ses n échantillons (xn), sa transformée est la suite de valeurs complexes Xk telles que Xk = ak + ibk, avec ak = .., bk = ..

Le resultat de la methode DFT applique sur le signal de la note 'La' represente 2 pique :
*image de LA dans AUDACITY*

Au debut j'etais partie avec quelques idees fausses.
Je pensais que les 2 piques symetriques representent la partie reele et la partie imaginaire. J'ai compris plus tard que c'est un repliment de spectre qui est due au fait que la composante fréquentielle du signal discret xk represente 2 valeurs complexes - Xk et XN-k. J'ai trouve des techniques d'eviter le repliment de spectre comme 'Lower frequency transducer' et 'Adjusting scale to its maximum' mais je n'ai pas appliquer une telle strategie.
Mon deuxieme point de incompehension etait que j'imaginait que les piques sont generes par rapport a la conséquence des differentes frequences dans l'ordre original du signal. Cela s'est passe a cause du fait que j'ai ecrit la methode sans vraiment beaucpup y reflechissant. Dans la partie 3.Transforme de fourier numerique c'est explique que Xk = ...somme... qui signifie que pour calculer une valeur complexe, la methode boucle sur toutes les valeurs reeles (xn). Je me suis rendu compte de mon errur quand j'ai visualise le dft de la gamme dans ordre de notes differents. 
Sur la capture c'est visible que le 3eme et le 4eme pique sont tres proches. Ainsi les seuls notes aussi proches sont Mi(330 Hz) et Fa(349 Hz) et les notes Mi et Fa ne se sont pas suivi dans le signal sonore original que je passait. Je me suis alors rendu compte que l'ordre dans le signal traite du DFT representait l'ordre des frequences croissantes.  


Le calcul de DFT et IDFT s'expriment en O(n2). Il existe un alogrithme de transformation de Fourier rapide qui varie en O(n log n). Je me suis servie de la  methode FFT propose par Paul Bourke. Le schema suivant montre son utilisation.
 *image de fft piktochart*

montre lasuperposition des notes

## EXO4
Un filtre passe-bas est un filtre qui laisse passer les basses fréquences et qui atténue les hautes fréquences supérieures à la fréquence de coupure. 
L'algorithme est assez simple - j'annule tout les valeurs plus hautes que la frequence de coupure. La position de cette derniere dans la partie reelle et la partie imaginaire du FFT est calcule par la formule M*freq_coup / freq_echant . Toutes les valeurs apres cette position sont mis a 0.

Avec les resultats du filtrage je constate que le filtrage