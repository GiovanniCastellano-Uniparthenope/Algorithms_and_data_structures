#ifndef PRIORITYQUEUE_HPP_INCLUDED
#define PRIORITYQUEUE_HPP_INCLUDED
//Header necessario alla class priorityqueue
#include "librerie.hpp"

class priorityqueue{
private:
    vector<char> alfabeto; //L'alfabeto dei valori
public:
    priorityqueue(){;} //Costruttore
    void Insert(char); //Inserimento del valore passato come parametro nella coda
    char ExtractMin(); //Metodo che restituisce il valore corrispondente alla minima occorrenza
    void SelectSort(vector<unsigned long int>*); //Sort per riordinare la coda
    void ChangePriority(int); //Metodo per rendere l'elemento d'indice passato come parametro a massima priorità
};

#endif // PRIORITYQUEUE_HPP_INCLUDED
