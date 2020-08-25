#include "main.hpp"                     //Header contenente tutto il necessario al main

int main()
{
    Graph grafo1("grafo1.txt"), grafo2("grafo2.txt");   //Dichiarazione di due oggetti Graph con relativi passaggi di parametri
    grafo1.BellmanFord();   //Metodo che esegue l'algoritmo di Bellman-Ford, chiamato sul primo grafo
    grafo2.BellmanFord();   //Metodo che esegue l'algoritmo di Bellman-Ford, chiamato sul secondo grafo
    getchar();
    return 0;
}
