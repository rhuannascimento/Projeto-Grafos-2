#include <iostream>
#include "./include/Problema.h"
#include "./include/Solucao.h"
#include <string>

using namespace std;

int main()
{

    for (int x = 1; x <= 10; x++)
    {
        cout<<"==========================================================Instancia " << x <<"==============================================================================================="<<endl;
        Problema *p = new Problema("instancias/"+to_string(x)+".txt");
        Solucao *s = new Solucao(p);

        vector<vector<int>> rotas = s->guloso();

        for (int i = 0; i < rotas.size(); ++i)
        {
            if (i == rotas.size() - 1)
            {
                cout << "Custo: ";
            }
            else
            {
                cout << "Rota " << i + 1 << ": ";
            }
            for (int j = 0; j < rotas[i].size(); ++j)
            {
                if (j == rotas[i].size() - 2 && !(i == rotas.size() - 1))
                {
                    cout << "Ditancia: " << rotas[i][j] << " ";
                }
                else if (j == rotas[i].size() - 1 && !(i == rotas.size() - 1))
                {
                    cout << "Carga total da rota: " << rotas[i][j] << " ";
                }
                else
                {
                    cout << rotas[i][j] << " ";
                }
            }
            cout << endl;
        }

        cout << "O resultado otimo é " << p->getSolucaoOtima() << endl;
        cout << "O resultado atual é " << s->getComparacao() << " vezes pior que a solução ótima!" << endl << endl << endl;
    }
    return 0;
}