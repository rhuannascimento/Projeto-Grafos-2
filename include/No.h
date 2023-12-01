#ifndef NO_H
#define NO_H
#include <iostream>
#include "Aresta.h"

using namespace std;
class Aresta;

class No {
private:
    int idNo;
    int demanda;
    int x;
    int y;
    No *proxNo = nullptr;
    Aresta *primeiraAresta = nullptr;
    Aresta *ultimaAresta = nullptr;

public:

    No(int idNo, int x, int y);
    ~No();

    int getIdNo(){return idNo;};
    int getDemanda(){return demanda;};
    int getX(){return x;};
    int getY(){return y;};
    No * getProxNo(){return proxNo;};
    Aresta * getPrimeiraAresta(){return primeiraAresta;}
    Aresta * getUltimaAresta(){return ultimaAresta;}

    void setProxNo(No *no);
    Aresta * insertAresta(int destino, int peso);
};

#endif // NO_H