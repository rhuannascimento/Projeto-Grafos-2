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
    bool deposito = false;
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
    int getDeposito(){return deposito;};
    int getX(){return x;};
    int getY(){return y;};
    No * getProxNo(){return proxNo;};
    Aresta * getPrimeiraAresta(){return primeiraAresta;}
    Aresta * getUltimaAresta(){return ultimaAresta;}

    void setProxNo(No *no);
    void setDemanda(int demanda);
    void setDeposito();
    Aresta * insertAresta(int destino, int peso);
};

#endif // NO_H