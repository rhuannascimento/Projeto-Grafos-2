#include <iostream>
#include "./include/Problema.h"
#include "./include/Solucao.h"
#include "./include/Valida.h"
#include <string>

using namespace std;

int main()
{
    int menu = 0;
    

    while (menu != 9)
    {
        cout << "====================================================================================================================================================" << endl;
        cout << "escolha uma opcao de algoritmo:" << endl;
        cout << "|1| Algoritmo guloso" << endl;
        cout << "|2| Algoritmo Guloso randomizado adaptativo" << endl;
        cout << "|3| Algoritmo Guloso randomizado adaptativo reativo" << endl;
        cout << "|9| Sair" << endl;
        cin >> menu;
        if (menu!=9)
        {

            for (int x = 1; x <= 10; x++)
            {
                cout << "==========================================================Instancia " << x << "===============================================================================================" << endl;
                Problema *p = new Problema("instancias/" + to_string(x) + ".txt");
                Solucao *s = new Solucao(p);
                vector<vector<int>> rotas;
                switch (menu)
                {
                case 1:
                    rotas = s->guloso();
                    break;
                case 2:         
                        rotas =  s->gulosoAdptativo(0.37, 10);
                    break;
                case 3:
                    vector<float> alfas = {0.1, 0.7, 0.5, 0.8, 0.3};
                    rotas = s->gulosoReativo(alfas, 10, 20);
                    break;
                }
                
                Valida *v = new Valida(rotas, p);

                for (size_t i = 0; i < rotas.size(); ++i)
                {
                    cout << "Rota " << i + 1 << ": ";
                    for (size_t j = 0; j < rotas[i].size(); ++j)
                    {
                        if (j == rotas[i].size() - 2)
                        {
                            cout << "Capacidade sobrando: ";
                        }
                        else if (j == rotas[i].size() - 1)
                        {
                            cout << "Custo da rota: ";
                        }

                        cout
                            << rotas[i][j] << " ";
                    }
                    cout
                        << endl;
                }
                cout
                    << "Custo Total: " << s->getCustoTotal() << endl;
                cout << "Solucao Otimo: " << p->getSolucaoOtima() << endl;

                double porcentagemAcimaOtimo = ((s->getCustoTotal() - p->getSolucaoOtima()) / p->getSolucaoOtima()) * 100;
                cout << "Valor acima da Solucao Otima: " << (s->getCustoTotal() - p->getSolucaoOtima()) << " que tem porcentagem: " << porcentagemAcimaOtimo << "%" << std::endl;


                if (!v->validar())
                {
                    if (!v->getAtendeuTodos())
                    {
                        cout << "A solução não atendeu todos os clientes!" << endl;
                    }
                    if (!v->getUnicoEmRota())
                    {
                        cout << "Os nós não são únicos nas rotas!" << endl;
                    }
                }
            }
        }
    }

    return 0;
}