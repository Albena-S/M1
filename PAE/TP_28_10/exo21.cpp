
#include "eval-perf.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

EvalPerf PE;
void print_PE(std::string method_name, int repetitions){
  std::cout<<  method_name << std::endl;
  std::cout<< "nbr seconds: " << PE.second() << std::endl;
  std::cout<< "nbr milliseconds: " << PE.millisecond() << std::endl;
  std::cout<< "nbr cycles: " << PE.nb_cycle() << std::endl;
  std::cout<< "CPI: " << PE.CPI((double) repetitions) << std::endl;
}

void counting_sort_naif(size_t *R,
                        const size_t *T,
                        size_t n,
                        size_t min,
                        size_t max){
    // Count=[0]*k
    int Count[max-min+1] = {0};
    int i,tmp;
    
    for (i = 0; i < n; i++){
      tmp = T[i];
      if (tmp != max)
        Count[(T[i]-min)+1] ++;
    }
     //for print purpose
    for (i = 0; i < (max-min+1); i++){
      std::cout << Count[i] << " ";
    }
    std::cout << std::endl; 
    //prefixsum
    for (i = 1; i < (max-min+1); i++){
      Count[i] += Count[i-1];
    }
    for (i = 0; i < (max-min +1); i++){
      std::cout << Count[i] << " ";
    }
    std::cout << " T ";
    for (i = 0; i < n; i++){
      std::cout << T[i] << " ";
    }
    std::cout << std::endl; 
    std::cout << std::endl;
    for (i = 0; i < n; i++){
      R[Count[(T[i]-min)]] = T[i];
      Count[(T[i]-min)]++;
    }
}

void counting_sort_bucketing(size_t *R,
                        const size_t *T,
                        size_t n,
                        size_t min,
                        size_t max){
    int i,tmp,nb_buckets, m,taille_bucket, q, 
        current_sous_tab = 0;
    
    nb_buckets = (n>=128) ? 64 : std::min(4,(int) n);
    taille_bucket = n/nb_buckets ;
    std::cout << "nb_buckets:" << nb_buckets<< " taille_bucket:" << taille_bucket << std::endl;

    //Bucket = [0]*nb_buckets;
    int Bucket[nb_buckets] = {0};
    //parcourir tout les elements de tableau initial
    for (i = 0; i < n; i++){
      //trouve le bucket dans lequel l'element se trouve
      q = (taille_bucket * (T[i]))/max;
      //verifier que c'est pas le dernier bucket equiv de T[i] != max
      if (q != nb_buckets-1){
        Bucket[q+1] ++;
        
      }
    }
    //RQ: j'ai trop d'elements qui tombent dans q=0
    //parce que la formule de q n'est pas correcte 
    //et je n'ai pas d'elements qui sont dans le dernier bucket
    //ex: pour {0,1,...,7} il y a 4 buckets mais Bucket= [(0) 4 3 1]
    //cad qu'il y a 4 elements dans q0, 3 dans q1...
    //mais le tableau est une suite donc la repartition dans les buckets
    //n'est pas correcte
    for (int p = 0; p < nb_buckets; p++){
          std::cout << Bucket[p] << " ";
        }
    std::cout << std::endl;
    //prefixsum
    for (i = 1; i < nb_buckets; i++){
      Bucket[i] += Bucket[i-1];
    }
    for (i = 0; i < nb_buckets; i++){
      std::cout << Bucket[i] << " ";
    }
    std::cout << std::endl;

    //BucketedInput=[0]*n
    size_t BucketedInput[n] = {0};
    
    
    for (i = 0; i < n; i++){
      
      q = (taille_bucket * (T[i]))/max;
      BucketedInput[Bucket[q]] = T[i];
      
      std::cout << "q:" << q << " ";
      std::cout << "Bucket[q]:" << Bucket[q] << " ";
       for (int p = 0; p < n; p++){
          std::cout << BucketedInput[p] << " ";
        }
        std::cout << std::endl;
      Bucket[q]++;
    }
    //for print purpose
      std::cout << "BucketInput[i]:" << std::endl;
      for (i = 0; i < n; i++){
        std::cout << BucketedInput[i] << " ";
      }
      std::cout << std::endl;
      

    counting_sort_naif(R,BucketedInput,n, min, max);
}

void counting_sort_naif_v2(size_t *R,
                        const size_t *T,
                        size_t n,
                        size_t min,
                        size_t max){
    // Count=[0]*k
    int Count[max-min+1] = {0};
    int i,tmp,pos=0;
    
    for (i = 0; i < n; i++){
        Count[(T[i]-min)] ++;
    }
    for (i = 0; i < (max-min +1); i++){
      tmp = Count[i];
      for (int p = 0; p < tmp; p++){
        R[pos] = i;
        pos++;
      }
    }
}


int main (int argc, char** argv){
  assert(argc==2);
  //long long int repetitions = atoi(argv[1])*100000;
  long long int repetitions = 1000;
  //varier de 128 a 1024 en 2**k
  int k = atoi(argv[1]);
  int n = pow(2, k);
  std::cout << "k "<< k <<std::endl;
  //int blocs = 8;
  //double A[n*n], B[n*n];
  size_t* A = new size_t[n*n];
  size_t* B = new size_t[n*n];

 //genere les matrices
  n=5;
  for (int i = 0; i< n; i++)
    for (int j = 0; j< n; j++)
      A[i + j*n] = j;

  A[0] = 4;
      //for print purpose
    /* for (int i = 0; i < n*n; i++){
      std::cout << A[i] << " ";
    } */
    std::cout << std::endl;
    int size=8;
    size_t T[size] =  {7,1,5,3,4,2,6,0};//{0,1,2,3,4,5,6,7};//{21,4,7,0,10,13,15,19};
    //counting_sort_naif(B, T, 5, 0, 21);

    //counting_sort_bucketing(B, T, size, 0, 7);
    counting_sort_naif_v2(B, T, size, 0, 7);
    std::cout << "B[i]" << std::endl;
      for (int i = 0; i < size; i++){
      std::cout << B[i] << " ";
    }
    std::cout << std::endl;

 
  return 0;
  }


