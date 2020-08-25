#ifndef EDGE_HPP_INCLUDED
#define EDGE_HPP_INCLUDED
//Header necessario alla class Edge
#include "librerie.hpp"

class Edge
{
private:
    int i_source; //Variabile che indica la sorgente dell'arco
    int i_destination; //Variabile che indica la destinazione dell'arco
    int weight; //Variabile che indica il peso dell'arco
public:
    Edge(){i_source=0xfffffff; i_destination=0xfffffff; weight=0xfffffff;} //Costruttore che inizializza tutto al massimo valore per un signed int
    //Metodi Set
    void SetSource(int i);
    void SetDestination(int i);
    void SetWeight(int w);
    //Metodi Get
    int GetSource();
    int GetDestination();
    int GetWeight();
};

#endif // EDGE_HPP_INCLUDED
