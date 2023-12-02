#include "../include/Solucao.h"

Solucao::Solucao(Problema *p){
    this->p = p;
}

vector<vector<int>> Solucao::guloso() {
    int capacidade = p->getCapacidadeCaminhao();
    int numCaminhoes = p->getNumCaminhoes();
    int dimensao = p->getDimensao();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();

    vector<bool> visitado(dimensao, false);
    vector<vector<int>> rotas(numCaminhoes + 1, vector<int>());

    float custoTotal = 0.0;

    for (int i = 0; i < numCaminhoes; ++i) {
        int cargaAtual = 0;
        int atual = 0; 

        float custoAtual = 0;
        while (true) {
            visitado[atual] = true;
            rotas[i].push_back(atual + 1);
            cargaAtual += p->getGrafo()->buscaNo(atual + 1)->getDemanda();
            //rotas[i].push_back(cargaAtual);

            int melhorVizinho = -1;
            float menorDistancia = numeric_limits<float>::max();

            for (int j = 0; j < dimensao; ++j) {
                if (!visitado[j] && matrizDistancia[atual][j] < menorDistancia &&
                    cargaAtual + p->getGrafo()->buscaNo(j+1)->getDemanda() < capacidade) {
                    menorDistancia = matrizDistancia[atual][j];
                    melhorVizinho = j;
                }
            }

            if (melhorVizinho == -1) {
                custoTotal += matrizDistancia[atual][0];
                rotas[i].push_back(1);
                rotas[i].push_back(custoAtual);
                rotas[i].push_back(cargaAtual);
                break;
            }

            custoAtual += menorDistancia;
            custoTotal += menorDistancia; 
            atual = melhorVizinho;
            
        }
    }

    for(int i = 0; i<dimensao; i++){
        if(visitado[i] == false){
            cout<<"Solução invalida! "<< "Nó " << i+1 << " não incluido!"<<endl;
        }
    }

   
    this->comparacao = (custoTotal - this->p->getSolucaoOtima())/this->p->getSolucaoOtima();

    rotas[numCaminhoes].push_back(custoTotal);
    

    return rotas;
}