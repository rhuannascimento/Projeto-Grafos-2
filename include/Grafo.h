#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>
#include "No.h"
#include "Aresta.h"

using namespace std;
class No;
class Aresta;

class Grafo {
private:
    
    No *raiz = nullptr;
    No *ultimoNo = nullptr;
    int ordem;

public:
    Grafo();
    ~Grafo();

    No *insereNo(int idNo, int x, int y);
    No *buscaNo(int idNo);
    void imprimeGrafo();

};

#endif // ARESTA_H