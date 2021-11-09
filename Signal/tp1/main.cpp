#include "Wave.hpp"
#include <math.h>
#include <iostream>


/*0  255  x
-1  1   y

y = ax + b
min: -1 = a*0   + b   -> b = -1
max: 1  = a*255 +(-1) -> a = 2/255 = 1/127.5*/

void uchar_to_double(const unsigned char *Y,
					double *X,
					int n){
	for (int i = 0; i < n; i ++){
		X[i] =  ((double) Y[i]/127.5) - 1.0;
	}
}

void double_to_uchar(const double *X,
					unsigned char *Y, 
					int n){
	for (int i = 0; i < n; i ++){
		if (X[i] > 1.0)
			Y[i] = 255;
		else
			if (X[i] < -1)
				Y[i] = 0;
			else
				Y[i] =  (unsigned char) floor(127.5*(X[i] + 1.0));
	}
}


void DFT(double *signal, 
		double *partie_reelle, 
		double *partie_imaginaire, 
		int n){
	
}

int main (int argc, char** argv){

/*
	unsigned char** data;
	Wave a_lire = Wave();
	a_lire.read("Whistle.wav");
	//recopie
	Wave copie = Wave(a_lire.data8, a_lire.data_nb, 1, a_lire.sampling_freq);*/
	
	double duree = 6.,
			freq_echant = 44100.,
			freq_la = 440.;
	int n = (int) floor(freq_echant * duree);
	double Y[n];
	double alpha;

	alpha = 2*M_PI * freq_la / freq_echant;
	for (int k = 0; k < n; k++){
		Y[k] = sin(alpha*k);
	}
	unsigned char X[n];
	double_to_uchar(Y, X, n );

	Wave res = Wave(X, n, 1, freq_echant);
	res.write("sample.wav");

	return 0;
}
