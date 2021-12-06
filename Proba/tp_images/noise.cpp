// blur.cpp : Floute une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <iostream>


int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  int som;
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut, *ImgDiff;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(ImgDiff, OCTET, nTaille);
  
   for (int i=0; i < nTaille; i++){
    ImgOut[i]= ImgIn[i];
    ImgDiff[i] = 0;
  }

 for (int i=1; i < nH-1; i++)
   for (int j=1; j < nW-1; j++)
     {
    som=
     ImgIn[(i-1)*nW+j-1]+ImgIn[(i-1)*nW+j]+ImgIn[(i-1)*nW+j+1]
    +ImgIn[i*nW+j-1]+ImgIn[i*nW+j]+ImgIn[i*nW+j+1]
    +ImgIn[(i+1)*nW+j-1]+ImgIn[(i+1)*nW+j]+ImgIn[(i+1)*nW+j+1];
    
       ImgOut[i*nW+j]=som/9;
     }

  int diff;
  for (int i=0; i < nTaille; i++){
    diff = ImgIn[i] - ImgOut[i];
    if (diff < -128) diff = -128;
    if (diff > 127) diff = 127;
    ImgDiff[i] = diff +128; 
   }
   ecrire_image_pgm(cNomImgEcrite, ImgDiff,  nH, nW);
   free(ImgIn);free(ImgOut);free(ImgDiff);
   return 1;
}