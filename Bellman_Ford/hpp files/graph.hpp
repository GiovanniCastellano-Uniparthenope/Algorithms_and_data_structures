#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED
//Header necessari alla class Graph
#include "librerie.hpp"
#include "edge.hpp"

class Graph
{
private: //Variabili private
    int V; //Variabile che segna il numero di vertici
    int E; //Variabile che segna il numero di archi
    char *filename; //Nome del file contenente il grafo
    vector<int> distances; //Vector che segna le distanze, per ogni nodo, dalla sorgente
    vector<Edge> edge; //Vector di tipo class Edge che contiene tutti gli archi del grafo
    void ReadFile(); //Il metodo che legge il grafo da un file .txt
public:
    Graph(char *nome){V=0; E=0; distances.push_back(0); filename=nome; ReadFile();} //Costruttore che inzializza tutto a zero, e inserisce la distanza dalla sorgente della sorgente stessa a 0
    void BellmanFord(); //Il metodo che esegue l'algoritmo di Bellman-Ford
};

#endif // GRAPH_HPP_INCLUDED
