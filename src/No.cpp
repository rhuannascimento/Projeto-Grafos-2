#include "../include/No.h"

No::No(int id, int x, int y){
    this->idNo = id;
    this->x = x;
    this->y = y;
}

void No::setProxNo(No *no){
    this->proxNo = no;
}