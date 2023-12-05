#include "../include/Valida.h"

Valida::Valida(vector<vector<int>> resultado, Problema *p){
    this->resultado = resultado;
    this->p = p;
}


bool Valida::validar(){
    this->validarTodosPresente();
    this->validarUnicoDeRota();
    if(this->atendeuTodos &&  this->unicoEmRota){
        return true;
    }

    return false;
}


bool Valida::validarTodosPresente(){
    Grafo *g = this->p->getGrafo();
    vector<vector<int>> resultado = this->resultado;

    No *no = g->getRaiz();
    bool encontrado;

    while (no != nullptr)
    {
        encontrado = false;
        for (int i = 0; i < resultado.size(); i++)
        {
            for (int j = 0; j < resultado[i].size() - 2; j++)
            {
                if(no->getIdNo() == resultado[i][j]){
                    encontrado = true;
                    break;
                }
            }
            if(encontrado){
                break;
            }
        }

        if(!encontrado){
            return false;
        }

        no = no->getProxNo();
        
    }
    

    this->atendeuTodos = true;
    return true;

}


bool Valida::validarUnicoDeRota(){
    Grafo *g = this->p->getGrafo();
    vector<vector<int>> resultado = this->resultado;

    int contador = 0;
    No *no = g->getRaiz();


    while (no != nullptr)
    {
        contador = 0;
        for (int i = 0; i < resultado.size(); i++)
        {
            for (int j = 0; j < resultado[i].size() - 2; j++)
            {
                if(no->getIdNo() == resultado[i][j] && !no->getDeposito()){
                    contador++;
                }
                if(contador > 1){
                    return false;
                }
            }
        }
        no = no->getProxNo();
    }

    this->unicoEmRota = true;
    return true;

}