#include "main.hpp"                   //Header contenente tutto il necessario al main

int main()
{
    file comprimi; //Oggetto di tipo file che diventerà il file determinato dal nomefile
    int i=0; //Contatore utilizzato per degli accorgimenti sulla stringa nome inserita da tastiera
    char *nomefile = new char[namesize]; //Nome del file da aprire, inserito da tastiera

    //Svolgimento dell'algoritmo di Huffman
    cout << "--------------------------------" << endl;
    cout << "Nome del file da comprimere: " << endl;
    fgets(nomefile, namesize, stdin);
    while(i<namesize)
    {
        if(*(nomefile+i)=='\n')
            break;
        i++;
    }
    *(nomefile+i)='\0';
    comprimi.Encoder(nomefile); getchar();
    system("cls"); cout << "Premere ENTER per avviare la decompressione"; getchar(); system("cls");
    comprimi.Decoder();
    cout << "--------------------------------" << endl;
    getchar();
    return 0;
}
