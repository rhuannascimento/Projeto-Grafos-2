#ifndef PROBLEMA_H
#define PROBLEMA_H
#include <iostream>
#include <vector>
#include "Grafo.h"

using namespace std;
class Grafo;

class Problema {
private:
    
    int numCaminhoes;
    int solucaoOtima;
    int dimensao;
    int capacidadeDepostio;
    vector<vector <int>> matrizDistancia;
    Grafo *g = nullptr;

public:
    
    Problema(string arquivo);
    ~Problema();

    void calculaMAtrizDitancia();
    void insereArestas();

    int getNumCaminhoes(){return numCaminhoes;};
    int getSolucaoOtima(){return solucaoOtima;};
    int getDimensao(){return dimensao;};
    int getCapacidadeDepostio(){return capacidadeDepostio;};
    vector<vector <int>> getMatrizDistancia(){return matrizDistancia;};
    Grafo *getGrafo(){return g;};
    
};

#endif // PROBLEMA_H