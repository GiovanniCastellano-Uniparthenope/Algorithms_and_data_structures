#include "classfile.hpp" //Nell'header vi sono solo i prototipi dei metodi
                         //In questo file, vi sono le definizioni

void file::Alphabet(char current_value) //Metodo che inserisce il carattere passato come parametro all'alfabeto e incrementa di 1 la sua occorrenza
{
    int i = 0;
    while(i < size_alfabeto && char_coda.at(i) != current_value) //Finchè non trova il carattere nell'alfabeto...
        i++;                                                      //...incrementa i
    if(i == size_alfabeto) //Se non è stato trovato il carattere...
    {                        //...inserisce il nuovo carattere nell'alfabeto e setta la sua occorrenza a 1...
        coda.Insert(current_value);
        char_coda.push_back(current_value);
        occorrenze.push_back(1);
        size_alfabeto++;
    }
    else                    //...altrimenti, trovato il carattere, ne incrementa l'occorrenza di 1
        occorrenze.at(i)+=1;
}

unsigned long int file::ExtractMin() //Metodo che estrae il minimo dalla coda di priorità
{
    unsigned long int occorrenza;
    alfabeto_estratto.push_back(coda.ExtractMin());
    occorrenza=occorrenze.at(occorrenze.size()-1);
    occorrenze.erase(occorrenze.begin()+occorrenze.size()-1);
    return occorrenza;
}

void file::ReadFile(char *filename) //Metodo che legge i caratteri dal file passatogli come parametro
{
    FILE *fp; //Puntatore a file
    int i=0;
    //Scrittura nella string name il nome del file, utile nei metodi successivi
    while(filename[i]!='.'&&i<namesize)
        name+=filename[i++];
    if(i<namesize-4)
    for(int j=0; j<4; j++)
        name+=filename[i++];
    char current_value;
    //Apertura del file in lettura binaria e controllo del puntatore per accertarsi che il file sia stato trovato
    fp = fopen(filename, "rb");
    if(fp==nullptr)
    {
        cout << "Nessun file trovato con quel nome" << endl; getchar();
        exit(-2);
    }
    //Lettura della dimensione del file tramite le funzioni fseek(FILE*, long, int) e ftell(FILE*)
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    binary_encode.reserve(filesize+1);
    //Output che scrive il tempo previsto per la lettura del file in base alla dimensione
    cout << "Lettura file: " << filename << endl;
    cout << "Dimensione file: " << filesize << " byte" << endl;
    cout << "Tempo previsto necessario: ";
    switch(filesize/DIVISORE_TEMPO)
    {
        case 0 : cout << "meno di un minuto" << endl; break;
        case 1 : cout << "circa 1 minuto" << endl; break;
        default: cout << "circa " << filesize/DIVISORE_TEMPO << " minuti" << endl; break;
    }
    //Lettura effettiva del file, con chiamata al metodo Alphabet(char)
    for(int i=0; i<filesize; i++)
    {
        fread(&current_value, sizeof(char), 1, fp);
        fseek(fp, 0, SEEK_CUR);
        binary_encode.push_back(current_value); //Si riscrive il file in binary_encode per tenerlo in memoria
        Alphabet(current_value);
    }
    //Output che segnala la corretta lettura del file
    cout << "\nIl file e' stato letto correttamente\n" << endl;
    cout << "Caratteri differenti trovati: " << size_alfabeto << endl;
    //Chiamata alla funzione SortByOccurrence() che riordina in senso decrescente l'alfabeto e le sue occorrenze
    coda.SelectSort(&occorrenze);
    fclose(fp); //chiusura del file
}

void file::CreateTree() //Metodo che crea l'albero a partire dalla tree di priorità, definita da alfabeto e occorrenze_alfabeto
{
    //Dichiarazione delle variabili
    vector<unsigned long int> f_nodo; //Nodi creati da questo metodo che non hanno valore, ma hanno una frequenza e fanno da ponte
    vector<int> i_nodo;               //Indici dei nodi senza valore nel tree
    unsigned long int min_occ, sec_min_occ, min_coda=0, sec_min_coda=0; //Occorrenze minime trovate (primo e secondo minimo)
    int min_i, sec_min_i;                   //Indici delle occorrenze minime trovate
    int i=size_alfabeto-1, j=0;
    do
    {
        min_occ=0xffffffff;
        sec_min_occ=0xffffffff;
        //Ricerca del primo e secondo minimo nel vector contenente i nodi senza valore
        for(int k=0; k<f_nodo.size(); k++)
        {
            if(min_occ>f_nodo.at(k))
            {
                min_occ=f_nodo.at(k);
                min_i=k;
            }
            else if(sec_min_occ>f_nodo.at(k)&&f_nodo.at(k)>=min_occ)
            {
                sec_min_occ=f_nodo.at(k);
                sec_min_i=k;
            }
        }
        if(i>0) //Controllo per evitare che, in determinate situazioni, si vada a controllare un settore della coda non esistente
        {
            if(min_coda==0)
            {
                min_coda=ExtractMin();
            }
            if(min_coda<=min_occ||f_nodo.size()==1)
            {
                sec_min_coda=ExtractMin();
                i--;
                if(sec_min_coda<=sec_min_occ) //Questo controllo qui, in caso di i=0 iniziale, avrebbe creato un errore di esecuzione senza il controllo i>0
                {
                    //Parte eseguita in caso le due occorrenze minime siano consecutive sulla coda di priorità
                    i--;
                    tree.push_back(new albero);
                    tree.at(j)->SetFrequence(min_coda+sec_min_coda);
                    f_nodo.push_back(min_coda+sec_min_coda);
                    i_nodo.push_back(j);

                    tree.push_back(new albero);
                    tree.at(j+1)->SetFrequence(min_coda);
                    tree.at(j+1)->SetValue(alfabeto_estratto.at(alfabeto_estratto.size()-2));
                    tree.at(j)->SetAddressLeftSon(tree.at(j+1));
                    min_coda=0;

                    tree.push_back(new albero);
                    tree.at(j+2)->SetFrequence(sec_min_coda);
                    tree.at(j+2)->SetValue(alfabeto_estratto.at(alfabeto_estratto.size()-1));
                    tree.at(j)->SetAddressRightSon(tree.at(j+2));
                    inizio_tree=tree.at(j);
                    sec_min_coda=0;
                    j+=3;
                }
                else
                {
                    //Parte eseguita in caso i minimi siano misti, uno proveniente dalla coda di priorità e l'altro proveniente dall'alhero
                    tree.push_back(new albero);
                    tree.at(j)->SetFrequence(min_coda+min_occ);
                    tree.at(j)->SetAddressRightSon(tree.at(i_nodo.at(min_i)));
                    f_nodo.erase(f_nodo.begin()+min_i);
                    i_nodo.erase(i_nodo.begin()+min_i);

                    f_nodo.push_back(min_coda+min_occ);
                    i_nodo.push_back(j);

                    tree.push_back(new albero);
                    tree.at(j+1)->SetFrequence(min_coda);
                    tree.at(j+1)->SetValue(alfabeto_estratto.at(alfabeto_estratto.size()-2));
                    tree.at(j)->SetAddressLeftSon(tree.at(j+1));
                    inizio_tree=tree.at(j);
                    if(sec_min_coda!=0)
                    {
                        min_coda=sec_min_coda;
                        sec_min_coda=0;
                    }
                    j+=2;
                }
            }
            else if(f_nodo.size()>1)
            {
                //Parte eseguita in caso entrambi i minimi provengano dall'albero
                tree.push_back(new albero);
                tree.at(j)->SetFrequence(min_occ+sec_min_occ);
                tree.at(j)->SetAddressLeftSon(tree.at(i_nodo.at(min_i)));
                tree.at(j)->SetAddressRightSon(tree.at(i_nodo.at(sec_min_i)));
                f_nodo.erase(f_nodo.begin()+min_i);
                i_nodo.erase(i_nodo.begin()+min_i);
                f_nodo.erase(f_nodo.begin()+sec_min_i-1);
                i_nodo.erase(i_nodo.begin()+sec_min_i-1);

                f_nodo.push_back(min_occ+sec_min_occ);
                i_nodo.push_back(j);

                inizio_tree=tree.at(j);
                j+=1;
            }
        }
        else if(f_nodo.size()>1)
        {
            //Parte eseguita se nell'alfabeto non ci sono più caratteri da esaminare
            tree.push_back(new albero);
            tree.at(j)->SetFrequence(min_occ+sec_min_occ);
            tree.at(j)->SetAddressLeftSon(tree.at(i_nodo.at(min_i)));
            tree.at(j)->SetAddressRightSon(tree.at(i_nodo.at(sec_min_i)));
            f_nodo.erase(f_nodo.begin()+min_i);
            i_nodo.erase(i_nodo.begin()+min_i);
            f_nodo.erase(f_nodo.begin()+sec_min_i-1);
            i_nodo.erase(i_nodo.begin()+sec_min_i-1);

            f_nodo.push_back(min_occ+sec_min_occ);
            i_nodo.push_back(j);

            inizio_tree=tree.at(j);
            j+=1;
        }
    }while(i>0||f_nodo.size()>1);
    if(i==0&&size_alfabeto>1)
    {
        //Parte eseguita solo se dovessero rimanere due valori da tener conto, uno nella coda di priorità e l'altro nell'albero
        if(min_coda==0)
            min_coda=ExtractMin();
        tree.push_back(new albero);
        tree.at(j)->SetFrequence(min_coda+f_nodo.at(0));
        tree.at(j)->SetAddressRightSon(tree.at(i_nodo.at(0)));

        f_nodo.erase(f_nodo.begin());
        i_nodo.erase(i_nodo.begin());

        tree.push_back(new albero);
        tree.at(j+1)->SetFrequence(min_coda);
        tree.at(j+1)->SetValue(alfabeto_estratto.at(alfabeto_estratto.size()-1));
        tree.at(j)->SetAddressLeftSon(tree.at(j+1));
        inizio_tree=tree.at(j);
    }
    else if(size_alfabeto==1)
    {
        //Parte eseguita nel caso speciale in cui l'alfabeto è composto da un singolo valore, che causerebbe errori di esecuzione nel ciclo precedente
        min_coda=ExtractMin();
        tree.push_back(new albero);
        tree.at(j)->SetFrequence(min_coda);

        tree.push_back(new albero);
        tree.at(j+1)->SetFrequence(min_coda);
        tree.at(j+1)->SetValue(alfabeto_estratto.at(i));
        tree.at(j)->SetAddressLeftSon(tree.at(j+1));
        inizio_tree=tree.at(j);
    }
}


void file::MarkTheWay(albero *walker, int bit, vector<int> *single_bit_sequence) //Metodo ricorsivo utilizzato per marcare ogni valore con una determinata sequenza elaborata grazie all'albero creato dalla MakeTree()
{

    int i=0;
    if(walker->IsLeaf())
    {
        //Se il nodo è una foglia, aggiunge il bit alla sequenza codice e passa subito al return
        single_bit_sequence->push_back(bit);
        while(walker->GetValue()!=alfabeto_estratto.at(i))
            i++;
        for(int j=0; j<single_bit_sequence->size(); j++)
            bit_sequence.at(i).push_back(single_bit_sequence->at(j));
    }
    else
    {
        //Se il nodo non è una foglia, oltre ad aggiungere il bit alla sequenza codice, visita anche i figli
        if(walker!=inizio_tree)//La radice non conta
            single_bit_sequence->push_back(bit);
        if(walker->GetAddressLeftSon()!=nullptr)
        {
            MarkTheWay(walker->GetAddressLeftSon(), 0, single_bit_sequence);
            single_bit_sequence->erase(single_bit_sequence->begin()+single_bit_sequence->size()-1);
        }
        if(walker->GetAddressRightSon()!=nullptr)
        {
            MarkTheWay(walker->GetAddressRightSon(), 1, single_bit_sequence);
            single_bit_sequence->erase(single_bit_sequence->begin()+single_bit_sequence->size()-1);
        }
    }
    return;
}

void file::Encoder(char *file_name) //Metodo che crea e scrive il file compresso
{
    FILE *fp; //puntatore a file
    //Chiamata ai metodi che leggono il file e ne creano l'albero relativo
    ReadFile(file_name);
    CreateTree();

    albero *walker=inizio_tree;
    vector<int>single_bit_sequence;
    vector<unsigned int>buffer;
    bit_sequence.reserve(alfabeto_estratto.size());
    for(int i=0; i<alfabeto_estratto.size(); i++)
        bit_sequence.push_back(buffer);
    MarkTheWay(walker, 0, &single_bit_sequence);

    char filename[name.size()+2];
    char file_format[]=".bin\0"; //formato del file compresso (.bin)
    int i=0, j=0, k=0, k2=0, h=0;
    vector<unsigned int> sequence;
    char character=0x0;
    for(i=0; i<8; i++)
        sequence.push_back(0);
    i=0;
    while(name.at(i)!='.') //Il nome del file è preso dal nome inserito in name durante la ReadFile()
    {
        filename[i]=name.at(i);
        i++;
    }
    filename[i]='2'; //Un "2" è aggiunto al nome per differenziarlo dal primo
    i++;
    for(j=0; j<5; j++)
    {
        filename[i]=file_format[j]; //Infine il formato del file da scrivere non viene preso da name, ma da file_format
                                    //(perchè i file compressi non devono poter essere aperti con gli stessi programmi
                                    //dei file originari)
        i++;
    }
    fp = fopen(filename, "wb"); //Viene quindi creato e aperto in scrittura binaria il file .bin
    if(fp==nullptr)
        exit(-3);
    //L'output segnala l'inizio del processo di scrittura
    cout << "Creazione file compresso: " << filename << endl << "L'operazione richiede circa il tempo di lettura" << endl;
    for(i=0; i<binary_encode.size(); i++)
    {
        if(i==97)
            int i=0;
        j=0;
        while(binary_encode.at(i)!=alfabeto_estratto.at(j)) //Si incrementa j finchè non si trova il carattere in alfabeto
            j++;
        k2=0; //k2 ricorda l'indice di bit_sequence
        while(k2<bit_sequence.at(j).size())
        {
            sequence.at(k++)=bit_sequence.at(j).at(k2++); //k ricorda l'indice di sequence
            if(k>=8) //Se la sequenza di 8 bit è stata scritta tutta in sequence...
            {
                for(h=0; h<7; h++)
                {
                    //...si tramuta la sequenza in binario, scrivendola in character
                    character=character|sequence.at(h);
                    character=character<<1;
                }
                character=character|sequence.at(h);
                //character quindi sarà un carattere dato dalla sequenza binaria precedentemente contenuta in sequence...
                fwrite(&character, sizeof(char), 1, fp);//...e viene scritto sul file compresso...
                //...quindi si azzerano character e k
                character=0x0;
                fseek(fp, 0, SEEK_CUR);
                k=0;
            }
        }//Il ciclo continua finchè non la sequenza non è finita, poi si prende la sequenza successiva
    }//Il ciclo esterno continua finchè ci sono caratteri da codificare
    if(k>0||sequence.at(0)==1)
    {
        //Il passaggio successivo consente di scrivere tutti bit 1 laddove la sequenza sia finita,
        // ma in sequence ci sono ancora spazi vuoti
        while(k<8)
            sequence.at(k++)=1;
        for(int h=0; h<7; h++)
        {
            character=character|sequence.at(h);
            character=character<<1;
        }
            character=character|sequence.at(h);
            fwrite(&character, sizeof(char), 1, fp);
            fseek(fp, 0, SEEK_CUR);
    }
    fclose(fp);
    cout << "\nScrittura del file compresso completata" << endl;
}

void file::Decoder() //Metodo che crea e scrive il file decompresso
{
    FILE *fp;
    char filename[name.size()+2], file_format[]=".bin\0", current_value;
    int i=0, bit, written=0;
    vector <int> sequence;
    albero *walker=inizio_tree;
    //Si riscrive il nome del file col "2" e con formato .bin, per leggere il file compresso creato dal metodo Encoder()
    for(i=0; i<8; i++)
        sequence.push_back(0);
    i=0;
    while(name.at(i)!='.')
    {
        filename[i]=name.at(i);
        i++;
    }
    filename[i]='2';
    i++;
    for(int j=0; j<5; j++)
    {
        filename[i]=file_format[j];
        i++;
    }
    cout << "\nLettura file: " << filename << endl;
    //Si apre il file compresso...
    fp = fopen(filename, "rb");
    //...e si riscrive in filename il nome del file col "2", ma dello stesso formato del file originario
    if(fp==nullptr)
        exit(-2);
    fseek(fp, 0L, SEEK_END);
    encoded_filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    i=0;
    while(name.at(i)!='.')
    {
        filename[i]=name.at(i);
        i++;
    }
    filename[i]='2';
    i++;
    for(int j=0; j<4; j++)
    {
        filename[i]=name.at(i-1);
        i++;
    }
    while(i<name.size())
    {
        filename[i]=name.at(i-1);
        i++;
    }
    filename[i]='\0';
    //L'output segnala l'inizio dell'operazione di lettura del file compresso
    cout << "Tempo previsto necessario: ";
    switch(encoded_filesize/DIVISORE_TEMPO)
    {
        case 0 : cout << "meno di un minuto" << endl; break;
        case 1 : cout << "circa 1 minuto" << endl; break;
        default: cout << "circa " << encoded_filesize/DIVISORE_TEMPO << " minuti" << endl; break;
    }
    for(i=0; i<encoded_filesize; i++)
    {
        fread(&current_value, sizeof(char), 1, fp);
        fseek(fp, 0, SEEK_CUR);
        binary_decode.push_back(current_value);
    }
    //L'output segnala la fine dell'operazione di lettura del file compresso
    cout << "\nIl file e' stato letto correttamente" << endl;
    cout << "Creazione file decompresso: " << filename << endl << "L'operazione richiede circa il tempo di lettura" << endl;
    //Si chiude quindi il file binario, per poi creare ed aprire in scrittura binaria il file in formato originale
    fclose(fp);
    fp = fopen(filename, "wb");
    if(fp==nullptr)
        exit(-2);

    for(i=0; i<binary_decode.size(); i++)
    {
        current_value=binary_decode.at(i);
        for(int j=7; j>=0; j--)
        {
            //Il ciclo decodifica la sequenza di 8 bit letta dal file compresso
            bit=current_value&(int)1;
            sequence.at(j)=bit;
            current_value=current_value>>1;
        }
        for(int j=0; j<8; j++)
        {
            //Il ciclo, data la sequenza letta, cammina sull'albero in cerca di una foglia
            if(sequence.at(j)==0)
                walker=walker->GetAddressLeftSon();
            else if(sequence.at(j)==1&&alfabeto_estratto.size()>1)
                walker=walker->GetAddressRightSon();
            if(walker->IsLeaf()&&written<filesize)
            {
                //Trovata la foglia, scrive il suo valore sul file
                current_value=walker->GetValue();
                fwrite(&current_value, sizeof(char), 1, fp);
                written++;
                fseek(fp, 0, SEEK_CUR);
            }
            if(walker->IsLeaf())
                walker=inizio_tree;
        }
    }
    //Infine, si chiude il file e l'output segnala che la decompressione è andata a buon fine
    fclose(fp);
    cout << "\nScrittura del file decompresso completata" << endl;
    cout << endl << "La compressione ha risparmiato " << filesize-encoded_filesize << " byte (" << 100-((encoded_filesize*100)/filesize) << "%)" << endl;
}
