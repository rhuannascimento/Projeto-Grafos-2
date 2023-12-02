#include "../include/Problema.h"

using namespace std;


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
    int noOfTrucks = 0, optimalValue = 0, dimension = 0, capacity = 0;
    bool leituraCoords = false;

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
                            noOfTrucks = stoi(valor);
                        }
                        else if (valor == "value:")
                        {
                            iss >> valor;
                            optimalValue = stoi(valor);
                        }
                    }
                }
                else if (chave == "DIMENSION")
                {
                    dimension = stoi(valor);
                }
                else if (chave == "CAPACITY")
                {
                    capacity = stoi(valor);
                }
            }
            else if (chave == "NODE_COORD_SECTION")
            {
                leituraCoords = true;
            }
            else if (chave == "DEMAND_SECTION")
            {
                leituraCoords = false;
            }
            else if (leituraCoords)
            {
                int id, x, y;
                id = stoi(chave);
                iss >> x >> y;  
                this->g->insereNo(id, x, y);
            }
        }
    }

    arquivo.close();
}

Problema::Problema(string nomeArquivo)
{

    this->g = new Grafo();
    this->auxLer(nomeArquivo);
    this->g->imprimeGrafo();
    
}
