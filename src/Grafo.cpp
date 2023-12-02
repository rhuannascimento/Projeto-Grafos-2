#include "../include/Grafo.h"


Grafo::Grafo(){
    
}

No *Grafo::insereNo(int idNo, int x, int y){

    No *novoNo = new No(idNo, x, y);

    if(this->raiz == nullptr){
        this->raiz = novoNo;
        this->ultimoNo = this->raiz;
        return this->raiz;
    }

    this->ultimoNo->setProxNo(novoNo);
    this->ultimoNo = novoNo;

    return this->ultimoNo;
}


void Grafo::imprimeGrafo(){
    No *no = this->raiz;
    while(no != nullptr){
        cout<< no->getIdNo() << " ";
        no = no->getProxNo();
    }
}
