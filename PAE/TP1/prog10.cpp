#include "eval-perf.h"
#include <iostream>
#include <immintrin.h>



using namespace std;


double min(float *x, int n) {
    __m256 x_vec, min, mask;
    min = _mm256_load_ps(x);

    cout<<"TEST avant boucle: " << endl;
    for(int i = 1; i < n ; i += 8){
        cout<<"TEST 16" << endl;
        x_vec = _mm256_load_ps(x+i);
        cout<<"TEST 17: " << endl;
        mask = _mm256_cmp_ps(x_vec, min, _CMP_LT_OQ);
        min = _mm256_blendv_ps(x_vec, min, mask);
    }
    
    float res[8],somme = 0;
    _mm256_storeu_ps(res, min);
    cout << "res[0] : " << res[0] << " res[1] : " << res[1] << " res[2] : " << res[2] << " res[3] : " << res[3] << endl;
    
    // for (int i = 0; i < 8 ;i++ )
    //     somme += res[i];
    return  somme ;
}



int main(int argc, char** argv) {
    int size = 16;        // taille du vect
    int nbr_calls = 123456; //nbr d'appels des fonctions
    double test_f[size];
    float test_int32[size];
    int result;

    EvalPerf PE;

    for(int i = 0; i < size; i ++) {
        test_f[i] = (float) i;

    }

    
    cout<<"TEST min: "<< min(test_int32, size) << endl;

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