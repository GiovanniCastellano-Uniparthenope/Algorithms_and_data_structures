#include "edge.hpp" //Nell'header vi sono solo i prototipi dei metodi
                    //In questo file, vi sono le definizioni

//Metodi Set- per la scrittura in memoria delle tre variabili della class Edge
void Edge::SetSource(int i)
{
    i_source=i;
}

void Edge::SetDestination(int i)
{
    i_destination=i;
}

void Edge::SetWeight(int w)
{
    weight=w;
}

//Metodi Get- per la lettura da memoria delle tre variabili della class Edge
int Edge::GetSource()
{
    return i_source;
}

int Edge::GetDestination()
{
    return i_destination;
}

int Edge::GetWeight()
{
    return weight;
}
