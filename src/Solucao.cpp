#include "../include/Solucao.h"

Solucao::Solucao(Problema *p)
{
    this->p = p;
}

struct Rota
{
    vector<int> nos;
    int capacidadeAtual;
};

vector<vector<int>> Solucao::guloso()
{
    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    Grafo *grafo = p->getGrafo();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();

    vector<Rota> rotas(numCaminhoes);

    No *deposito = grafo->getRaiz();
    deposito->setVisitado(true);

    for (int i = 0; i < numCaminhoes; i++)
    {
        rotas[i].nos.push_back(deposito->getIdNo());
        rotas[i].capacidadeAtual = capacidadeCaminhao;
    }

    for (int i = 0; i < numCaminhoes; i++)
    {
        No *noAtual = deposito;

        int custoDaRota = 0;
        while (true)
        {
            float menorDistancia = numeric_limits<float>::max();
            int proxNo = -1;

            for (int j = 1; j <= p->getDimensao(); j++)
            {
                No *possivelProximoNo = grafo->buscaNo(j);
                if (!possivelProximoNo->getVisitado() && matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1] < menorDistancia &&
                    possivelProximoNo->getDemanda() <= rotas[i].capacidadeAtual)
                {
                    menorDistancia = matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1];
                    proxNo = possivelProximoNo->getIdNo();
                    custoDaRota += menorDistancia;
                }
            }

            if (proxNo == -1)
            {
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
    for (const auto &rota : rotas)
    {
        resultado.push_back(rota.nos);
    }

    No *n = p->getGrafo()->getRaiz();

    while (n != nullptr)
    {
        if (n->getVisitado() == false)
        {
            cout << "Solução invalida, nó " << n->getIdNo() << " não visistado!" << endl;
        }
        n = n->getProxNo();
    }

    this->resultado = resultado;

    return this->resultado;
}


/*
vector<vector<int>> Solucao::gulosoRandomizadoAdaptativo()
{
    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    Grafo *grafo = p->getGrafo();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();

    vector<Rota> rotas(numCaminhoes);

    No *deposito = grafo->getRaiz();
    deposito->setVisitado(true);

    for (int i = 0; i < numCaminhoes; i++)
    {
        rotas[i].nos.push_back(deposito->getIdNo());
        rotas[i].capacidadeAtual = capacidadeCaminhao;
    }

    for (int i = 0; i < numCaminhoes; i++)
    {
        No *noAtual = deposito;

        int custoDaRota = 0;
        while (true)
        {
            vector<No *> nosPossiveis;

            for (int j = 1; j <= p->getDimensao(); j++)
            {
                No *possivelProximoNo = grafo->buscaNo(j);
                if (!possivelProximoNo->getVisitado())
                {
                    nosPossiveis.push_back(possivelProximoNo);
                }
            }

            if (nosPossiveis.empty())
            {
                rotas[i].nos.push_back(deposito->getIdNo());
                break;
            }

            No *proximoNo = nosPossiveis[static_cast<int>(getAleatorio() * nosPossiveis.size())];
            proximoNo->setVisitado(true);
            rotas[i].nos.push_back(proximoNo->getIdNo());
            noAtual = proximoNo;
        }

        rotas[i].nos.push_back(rotas[i].capacidadeAtual);
        rotas[i].nos.push_back(custoDaRota);
        this->custoTotal += custoDaRota;
    }

    vector<vector<int>> resultado;
    for (const auto &rota : rotas)
    {
        resultado.push_back(rota.nos);
    }

    No *n = p->getGrafo()->getRaiz();

    while (n != nullptr)
    {
        if (n->getVisitado() == false)
        {
            cout << "Solução invalida, nó " << n->getIdNo() << " não visistado!" << endl;
        }
        n = n->getProxNo();
    }

    this->resultado = resultado;

    return this->resultado;
}
*/

vector<vector<int>> Solucao::gulosoReativo(float parametroReativo) {
    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    Grafo *grafo = p->getGrafo();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();

    vector<Rota> rotas(numCaminhoes);

    No *deposito = grafo->getRaiz();
    deposito->setVisitado(true);

    float alfa = parametroReativo; // Fator de influência da atualização da probabilidade
    vector<float> probabilidades(p->getDimensao(), 1.0); // Inicialmente, todas as probabilidades são iguais

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
            float somaProbabilidades = 0.0;

            for (int j = 1; j <= p->getDimensao(); j++) {
                No *possivelProximoNo = grafo->buscaNo(j);
                if (!possivelProximoNo->getVisitado() && possivelProximoNo->getDemanda() <= rotas[i].capacidadeAtual) {
                    menorDistancia = matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1];
                    somaProbabilidades += probabilidades[j - 1];
                }
            }

            float escolhaAleatoria = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * somaProbabilidades;

            float acumuladorProbabilidades = 0.0;
            for (int j = 1; j <= p->getDimensao(); j++) {
                No *possivelProximoNo = grafo->buscaNo(j);
                if (!possivelProximoNo->getVisitado() && possivelProximoNo->getDemanda() <= rotas[i].capacidadeAtual) {
                    menorDistancia = matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1];
                    acumuladorProbabilidades += probabilidades[j - 1];
                    if (acumuladorProbabilidades >= escolhaAleatoria) {
                        proxNo = possivelProximoNo->getIdNo();
                        custoDaRota += menorDistancia;
                        break;
                    }
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

            for (int j = 1; j <= p->getDimensao(); j++) {
                No *possivelProximoNo = grafo->buscaNo(j);
                if (!possivelProximoNo->getVisitado() && possivelProximoNo->getDemanda() <= rotas[i].capacidadeAtual) {
                    float delta = matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1] / custoDaRota;
                    probabilidades[j - 1] = (1 - alfa) * probabilidades[j - 1] + alfa * delta;
                }
            }
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
            cout<<"Solução inválida, nó "<< n->getIdNo() << " não visitado!" <<endl;
        }
        n = n->getProxNo();
    }
 
    this->resultado = resultado;

    return this->resultado;
}





