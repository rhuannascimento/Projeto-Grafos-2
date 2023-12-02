#ifndef SOLCUAO_H
#define SOLUCAO_H
#include <iostream>
#include <vector>
#include <limits>
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

public:
    
    Solucao(Problema *p);
    ~Solucao();

    vector<vector<int>> guloso();
    float getComparacao(){return comparacao;};
    //vector<Aresta> gulosoAdptativo();
    //vector<Aresta> gulosoReativo();
    
};

#endif // SOLCUAO_H