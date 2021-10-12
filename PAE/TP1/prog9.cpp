#include "eval-perf.h"
#include <iostream>
#include <immintrin.h>



using namespace std;


double reduce_mul(double *x, int n) {
    __m256d x_vec, accum;
    accum = _mm256_set1_pd(1);

    for(int i = 0; i < n ; i += 4){
        x_vec = _mm256_load_pd(x+i);
        accum = _mm256_mul_pd(x_vec, accum);
    }
    
    double res[4];
    _mm256_storeu_pd(res, accum);
    cout << "res[0] : " << res[0] << " res[1] : " << res[1] << " res[2] : " << res[2] << " res[3] : " << res[3] << endl;
    return  res[0] * res[1] * res[2] * res[3] ;
}

double reduce_add(int32_t *x, int n) {
    __m256i x_vec, accum;
    accum = _mm256_set1_epi32(0);

    for(int i = 0; i < n ; i += 8){
        x_vec = _mm256_load_si256((__m256i*)(x+i));
        accum = _mm256_add_epi32(x_vec, accum);
    }
    
    int32_t res[8],somme = 0;
    _mm256_storeu_si256((__m256i*)res, accum);
    cout << "res[0] : " << res[0] << " res[1] : " << res[1] << " res[2] : " << res[2] << " res[3] : " << res[3] << endl;
    
    for (int i = 0; i < 8 ;i++ )
        somme += res[i];
    return  somme ;
}



int main(int argc, char** argv) {
    int size = 16;        // taille du vect
    int nbr_calls = 123456; //nbr d'appels des fonctions
    double test_d[size];
    int32_t test_int32[size];
    int result;

    EvalPerf PE;

    for(int i = 0; i < size; i ++) {
        test_d[i] =(double) i;
        test_int32[i] =(int32_t) i;

    }
    cout<<"TEST reduce_mul: "<< reduce_mul(test_d, size) << endl;
    cout<<"TEST reduce_add: "<< reduce_add(test_int32, size) << endl;

    // PE.start();
    // for(int i = 0; i < nbr_calls; i ++) {
    //     reduce(v_test, result);
    // }
    // PE.stop();

    // cout<<"TEST reduce: "<<std::endl;
    // cout<<"nombre d'instructions total: "<<size * nbr_calls<<std::endl;
    // cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    // cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    // cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    // cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    // cout<<"CPI= "<<PE.CPI(size * nbr_calls)<<std::endl;
    // cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    // cout<<std::endl;


    

    

    return 0;
}