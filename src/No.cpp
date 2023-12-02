#include "../include/No.h"

No::No(int id, float x, float y){
    this->idNo = id;
    this->x = x;
    this->y = y;
}

void No::setProxNo(No *no){
    this->proxNo = no;
}

void No::setDemanda(int demanda){
    this->demanda = demanda;
}

void No::setDeposito(){
    this->deposito = true;
}