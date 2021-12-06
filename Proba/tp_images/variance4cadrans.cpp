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
    int nbDivisions = 2,
      tailleCadrant = nH / 2;
  
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

    int nbCadrants = 2*nbDivisions;
    float moyenne[nbCadrants], variance[nbCadrants],ecart_type[nbCadrants], 
        moyenneLocalle = 0,ecarts_carres =0;
    for (int cadrant=0; cadrant < nbCadrants; cadrant++){
      for (int i=cadrant*tailleCadrant; i < tailleCadrant*(cadrant+1); i++)
        for (int j=cadrant*tailleCadrant; j < tailleCadrant*(cadrant+1); j++){
          std::cout << "i:" << cadrant*tailleCadrant << " " << tailleCadrant*(cadrant+1)<<"moyenneLocalle" << moyenneLocalle << std::endl;
          moyenneLocalle += ((i + j) *coleurs[i + j] ); 
        }
      moyenne[cadrant] = moyenneLocalle/ tailleCadrant;
       for (int i=cadrant*tailleCadrant; i < tailleCadrant*(cadrant+1); i++)
        for (int j=cadrant*tailleCadrant; j < tailleCadrant*(cadrant+1); i++){
          ecarts_carres += ( ImgIn[i + j*tailleCadrant] - moyenne[cadrant] ) * ( ImgIn[i + j*tailleCadrant] - moyenne[cadrant] ); 
        }
      variance[cadrant] = ecarts_carres / nTaille;
      ecart_type[cadrant] = sqrt(variance[cadrant]);
      std::cout << "moyenne[cadrant]" << moyenne[cadrant] << "variance[cadrant]" << variance[cadrant] << "ecart_type[cadrant]" << ecart_type[cadrant] << std::endl;
      moyenneLocalle = 0;ecarts_carres = 0;
    }


   

    std::cout << "moyenne " << moyenne << std::endl;
    std::cout << "variance " << variance << std::endl;
    std::cout << "ecart_type " << ecart_type << std::endl;
    FILE *file;
    //file = fopen("ddp.dat", "w");
    for (int i=0; i < 256; i++)
    {
     //fprintf(file, "%d %f\n", i, ddp[i]);
    }

    //fclose(file);
      

/* for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       if ( ImgIn[i*nW+j] < S) ImgOut[i*nW+j]=0; else ImgOut[i*nW+j]=255;
     }*/

   //ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn); free(ImgOut);

   return 1;
}
