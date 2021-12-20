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

void real_to_uchar_N(double *partie_reelle, 
					double *partie_imaginaire, 
					unsigned char *res,
					int N){
	int i;
	double val, vmax, vmin;

	vmax = partie_reelle[0]*partie_reelle[0] + partie_imaginaire[0]*partie_imaginaire[0];
	vmax = sqrt(vmax);
	vmin = vmax;

	for (i =0; i < N; i++){
		val = partie_reelle[i]*partie_reelle[i] + partie_imaginaire[i]*partie_imaginaire[i];
		val = sqrt(val);

		vmin = vmin > val ? val : vmin;
		vmax = vmax < val ? val : vmax;
	}
	for (i =0; i < N; i++){
		val = partie_reelle[i]*partie_reelle[i] + partie_imaginaire[i]*partie_imaginaire[i];
		val = sqrt(val) ;
		val = (val-vmin) / (vmax - vmin);
		res[i] = (unsigned char)floor(255*val);
	}


}

void DFT(double *signal, 
		double *partie_reelle, 
		double *partie_imaginaire, 
		int N){

	double ak, bk,alpha1,alpha, alpha2;
	alpha1 = 2.0*M_PI/(double)N ;
	for (int k = 0; k < N; k ++){
		ak = 0.;
		bk = 0.;
		alpha2 = alpha1 * (double)k ;
		//std::cout << alpha1 <<" ";
		
		for (int n = 0; n < N; n++){
			alpha = alpha2 * n;
			
			ak += signal[n] * cos(alpha);
			bk += signal[n] * sin(alpha);

		}
		
		partie_reelle[k] = ak;
		partie_imaginaire[k] = - bk;		

	}
	
}

int main (int argc, char** argv){

/*
	unsigned char** data;
	Wave a_lire = Wave();
	a_lire.read("Whistle.wav");
	//recopie
	Wave copie = Wave(a_lire.data8, a_lire.data_nb, 1, a_lire.sampling_freq);*/
	
	/*
	//5.1• Création d’un fichier La.wav
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
	res.write("La.wav");
*/
	std::cout << "115" <<" ";

	double duree = .5,
			freq_echant = 44100.,
			freq_la = 440.,
			freq_si = 475.,
			freq_do = 528.,
			freq_re = 297.,
			freq_mi = 330.,
			freq_fa = 371.,
			freq_sol = 396.;
			;
	int n = (int) floor(freq_echant * duree);
	double Y[n];
	double alpha;
    unsigned char X[n];
	double partie_reelle[n], 
		   partie_imaginaire[n];
	alpha = 2*M_PI * freq_la / freq_echant;
	for (int k = 0; k < n; k++){
		Y[k] = sin(alpha*k);
	}

    double_to_uchar(Y, X, n);
    Wave La = Wave(X, n, 1, freq_echant);
	La.write("La.wav");

	DFT(Y, partie_reelle, partie_imaginaire, n);
	
	real_to_uchar_N(partie_reelle, partie_imaginaire, X, n);

	Wave res = Wave(X, n, 1, freq_echant);
	res.write("testExo2.wav");
	return 0;
}
