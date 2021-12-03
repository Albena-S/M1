#include "Wave.hpp"
#include <math.h>
#include <iostream>
#include <string>

//dans les fichiers sonores il y a toujours une symetrie -> vibraitons sur une membrane
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

int trouver_m(int N){
	int m = 0;
	int M = 1;
	while( M < N){
		m++;
		M = M*2;
	}
	return m;
}

void normalize(double* signal,
			   int N){
	double maxi;
	maxi=fabs(signal[0]);
	for(unsigned int i=0;i<N;i++){
		maxi=fabs(signal[i])>maxi ? fabs(signal[i]) : maxi;
	}
	for(unsigned int i=0;i<N;i++){
		signal[i]/=maxi;
	}
}
/*
	This FFT has been proposed by Paul Bourke 
	http://paulbourke.net/miscellaneous/dft/
	This computes an in-place complex-to-complex FFT 
	x and y are the real and imaginary arrays of 2^m points.
	dir =  1 gives forward transform
	dir = -1 gives reverse transform 
	You MUST compute first the value m such that
	2^(m-1) < n (size of your signal) <= 2^m
	allocate a new signal of nm=2^m values
	then fill the n first values of this new signal 
 with your signal and fill the rest with 0
	WARNING : you must pass m, not nm !!!
	*/

int FFT(int dir,int m,double *x,double *y)
{
	int n,i,i1,j,k,i2,l,l1,l2;
	double c1,c2,tx,ty,t1,t2,u1,u2,z;
	
	/* Calculate the number of points */
	n = 1;
	for (i=0;i<m;i++) 
		n *= 2;
	
	/* Do the bit reversal */
	i2 = n >> 1;
	j = 0;
	for (i=0;i<n-1;i++) {
		if (i < j) {
			tx = x[i];
			ty = y[i];
			x[i] = x[j];
			y[i] = y[j];
			x[j] = tx;
			y[j] = ty;
		}
		k = i2;
		while (k <= j) {
			j -= k;
			k >>= 1;
		}
		j += k;
	}
	
	/* Compute the FFT */
	c1 = -1.0; 
	c2 = 0.0;
	l2 = 1;
	for (l=0;l<m;l++) {
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0; 
		u2 = 0.0;
		for (j=0;j<l1;j++) {
			for (i=j;i<n;i+=l2) {
				i1 = i + l1;
				t1 = u1 * x[i1] - u2 * y[i1];
				t2 = u1 * y[i1] + u2 * x[i1];
				x[i1] = x[i] - t1; 
				y[i1] = y[i] - t2;
				x[i] += t1;
				y[i] += t2;
			}
			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}
		c2 = sqrt((1.0 - c1) / 2.0);
		if (dir == 1) 
			c2 = -c2;
		c1 = sqrt((1.0 + c1) / 2.0);
	}
	
	/* Scaling for forward transform */
	if (dir == 1) {
		for (i=0;i<n;i++) {
			x[i] /= n;
			y[i] /= n;
		}
	}
	
	return(1);
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

/*void IDFT(double *signal, 
		double *partie_reelle, 
		double *partie_imaginaire, 
		int N){
	//m = 2^N
	FFT(-1, trouver_m(N) ,partie_reelle, partie_imaginaire  );

}*/
void filtre_passe_bas_numerique(int freq_coup,
								int freq_echant,
								double *partie_reelle, 
								double *partie_imaginaire, 
								int M){
	int freq_coup_deb = (int) floor((M*freq_coup) / (float)freq_echant); 
	int freq_coup_fin = M - freq_coup_deb; 
	std::cout << "freq_coup " << freq_coup << "  freq_coup_deb " << freq_coup_deb << "  freq_coup_fin " << freq_coup_fin<< std::endl;
	for (int k = freq_coup_deb; k < freq_coup_fin; k++){
			partie_reelle[k] = 0.;
			partie_imaginaire[k] = 0.;		
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

	std::cout << "115" <<" ";
	//exemple avec mon dft
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

	alpha = 2*M_PI * freq_la / freq_echant;
	for (int k = 0; k < n; k++){
		Y[k] = sin(alpha*k);
	}
	//unsigned char X[n];
	double partie_reelle[n], 
		   partie_imaginaire[n];

	
	DFT(Y, partie_reelle, partie_imaginaire, n);
	real_to_uchar_N(partie_reelle, partie_imaginaire, X, n);
	Wave res = Wave(X, n, 1, freq_echant);
	res.write("testExo2IDFT.wav");

	*/
/*	TF du 26.11
	double duree = .5,
			freq_echant = 44100.,
			freq_la = 440.;

	int n = (int) floor(freq_echant * duree);
	int m = trouver_m(n),
		M = (int)floor(pow(2,m));
	double partie_reelle[M],partie_imaginaire[M];
	double alpha;

	alpha = 2*M_PI * freq_la / freq_echant;
	for (int k = 0; k < n; k++){
		partie_reelle[k] = sin(alpha*k);
		partie_imaginaire[k] = .0;
	}
	for (int i = n; i < M; i++ ){
		partie_reelle[i] = .0;
		partie_imaginaire[i] = .0;
	}
	//std::cout << "partie_reelle[n-1]" << partie_reelle[n-1] << " partie_reelle[n]" << partie_reelle[n]<< std::endl;
	FFT(1, M, partie_reelle, partie_imaginaire);
	//std::cout << "partie_reelle[n-1]" << partie_reelle[n-1] << " partie_reelle[n]" << partie_reelle[n]<< std::endl;
	
	FFT(-1, M, partie_reelle, partie_imaginaire);

	unsigned char signal_final_M[M], signal_final[n];

	double_to_uchar(partie_reelle, signal_final_M, M );
	std::cout << "signal_final_M[n]" << signal_final_M[n]<< std::endl;
	
	//std::cout << << std::endl;
	for (int i = 0; i < n; i++ ){
		signal_final[i] = signal_final_M[i]; 
	}

	Wave res = Wave(signal_final, n, 1, freq_echant);
	res.write("testExo2IDFT.wav");*/

	
	double duree = .5,
			freq_echant = 44100.;
	double freq_gamme[7] = { 440., 297., 330., 371., 396., 475., 528.,};

	int n = (int) floor(freq_echant * duree);
	int m = trouver_m(n),
		M = (int)floor(pow(2,m));
	double partie_reelle[M],partie_imaginaire[M];
	double alpha;
	std::cout << "M" << M << std::endl;
	alpha = 2*M_PI * freq_gamme[0] / freq_echant;
	int gamme=0, gamme_i=0;
	for (int k = 0; k < n; k++){
		
		partie_reelle[k] = sin(alpha*k);
		partie_imaginaire[k] = .0;
		if (k % (n/7)==0 && k != 0){
			gamme_i ++;
			std::cout << gamme_i << " " << k << std::endl;
			alpha = 2*M_PI * freq_gamme[gamme_i] / freq_echant;
		}
	}
	for (int i = n; i < M; i++ ){
		partie_reelle[i] = .0;
		partie_imaginaire[i] = .0;
	}
	unsigned char signal_final_M[M], signal_final[n];
	double_to_uchar(partie_reelle, signal_final, n );
	Wave init = Wave(signal_final, n, 1, freq_echant);
	init.write("TestGamme.wav");
	FFT(1, m, partie_reelle, partie_imaginaire);
	
	//filtre_passe_bas_numerique(300, (int)freq_echant, partie_reelle, partie_imaginaire, M);

	FFT(-1, m, partie_reelle, partie_imaginaire);
	//normalize(partie_reelle,n);
	//unsigned char  signal_final[n];

	double_to_uchar(partie_reelle, signal_final, n );


	Wave res = Wave(signal_final, n, 1, freq_echant);
	res.write("testExo2TestGamme.wav");
	return 0;
}
