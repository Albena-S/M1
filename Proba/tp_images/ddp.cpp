// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  
  /*if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm Seuil \n"); 
       exit (1) ;
     }*/
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   //sscanf (argv[2],"%s",cNomImgEcrite);
   //sscanf (argv[3],"%d",&S);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
	
  int coleurs[256];
  float ddp[256];
  for (int i=0; i < 256; i++)
    {
     coleurs[i] = 0;
    }
   //   for (int i=0; i < nTaille; i++)
   // {
   //  if ( ImgIn[i] < S) ImgOut[i]=0; else ImgOut[i]=255;
   //  }
    int total =0;
    for (int i=0; i < nTaille; i++)
    {
      coleurs[ImgIn[i]]++;
    }
    for (int i=0; i < 256; i++)
    {
      ddp[i] = (float)coleurs[i] / nTaille;
    }
    FILE *file;
    file = fopen("ddp.dat", "w");
    for (int i=0; i < 256; i++)
    {
     fprintf(file, "%d %f\n", i, ddp[i]);
    }

    fclose(file);
      

/* for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       if ( ImgIn[i*nW+j] < S) ImgOut[i*nW+j]=0; else ImgOut[i*nW+j]=255;
     }*/

   //ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn); free(ImgOut);

   return 1;
}
