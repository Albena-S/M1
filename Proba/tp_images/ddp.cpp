// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  
 
   sscanf (argv[1],"%s",cNomImgLue) ;

   OCTET *ImgIn;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  
  int coleurs[256];
  float ddp[256];
  for (int i=0; i < 256; i++)
    {
     coleurs[i] = 0;
    }
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
      

   free(ImgIn);

   return 1;
}
