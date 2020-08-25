#include "librerie.hpp"
#include "classfile.hpp"

using namespace std;

void file::Alphabet(char current_value)
{
    int i = 0;
    while(i < alfabeto.size() && alfabeto.at(i) != current_value)
        i++;
    if(i == alfabeto.size())
    {
        alfabeto.push_back(current_value);
        occorrenze_alfabeto.push_back(1);
    }
    else
        occorrenze_alfabeto.at(i)++;
}

void file::SortByOccurrence()
{
    int maxvalue, i_max, i=0, j=-1;
    char appoggio;
    do
    {
        j++;
        maxvalue=occorrenze_alfabeto.at(j);
        i_max=j;
        for(i=j; i<occorrenze_alfabeto.size(); i++)
        {
            if(maxvalue<occorrenze_alfabeto.at(i))
            {
                maxvalue=occorrenze_alfabeto.at(i);
                i_max=i;
            }
        }
        occorrenze_alfabeto.at(i_max)=occorrenze_alfabeto.at(j);
        occorrenze_alfabeto.at(j)=maxvalue;
        appoggio=alfabeto.at(i_max);
        alfabeto.at(i_max)=alfabeto.at(j);
        alfabeto.at(j)=appoggio;
    }while(j<alfabeto.size()-1);
}

void file::ReadFile(string filename)
{

    int percent;
    char current_value;
    char *nome = new char[filename.size()];

    fp = fopen(nome, "rb");
    if(fp==nullptr)
        exit(-2);
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    binary.reserve(filesize+1);
    cout << "Dimensione file: " << filesize << " byte" << endl;
    for(int i=0; i<filesize; i++)
    {
        percent = (int)((float)((i+1)*100)/(float)filesize);
        fread(&current_value, sizeof(char), 1, fp);
        fseek(fp, 0, SEEK_CUR);
        binary.push_back(current_value);
        Alphabet(current_value);
        cout << "\r";
        cout << "Passo "<< i+1 << ": lettura file e creazione alfabeto " << percent << "% completata";
        cout << "\nIl file e' stato letto correttamente\n" << endl;
        cout << "Caratteri differenti trovati: " << alfabeto.size() << "(" << occorrenze_alfabeto.size() << ")";
    }
    SortByOccurrence();
    fclose(fp);
}

void file::PrintFile()
{
    for(int i=0; i<filesize; i++)
    {
         cout << alfabeto.at(i) << ": " << occorrenze_alfabeto.at(i) << endl;
    }
}
