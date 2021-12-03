#include "Wave.hpp"
#include <iostream>
#include <cmath>

double charToDouble(char x){
	return ((double)x/127.5)-1.0;
}

unsigned char doubleToChar(double x){
	if(x>1.0){
		return 255;
	}
	if(x<-1.0){
		return 0;
	}
	return (unsigned char)floor(127.5*(x+1.0));
}

/*void DFT(double* signal, double* partie_reelle, double* partie_imaginaire, int N){
	double sommeA=0.0;
	double sommeB=0.0;
	double dpi=2.0*M_PI;
	for(unsigned int i=0;i<N;i++){
		double in=(double)i/(double)N;
		for(unsigned int n=0;n<N;n++){
			sommeA+=signal[n]*cos(dpi*(in*n));
			sommeB-=signal[n]*sin(dpi*(in*n));
		}
		partie_reelle[i]=sommeA;
		partie_imaginaire[i]=sommeB;
		sommeA=0.0;
		sommeB=0.0;
	}
}

void IDFT(double* signal, double* partie_reelle, double* partie_imaginaire, int N){
	
	double sommeR=0.0;
	double sommeI=0.0;
	for(unsigned int n=0;i<N;i++){
		for(unsigned int k=0;i<N;k++){

		}
	}

}*/


void FFT(int dir,int m,double *x,double *y)
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
}


void real2CharN(double* R, double* I, unsigned char* C, int N){
	double val, max, min;
	max=R[0]*R[0]+I[0]*I[0];
	max=sqrt(max);
	min=max;
	for(int i=0;i<N;i++){
		val=R[i]*R[i]+I[i]*I[i];
		val=sqrt(val);
		min= min>val ? val : min;
		max= max<val ? val : max;
	}
	for(int i=0;i<N;i++){
		val=R[i]*R[i]+I[i]*I[i];
		val=sqrt(val);
		val=(val-min)/(max-min);
		C[i]=(unsigned char)floor(255*val);
	}
}

int trouver(int N){
	int m=0;
	int M=1;
	while(M<N){
		m++;
		M=M*2;
	}
	return m;
}

void filtrePasseBas(int freqcut, long M, double* signal, double* I){
	int freqcutsup=M-freqcut;
	std::cout << "freqcut " << freqcut << " " << freqcutsup << std::endl;
	for(unsigned int i=0;i<M;i++){
		if(i>=freqcut and i<=freqcutsup){
			signal[i]=0.0;
			I[i]=0.0;
		}
	}
}

void normalize(double* signal, int N){
	double maxi;
	maxi=fabs(signal[0]);
	for(unsigned int i=0;i<N;i++){
		maxi=fabs(signal[i])>maxi ? fabs(signal[i]) : maxi;
	}
	for(unsigned int i=0;i<N;i++){
		signal[i]/=maxi;
	}
}

int main(int argc, char** argv){
	double time=.5;
	long int freqLa=440;
	long int freqSi=494;
	long int freqDo=262;
	long int freqRe=294;
	long int freqMi=330;
	long int freqFa=349;
	long int freqSol=392;
	short channels_nb=1;
	int sampling_freq=44100;
	long data_nb=(int)floor(time*sampling_freq);
	int m=trouver(data_nb);
	long M=(long)floor(pow(2,m));
	std::cout << "M" << M << std::endl;
	std::cout<<m<<" "<<M<<"\n";
	unsigned char* data8=new unsigned char [data_nb];
	double xk;
	double signal[M];
	//double signal[data_nb];
	unsigned char data;
	int freqcut=(int)floor(M*300/(float)44100);
	for(unsigned int i=0;i<data_nb;i++){
		if(i<=(data_nb-1)/7){
			xk=sin(2.0*M_PI*freqLa*i/(double)sampling_freq);
		}
		else if(i<=2*(data_nb-1)/7){
			xk=sin(2.0*M_PI*freqSi*i/(double)sampling_freq);
		}
		else if(i<=3*(data_nb-1)/7){
			xk=sin(2.0*M_PI*freqDo*i/(double)sampling_freq);
		}
		else if(i<=4*(data_nb-1)/7){
			xk=sin(2.0*M_PI*freqRe*i/(double)sampling_freq);
		}
		else if(i<=5*(data_nb-1)/7){
			xk=sin(2.0*M_PI*freqMi*i/(double)sampling_freq);
		}
		else if(i<=6*(data_nb-1)/7){
			xk=sin(2.0*M_PI*freqFa*i/(double)sampling_freq);
		}
		else{
			xk=sin(2.0*M_PI*freqSol*i/(double)sampling_freq);
		}
		signal[i]=xk;
	}
	//double R[data_nb];
	double I[M];
	//double I[data_nb];
	for(unsigned int i=data_nb;i<M;i++){
		signal[i]=0.0;	
	}
	for(unsigned int i=0;i<M;i++){
		I[i]=0.0;
	}
	for(unsigned int i=0;i<data_nb;i++){
		data8[i]=doubleToChar(signal[i]);
	}
	Wave* La=new Wave(data8, data_nb, channels_nb, sampling_freq);
	//double k=440;
	//filtrePasseBas(freqcut,M,signal,I);
	//DFT(signal,R,I,data_nb);
	La->write("gamme.wav");
	FFT(1,m,signal,I);
	filtrePasseBas(freqcut,M,signal,I);
	FFT(-1,m,signal,I);
	normalize(signal,data_nb);
	//real2CharN(signal,I,data8,data_nb);
	for(unsigned int i=0;i<data_nb;i++){
		data8[i]=doubleToChar(signal[i]);
	}
	Wave* filtre=new Wave(data8, data_nb, channels_nb, sampling_freq);
	//Wave* La=new Wave(data8, data_nb, channels_nb, sampling_freq);
	filtre->write("gammefiltre.wav");
	return 0;
}