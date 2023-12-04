#ifndef SOLUCAO_H
#define SOLUCAO_H
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <random>
//#include "Aresta.h"
#include "Problema.h"
#include "No.h"

using namespace std;
class Problema;
class No;
//class Aresta;

class Solucao {
private:
    
    //vector<Aresta> resultado;
    Problema *p;
    float comparacao;
    float custoTotal = 0;
    vector<vector<int>> resultado;

public:
    
    Solucao(Problema *p);
    ~Solucao();

    vector<vector<int>> guloso();
    vector<vector<int>> getResultado(){return resultado;};
    float getComparacao(){return comparacao;};
    float getCustoTotal(){return custoTotal;};
    //vector<Aresta> gulosoAdptativo();
    vector<vector<int>> gulosoReativo(float parametroAlpha, float parametroProbabilidadeInicial, int parametroTamanhoListaCandidatos);
    
};

#endif // SOLUCAO_H