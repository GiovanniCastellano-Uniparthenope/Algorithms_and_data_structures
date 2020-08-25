#ifndef CLASSALBERO_HPP_INCLUDED
#define CLASSALBERO_HPP_INCLUDED
//Header necessario alla class albero
#include "librerie.hpp"

class albero{
private:
    unsigned long int frequenza; //Frequenza del carattere del nodo, o somma delle frequenze dei figli
    char valore; //Carattere del nodo, NULL (0x0) se il nodo è un ponte
    albero *pfs; //Indirizzo del figlio sinistro
    albero *pfd; //Indirizzo del figlio destro
public:
    albero(){frequenza=0; valore=0; pfs=nullptr; pfd=nullptr;} //Costruttore che inzializza a null tutti gli attributi
    //Metodi Set
    void SetFrequence(unsigned long int);
    void SetValue(char);
    void SetAddressLeftSon(albero*);
    void SetAddressRightSon(albero*);
    //Metodi Get
    char GetValue();
    albero* GetAddressLeftSon();
    albero* GetAddressRightSon();
    //Metodo per controllare se il nodo è una foglia
    bool IsLeaf();
};


#endif // CLASSALBERO_HPP_INCLUDED
