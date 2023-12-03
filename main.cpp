#include <iostream>
#include "./include/Problema.h"
#include "./include/Solucao.h"
#include <string>

using namespace std;

int main()
{

    for (int x = 1; x <= 10; x++)
    {
        cout << "==========================================================Instancia " << x << "===============================================================================================" << endl;
        Problema *p = new Problema("instancias/" + to_string(x) + ".txt");
        Solucao *s = new Solucao(p);

        vector<vector<int>> rotas = s->guloso();

        for (size_t i = 0; i < rotas.size(); ++i)
        {
            cout << "Rota " << i + 1 << ": ";
            for (size_t j = 0; j < rotas[i].size(); ++j)
            {
                if(j ==  rotas[i].size() - 2){
                    cout<<"Capacidade sobrando: ";
                }else if(j ==  rotas[i].size() - 1){
                    cout<<"Custo da rota: ";
                }
                
                cout << rotas[i][j] << " ";
                
            }
            cout << endl;
        }
        cout<<"Custo Total: "<<s->getCustoTotal()<<endl;
        cout<<"Solucao Otimo: "<<p->getSolucaoOtima()<<endl;
    }
    return 0;
}