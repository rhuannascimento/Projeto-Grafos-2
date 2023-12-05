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
                    
                }
            }

            

 if (proxNo == -1) {
                custoDaRota += matrizDistancia[noAtual->getIdNo() - 1][deposito->getIdNo() - 1];
                rotas[i].nos.push_back(deposito->getIdNo());
                break;
            }

            custoDaRota += menorDistancia;

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




vector<vector<int>> Solucao::gulosoReativo(float parametroAlpha, float parametroProbabilidadeInicial, int parametroTamanhoListaCandidatos) {
    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    Grafo *grafo = p->getGrafo();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();

    vector<Rota> rotas(numCaminhoes);

    No *deposito = grafo->getRaiz();
    deposito->setVisitado(true);

    float alpha = parametroAlpha; // Taxa de aprendizado
    float probabilidadeInicial = parametroProbabilidadeInicial; // Probabilidade inicial de escolha
    int tamanhoListaCandidatos = parametroTamanhoListaCandidatos; // Tamanho da lista de candidatos

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

            vector<int> candidatos;

            for (int j = 1; j <= p->getDimensao(); j++) {
                No *possivelProximoNo = grafo->buscaNo(j);
                if (!possivelProximoNo->getVisitado() && possivelProximoNo->getDemanda() <= rotas[i].capacidadeAtual) {
                    candidatos.push_back(possivelProximoNo->getIdNo());
                }
            }

            // Embaralhar a lista de candidatos
            random_shuffle(candidatos.begin(), candidatos.end());

            int contadorCandidatos = 0;
            float probabilidade = probabilidadeInicial;

            // Escolha adaptativa reativa
            while (contadorCandidatos < tamanhoListaCandidatos && contadorCandidatos < candidatos.size()) {
                float prob = (float)rand() / RAND_MAX;
                if (prob < probabilidade) {
                    No *possivelProximoNo = grafo->buscaNo(candidatos[contadorCandidatos]);
                    float distancia = matrizDistancia[noAtual->getIdNo() - 1][possivelProximoNo->getIdNo() - 1];
                    if (distancia < menorDistancia) {
                        menorDistancia = distancia;
                        proxNo = possivelProximoNo->getIdNo();
                        custoDaRota += menorDistancia;
                    }
                }
                contadorCandidatos++;
                probabilidade = (1 - alpha) * probabilidade + alpha * (1 - probabilidade);
            }

            if (proxNo == -1) {
                rotas[i].nos.push_back(deposito->getIdNo());
                break;
            }

            custoDaRota += menorDistancia;

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

int randomRange(int min, int max)
{
    return min + rand() % (max - min + 1);
}

vector<vector<int>> Solucao::gulosoAdptativo(float alfa, int numIter)
{
    vector<vector<int>> solBest;
    float custoBest = numeric_limits<float>::max(); 

    int numCaminhoes = p->getNumCaminhoes();
    int capacidadeCaminhao = p->getCapacidadeCaminhao();
    vector<vector<float>> matrizDistancia = p->getMatrizDistancia();
    Grafo *grafo = p->getGrafo();

    for (int i = 0; i < numIter; ++i)
    {
        vector<int> clientesRestantes; 
        for (int j = 2; j <= p->getDimensao(); ++j)
        {
            clientesRestantes.push_back(j);
        }

        vector<vector<int>> rotas;

        for (int caminhao = 0; caminhao < numCaminhoes; ++caminhao)
        {
            float capacidadeAtual = capacidadeCaminhao;
            vector<int> rota;
            int custoDaRota = 0;
            int atual = 1; 
            rota.push_back(1);

            while (!clientesRestantes.empty())
            {
                vector<pair<float, int>> candidatosViaveis; 
                for (int cliente : clientesRestantes)
                {
                    if (capacidadeAtual - grafo->buscaNo(cliente)->getDemanda() >= 0)
                    {
                        candidatosViaveis.push_back(make_pair(matrizDistancia[cliente - 1][rota[rota.size() - 1] - 1], cliente));
                    }
                }

                if (!candidatosViaveis.empty())
                {
                    sort(candidatosViaveis.begin(), candidatosViaveis.end(),
                              [](const auto &a, const auto &b)
                              {
                                  return a.first < b.first;
                              });

                    int k = randomRange(0, alfa * (candidatosViaveis.size() - 1));

                    int escolha = candidatosViaveis[k].second;

                    rota.push_back(escolha);

                    atual = escolha;
                    capacidadeAtual -= grafo->buscaNo(escolha)->getDemanda();

                    clientesRestantes.erase(find(clientesRestantes.begin(), clientesRestantes.end(), escolha));
                }
                else
                {
                    break; 
                }
            }

            rota.push_back(1);
            rota.push_back(capacidadeAtual);
            rotas.push_back(rota);
        }

        int custoTotal = 0;
        for (auto &rota : rotas)
        {
            int custoRota = 0;
            for (size_t i = 1; i < rota.size() - 1; ++i)
            {
                custoRota += matrizDistancia[rota[i - 1] - 1][rota[i] - 1];
            }
            rota.push_back(custoRota);
            custoTotal += custoRota;
        }

        Valida *v = new Valida(rotas, this->p);

        bool valido = v->validar();

        if (custoTotal < custoBest && valido)
        {
            solBest = rotas;
            custoBest = custoTotal;
            this->custoTotal = custoBest;
        }
    }

    return solBest;
}
