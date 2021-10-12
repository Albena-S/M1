#include "eval-perf.h"
#include <iostream>
#include <vector>

#define OP *
#define START 1

using namespace std;

template <typename T>
void reduce (const std::vector<T> &V, T &res) {
    res=START;

    for(size_t i = 0; i < V.size(); i ++)
        res = res OP V[i];
}
//reduce 2 -> stockage de la taille une seule fois avant la boucle
template <typename T>
void reduce2 (const std::vector<T> &V, T &res) { 
    res=START;
    int size = V.size();

    for(int i = 0; i < size; i ++)
        res = res OP V[i];
}
//reduce 3 -> accumulateur pour faire les calculs pour n'utiliser res qu'une seule fois
template <typename T>
void reduce3 (const std::vector<T> &V, T &res) {
    res=START;
    int size = V.size();
    int acc = res;

    for(int i = 0; i < size; i ++)
        acc = acc OP V[i];
    res = acc;
}
//reduce 4 -> tentative de déroulement de la boucle (2 calculs par tour de boucle) pour utiliser le parallelisme d'instruction
template <typename T>
void reduce4 (const std::vector<T> &V, T &res) {
    res=START;
    int size = V.size();
    int acc = res;
    int acc2 = START;
    int i;

    for(i = 0; i < size-1; i +=2){
        acc = acc OP V[i];
        acc2 = acc2 OP V[i + 1];
    }

    for(;i < size; i++)             /* si vecteur de taille impaire il faut calculer la dernière valeur */
        acc = acc OP V[i]; 

    res = acc OP acc2;
}
//reduce 5 -> 
template <typename T>
void reduce5 (const std::vector<T> &V, T &res) {
    res=START;
    int size = V.size();
    int acc = res;
    int acc2 = START;
    int acc3 = START;
    int acc4 = START;
    int i;

    for(i = 0; i < size-1; i +=4){
        acc = acc OP V[i];
        acc2 = acc2 OP V[i + 1];
        acc3 = acc3 OP V[i + 2];
        acc4 = acc4 OP V[i + 3];
    }

    for(;i < size; i++)             /* si vecteur de taille impaire il faut calculer la dernière valeur */
        acc = acc OP V[i]; 

    res = acc OP acc2 OP acc3 OP acc4;
}
//reduce 6
template <typename T>
void reduce6 (const std::vector<T> &V, T &res) {
    res=START;
    int size = V.size();
    int acc = res;
    int acc2 = START;
    int acc3 = START;
    int acc4 = START;
    int acc5 = START;
    int acc6 = START;
    int acc7 = START;
    int acc8 = START;
    int acc9 = START;
    int acc10 = START;
    int i;

    for(i = 0; i < size-1; i +=10){
        acc = acc OP V[i];
        acc2 = acc2 OP V[i + 1];
        acc3 = acc3 OP V[i + 2];
        acc4 = acc4 OP V[i + 3];
        acc5 = acc5 OP V[i + 4];
        acc6 = acc6 OP V[i + 5];
        acc7 = acc7 OP V[i + 6];
        acc8 = acc8 OP V[i + 7];
        acc9 = acc9 OP V[i + 8];
        acc10 = acc10 OP V[i + 9];
    }

    for(;i < size; i++)             /* si vecteur de taille impaire il faut calculer la dernière valeur */
        acc = acc OP V[i]; 

    res = acc OP acc2 OP acc3 OP acc4 OP acc5 OP acc6 OP acc7 OP acc8 OP acc9 OP acc10;
}

//reduce 7 : 12 itérations
template <typename T>
void reduce7 (const std::vector<T> &V, T &res) {
    res=START;
    int size = V.size();
    int acc = res;
    int acc2 = START;
    int acc3 = START;
    int acc4 = START;
    int acc5 = START;
    int acc6 = START;
    int acc7 = START;
    int acc8 = START;
    int acc9 = START;
    int acc10 = START;
    int acc11 = START;
    int acc12 = START;
    int i;

    for(i = 0; i < size-1; i +=10){
        acc = acc OP V[i];
        acc2 = acc2 OP V[i + 1];
        acc3 = acc3 OP V[i + 2];
        acc4 = acc4 OP V[i + 3];
        acc5 = acc5 OP V[i + 4];
        acc6 = acc6 OP V[i + 5];
        acc7 = acc7 OP V[i + 6];
        acc8 = acc8 OP V[i + 7];
        acc9 = acc9 OP V[i + 8];
        acc10 = acc10 OP V[i + 9];
        acc11 = acc11 OP V[i + 9];
        acc12 = acc12 OP V[i + 10];
    }

    for(;i < size; i++)             /* si vecteur de taille impaire il faut calculer la dernière valeur */
        acc = acc OP V[i]; 

    res = acc OP acc2 OP acc3 OP acc4 OP acc5 OP acc6 OP acc7 OP acc8 OP acc9 OP acc10 OP acc11 OP acc12;
}

int main(int argc, char** argv) {
    int size = 1000;        // taille du vect
    int nbr_calls = 123456; //nbr d'appels des fonctions
    std::vector<int> v_test(1000);
    int result;

    EvalPerf PE;

    for(int i = 0; i < size; i ++) 
        v_test[i] = i;
    
    PE.start();
    for(int i = 0; i < nbr_calls; i ++) {
        reduce(v_test, result);
    }
    PE.stop();

    cout<<"TEST reduce: "<<std::endl;
    cout<<"nombre d'instructions total: "<<size * nbr_calls<<std::endl;
    cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    cout<<"CPI= "<<PE.CPI(size * nbr_calls)<<std::endl;
    cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    cout<<std::endl;


    PE.start();
    for(int i = 0; i < nbr_calls; i ++) {
        reduce2(v_test, result);
    }
    PE.stop();

    cout<<"TEST reduce2: "<<std::endl;
    cout<<"nombre d'instructions total: "<<size * nbr_calls<<std::endl;
    cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    cout<<"CPI= "<<PE.CPI(size * nbr_calls)<<std::endl;
    cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    cout<<std::endl;


    PE.start();
    for(int i = 0; i < nbr_calls; i ++) {
        reduce3(v_test, result);
    }
    PE.stop();

    cout<<"TEST reduce3: "<<std::endl;
    cout<<"nombre d'instructions total: "<<size * nbr_calls<<std::endl;
    cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    cout<<"CPI= "<<PE.CPI(size * nbr_calls)<<std::endl;
    cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    cout<<std::endl;


    PE.start();
    for(int i = 0; i < nbr_calls; i ++) {
        reduce4(v_test, result);
    }
    PE.stop();

    cout<<"TEST reduce4: "<<std::endl;
    cout<<"nombre d'instructions total: "<<2 * size * nbr_calls<<std::endl;
    cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    cout<<"CPI= "<<PE.CPI(2 * size * nbr_calls)<<std::endl;
    cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    cout<<std::endl;


    PE.start();
    for(int i = 0; i < nbr_calls; i ++) {
        reduce5(v_test, result);
    }
    PE.stop();

    cout<<"TEST reduce5: "<<std::endl;
    cout<<"nombre d'instructions total: "<<2 * size * nbr_calls<<std::endl;
    cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    cout<<"CPI= "<<PE.CPI(4 * size * nbr_calls)<<std::endl;
    cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    cout<<std::endl;


    PE.start();
    for(int i = 0; i < nbr_calls; i ++) {
        reduce6(v_test, result);
    }
    PE.stop();

    cout<<"TEST reduce6: "<<std::endl;
    cout<<"nombre d'instructions total: "<<2 * size * nbr_calls<<std::endl;
    cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    cout<<"CPI= "<<PE.CPI(10 * size * nbr_calls)<<std::endl;
    cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    cout<<std::endl;


    PE.start();
    for(int i = 0; i < nbr_calls; i ++) {
        reduce7(v_test, result);
    }
    PE.stop();

    cout<<"TEST reduce7: "<<std::endl;
    cout<<"nombre d'instructions total: "<<2 * size * nbr_calls<<std::endl;
    cout<<"nbr cycles: "  <<PE.nb_cycle()<<std::endl;
    cout<<"Temps d'executions (par appel de fonction): "<<std::endl;
    cout<<"nbr secondes: "  <<PE.second()<<std::endl;
    cout<<"nbr millisecondes: " <<PE.millisecond()<<std::endl;
    cout<<"CPI= "<<PE.CPI(12 * size * nbr_calls)<<std::endl;
    cout<<"Temps d'exécution total:"<<PE.second()<<std::endl;
    cout<<std::endl;


    

    return 0;
}