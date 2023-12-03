#include "../include/Solucao.h"

Solucao::Solucao(Problema *p){
    this->p = p;
}

struct Rota {
    vector<int> nos;
    int capacidadeAtual;
};

vector<vector<int>> Solucao::guloso() {
    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    Grafo *grafo = p->getGrafo();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();

    vector<Rota> rotas(numCaminhoes);

    No *deposito = grafo->getRaiz();
    deposito->setVisitado(true);

    for (int i = 0; i < numCaminhoes; i++) {
        rotas[i].nos.push_back(deposito->getIdNo()); 
        rotas[i].capacidadeAtual = capacidadeCaminhao; 
    }

    for (int i = 0; i < numCaminhoes; i++) {
        No *noAtual = deposito; 


        int custoDaRota = 0;
        while (true) {
            float menorDistancia = numeric_limits<float>::max();
            int proxNo = -1;
            
            for (int j = 1; j <= p->getDimensao(); j++) {
                No *possivelProximoNo = grafo->buscaNo(j);
                if (!possivelProximoNo->getVisitado() && matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1] < menorDistancia &&
                    possivelProximoNo->getDemanda() <= rotas[i].capacidadeAtual) {
                    menorDistancia = matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1];
                    proxNo = possivelProximoNo->getIdNo();
                    custoDaRota += menorDistancia;
                }
            }

            if (proxNo == -1) {
                rotas[i].nos.push_back(deposito->getIdNo());
                break;
            }

            No *proximoNo = grafo->buscaNo(proxNo);
            proximoNo->setVisitado(true);
            rotas[i].nos.push_back(proximoNo->getIdNo());
            rotas[i].capacidadeAtual -= proximoNo->getDemanda();
            noAtual = proximoNo;
        }

        rotas[i].nos.push_back(rotas[i].capacidadeAtual);
        rotas[i].nos.push_back(custoDaRota);
        this->custoTotal += custoDaRota;
 
    }

    vector<vector<int>> resultado;
    for (const auto &rota : rotas) {
        resultado.push_back(rota.nos);
    }

    No *n = p->getGrafo()->getRaiz();

    while(n != nullptr){
        if(n->getVisitado() == false){
            cout<<"Solução invalida, nó "<< n->getIdNo() << " não visistado!" <<endl;
        }
        n = n->getProxNo();
    }
 
    this->resultado = resultado;

    return this->resultado;
}


vector<vector<int>> Solucao::gulosoReativo() {
    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    Grafo *grafo = p->getGrafo();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();

    vector<Rota> rotas(numCaminhoes);

    No *deposito = grafo->getRaiz();
    deposito->setVisitado(true);

    // Definição dos parâmetros do algoritmo reativo
    const int NUM_ITERACOES = 10; // Número de iterações
    const int NUM_CANDIDATOS = 5; // Número de candidatos a serem avaliados
    vector<float> feromonio(p->getDimensao(), 1.0); // Nível inicial de feromônio para cada nó

    // Iterações do algoritmo reativo
    for (int iteracao = 0; iteracao < NUM_ITERACOES; iteracao++) {
        // Construção das rotas para cada caminhão
        for (int i = 0; i < numCaminhoes; i++) {
            rotas[i].nos.push_back(deposito->getIdNo());
            rotas[i].capacidadeAtual = capacidadeCaminhao;

            No *noAtual = deposito;
            int custoDaRota = 0;

            while (true) {
                vector<pair<int, float>> candidatos; // Armazena os candidatos a serem avaliados

                // Encontre os nós candidatos para visitar
                for (int j = 1; j <= p->getDimensao(); j++) {
                    No *possivelProximoNo = grafo->buscaNo(j);

                    if (!possivelProximoNo->getVisitado() && possivelProximoNo->getDemanda() <= rotas[i].capacidadeAtual) {
                        float feromProb = feromonio[possivelProximoNo->getIdNo() - 1]; // Nível de feromônio do nó
                        float dist = matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1];
                        candidatos.push_back(make_pair(possivelProximoNo->getIdNo(), feromProb / dist));
                    }
                }

                // Escolha o próximo nó com base na probabilidade ponderada do feromônio
                if (!candidatos.empty()) {
                    sort(candidatos.begin(), candidatos.end(), [](const pair<int, float> &a, const pair<int, float> &b) {
                        return a.second > b.second;
                    });

                    int proxNo = candidatos[0].first;
                    No *proximoNo = grafo->buscaNo(proxNo);
                    proximoNo->setVisitado(true);
                    rotas[i].nos.push_back(proximoNo->getIdNo());
                    rotas[i].capacidadeAtual -= proximoNo->getDemanda();
                    custoDaRota += matrizDistancia[noAtual->getIdNo() - 1][proximoNo->getIdNo() - 1];
                    noAtual = proximoNo;
                } else {
                    rotas[i].nos.push_back(deposito->getIdNo());
                    break;
                }
            }

            rotas[i].nos.push_back(rotas[i].capacidadeAtual);
            rotas[i].nos.push_back(custoDaRota);
            this->custoTotal += custoDaRota;
        }

        // Atualização do feromônio após construção das rotas
        for (const auto &rota : rotas) {
            float deltaFeromonio = 1.0 / rota.nos[rota.nos.size() - 1];
            for (int i = 0; i < rota.nos.size() - 2; i++) {
                int noAtual = rota.nos[i];
                int proxNo = rota.nos[i + 1];
                feromonio[noAtual - 1] += deltaFeromonio;
                feromonio[proxNo - 1] += deltaFeromonio;
            }
        }
    }

    // Construção do resultado final
    vector<vector<int>> resultado;
    for (const auto &rota : rotas) {
        resultado.push_back(rota.nos);
    }

    // Verificação da validade da solução
    No *n = p->getGrafo()->getRaiz();
    while (n != nullptr) {
        if (n->getVisitado() == false) {
            cout << "Solução inválida, nó " << n->getIdNo() << " não visitado!" << endl;
        }
        n = n->getProxNo();
    }

    this->resultado = resultado;
    return this->resultado;
}