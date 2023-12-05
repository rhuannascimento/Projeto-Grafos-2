#include "../include/Solucao.h"

Solucao::Solucao(Problema *p){
    this->p = p;
}


struct Rota
{
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

vector<vector<int>> Solucao::gulosoAdptativo(float alfa, int numIter) {
    vector<vector<int>> solBest; // Melhor solução encontrada
    float custoBest = numeric_limits<float>::max(); // Custo da melhor solução

    // Obtendo informações do problema
    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();
    Grafo* grafo = p->getGrafo();

    // Iterações do algoritmo
    for (int i = 0; i < numIter; ++i) {
        vector<int> clientesRestantes; // Lista de clientes não visitados
        for (int j = 2; j <= p->getDimensao(); ++j) {
                clientesRestantes.push_back(j);
        }

        vector<vector<int>> rotas; // Armazenar as rotas dos caminhões

        // Construir rotas para cada caminhão
        for (int caminhao = 0; caminhao < numCaminhoes; ++caminhao) {
            float capacidadeAtual = capacidadeCaminhao;
            vector<int> rota;
            int custoDaRota = 0;
            int atual = 1; // Começar do depósito

            rota.push_back(1);

            // Construir uma rota
            while (!clientesRestantes.empty()) {
                vector<int> candidatosViaveis; // Candidatos viáveis para próxima visita
                for (int cliente : clientesRestantes) {
                    if (capacidadeAtual - grafo->buscaNo(cliente)->getDemanda() >= 0) {
                        candidatosViaveis.push_back(cliente);
                    }
                }

                if (!candidatosViaveis.empty()) {
                    // Calcula probabilidades para os candidatos
                    random_shuffle(candidatosViaveis.begin(), candidatosViaveis.end());
                    int k = static_cast<int>(alfa * candidatosViaveis.size());
                    int escolha = candidatosViaveis[k];

                    // Atualiza rota e capacidade
                    rota.push_back(escolha);
    
                    atual = escolha;
                    capacidadeAtual -= grafo->buscaNo(escolha)->getDemanda();

                    // Remove cliente da lista de não visitados
                    clientesRestantes.erase(find(clientesRestantes.begin(), clientesRestantes.end(), escolha));
                } else {
                    break; // Não há mais clientes viáveis para essa rota
                }
            }

            // Adiciona o depósito no final da rota
            rota.push_back(1);
            rota.push_back(capacidadeAtual);
            rotas.push_back(rota);
        }

        // Calcular custo total das rotas
        int custoTotal = 0;
        for (auto& rota : rotas) {
            int custoRota = 0;
            for (size_t i = 1; i < rota.size() - 1; ++i) {
                custoRota += matrizDistancia[rota[i - 1] - 1][rota[i] - 1];          
            }
            rota.push_back(custoRota);
            custoTotal += custoRota;
        }

        Valida *v = new Valida(rotas, this->p);

        bool valido = v->validar();

        // Atualizar a melhor solução encontrada
        if (custoTotal < custoBest && valido) {
            solBest = rotas;
            custoBest = custoTotal;
            this->custoTotal = custoBest;
        }
    }

    return solBest;
}

  