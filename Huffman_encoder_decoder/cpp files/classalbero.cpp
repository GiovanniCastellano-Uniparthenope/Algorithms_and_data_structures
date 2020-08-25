#include "classalbero.hpp" //Nell'header vi sono solo i prototipi dei metodi
                           //In questo file, vi sono le definizioni

//Metodi Set- Permettono di settare gli attributi della class albero
void albero::SetFrequence(unsigned long int freq)
{
    frequenza=freq;
}

void albero::SetValue(char val)
{
    valore=val;
}

void albero::SetAddressLeftSon(albero* nodo)
{
    pfs=nodo;
}

void albero::SetAddressRightSon(albero *nodo)
{
    pfd=nodo;
}
//Metodi Get- Restituiscono il valore (carattere) del nodo e gli indirizzi dei figli
char albero::GetValue()
{
    return valore;
}

albero* albero::GetAddressLeftSon()
{
    return pfs;
}

albero* albero::GetAddressRightSon()
{
    return pfd;
}
//Metodo booleano che controlla se il nodo è una foglia
bool albero::IsLeaf()
{
    if(pfs==nullptr&&pfd==nullptr)
        return true;
    else
        return false;
}
