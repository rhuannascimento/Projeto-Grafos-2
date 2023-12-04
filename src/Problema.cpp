#include "../include/Problema.h"

using namespace std;

Problema::Problema(string nomeArquivo)
{

    this->g = new Grafo();
    this->auxLer(nomeArquivo);
    // this->g->imprimeGrafo();
    this->calculaMatrizDitancia();
}

void Problema::auxLer(string nomeArquivo)
{
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string linha;
    string chave, valor;
    bool leituraCoords = false;
    bool leituraDemanda = false;
    bool leituraDeposito = false;

    while (getline(arquivo, linha))
    {
        istringstream iss(linha);

        if (!linha.empty())
        {
            iss >> chave;

            if (chave == "COMMENT" || chave == "DIMENSION" || chave == "CAPACITY")
            {
                iss >> valor;
                iss >> valor;
                if (chave == "COMMENT")
                {

                    while (iss >> valor)
                    {
                        if (valor == "trucks:")
                        {
                            iss >> valor;
                            this->numCaminhoes = stoi(valor);
                        }
                        else if (valor == "value:")
                        {
                            iss >> valor;
                            this->solucaoOtima = stoi(valor);
                        }
                    }
                }
                else if (chave == "DIMENSION")
                {
                    this->dimensao = stoi(valor);
                }
                else if (chave == "CAPACITY")
                {
                    this->capacidadeCaminhao = stoi(valor);
                }
            }
            else if (chave == "NODE_COORD_SECTION")
            {
                leituraCoords = true;
            }
            else if (chave == "DEMAND_SECTION")
            {
                leituraCoords = false;
                leituraDemanda = true;
            }
            else if (chave == "DEPOT_SECTION")
            {
                leituraDemanda = false;
                leituraDeposito = true;
            }
            else if (leituraCoords)
            {
                int id;
                float x, y;
                id = stoi(chave);
                iss >> x >> y;
                this->g->insereNo(id, x, y);
            }
            else if (leituraDemanda)
            {
                int id, demanda;
                id = stoi(chave);
                iss >> demanda;
                No *no = this->g->buscaNo(id);
                no->setDemanda(demanda);
            }
            else if (leituraDeposito)
            {
                int id;
                id = stoi(chave);
                No *no = this->g->buscaNo(id);
                no->setDeposito();
                leituraDeposito = false;
            }
        }
    }

    arquivo.close();
}

float Problema::calcularDistanciaEuclidiana(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Problema::calculaMatrizDitancia()
{

    this->matrizDistancia = vector<vector<float>>(this->dimensao, vector<float>(this->dimensao));

    No *atual = this->g->getRaiz();

    while (atual != nullptr)
    {
        No *comparacao = this->g->getRaiz();
        while (comparacao != nullptr)
        {
            if (comparacao->getIdNo() != atual->getIdNo())
            {
                this->matrizDistancia[atual->getIdNo() - 1][comparacao->getIdNo() - 1] = this->calcularDistanciaEuclidiana(atual->getX(), atual->getY(), comparacao->getX(), comparacao->getY());
            }
            else
            {
                this->matrizDistancia[atual->getIdNo() - 1][comparacao->getIdNo() - 1] = 0.0;
            }
            comparacao = comparacao->getProxNo();
        }
        atual = atual->getProxNo();
    }
}