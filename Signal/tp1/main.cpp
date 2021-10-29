#include "Wave.hpp"

int main (int argc, char** argv){


	unsigned char** data;


	Wave a_lire = Wave();
	a_lire.read("Whistle.wav");
	
	//recopie
	Wave copie = Wave(a_lire.data8, a_lire.data_nb, 1, a_lire.sampling_freq);
	copie.write("sample.wav");

	//exo1 - synthese de la note la - 440 Hz
	freqE = 44100 Hz freq de l'echqntillage
	freq = 440 hz, le signal continue est sin(2*PI*freq)
	dans un moment k Tc on a xk = 
	Wave copie = Wave(a_lire.data8, a_lire.data_nb, 1, a_lire.sampling_freq);
	copie.write("sample.wav");


//A faire - fct qui trqnsforme tqb de unsigned char a un tab de double parce qu'on stocke dans les fichier en unsigned [0,255]
/*	et on traite le sognal en double [-1,1]
	pour le son de la
	faire une boucle pour les k
	chque vqleur (xk) est sin(2PI*f/fc*k) */
	return 0;
}