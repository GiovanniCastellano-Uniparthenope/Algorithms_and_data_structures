#include "priorityqueue.hpp" //Nell'header vi sono solo i prototipi dei metodi
                             //In questo file, vi sono le definizioni

//Metodo Insert(char), permette di inserire nella coda il carattere passato come parametro
void priorityqueue::Insert(char carattere)
{
    alfabeto.push_back(carattere);
}

//SelectSort per ordinare la coda in modo lineare
void priorityqueue::SelectSort(vector<unsigned long int> *occorrenze)
{
    int maxvalue, i_max, i=0, j=-1;
    char appoggio;
    do
    {
        j++;
        maxvalue=occorrenze->at(j);
        i_max=j;
        for(i=j; i<occorrenze->size(); i++)
        {
            if(maxvalue<occorrenze->at(i))
            {
                maxvalue=occorrenze->at(i);
                i_max=i;
            }
        }
        occorrenze->at(i_max)=occorrenze->at(j);
        occorrenze->at(j)=maxvalue;
        appoggio=alfabeto.at(i_max);
        alfabeto.at(i_max)=alfabeto.at(j);
        alfabeto.at(j)=appoggio;
    }while(j<alfabeto.size()-1);
}

//ExtractMin() estrae il minimo dalla coda, ovvero l'ultimo elemento
char priorityqueue::ExtractMin()
{
    char var = alfabeto.at(alfabeto.size()-1);
    alfabeto.erase(alfabeto.begin()+alfabeto.size()-1);
    return var;
}

//Metodo che cambia la priorità dell'elemento puntato dall'indice i
void priorityqueue::ChangePriority(int i)
{
    char val;
    val=alfabeto[i];
    for(int j=alfabeto.size()-1; j>=i; j--)
    {
        alfabeto[j-1]=alfabeto[j];
    }
    alfabeto[alfabeto.size()-1]=val;
}
