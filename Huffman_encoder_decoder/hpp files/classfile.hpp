#ifndef CLASSFILE_HPP_INCLUDED
#define CLASSFILE_HPP_INCLUDED
//Header necessari alla class file
#include "librerie.hpp"
#include "classalbero.hpp"
#include "priorityqueue.hpp"
#define DIVISORE_TEMPO 10000000

class file{
private:
    long int filesize, encoded_filesize; //Dimensioni dei file originario e compresso
    string name; //nome del file originario
    int size_alfabeto; //Numero di valori differenti trovati
    vector<char> binary_encode, binary_decode; //File originario e file compresso
    vector<vector<unsigned int>> bit_sequence; //Sequenza di bit che codifica i vari caratteri dell'alfabeto
    priorityqueue coda; //La coda di priorità che consente di creare l'albero
    vector<char> alfabeto_estratto; //I valori che vengono estratti dalla coda man mano che si crea l'albero di Huffman
    vector<char> char_coda; //Caratteri già inseriti nella coda
    vector<unsigned long int> occorrenze; //Occorrenze dei vari caratteri estratti dal file
    vector<albero*> tree; //Il vector di class albero, è l'albero che permette al decoder di identificare i caratteri
    albero *inizio_tree; //Pointer all'inizio dell'albero (radice)
    void Alphabet(char); //Metodo che crea l'alfabeto e le occorrenze
    unsigned long int ExtractMin(); //Funzione che richiede alla coda la minima occorrenza e il suo valore
    void ReadFile(char*); //Metodo che legge il file, lo scrive in binary_encode/decode e scrive nome e formato in name
    void CreateTree(); //Metodo che crea l'albero di Huffman
    void MarkTheWay(albero*, int, vector<int>*); //Metodo che crea ricorsivamente le varie sequenze binarie

public:
    file(){filesize=0; encoded_filesize=0; size_alfabeto=0; inizio_tree=nullptr;} //Costruttore che inzializza i size a zero e il pointer a null
    void Encoder(char*); //Metodo che crea il file compresso codificando l'originale
    void Decoder(); //Metodo che ricrea il file originale sotto diverso nome decodificando il file compresso
};

#endif // CLASSFILE_HPP_INCLUDED
