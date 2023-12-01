#ifndef SOLCUAO_H
#define SOLUCAO_H
#include <iostream>
#include <vector>
#include "Aresta.h"
#include "Problema.h"

using namespace std;

class Solucao {
private:
    
    vector<Aresta> resultado;
    Problema p;
    int comparacao;

public:
    
    Solucao(Problema p);
    ~Solucao();

    vector<Aresta> guloso();
    vector<Aresta> gulosoAdptativo();
    vector<Aresta> gulosoReativo();
    
};

#endif // SOLCUAO_H