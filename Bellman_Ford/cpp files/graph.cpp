#include "graph.hpp" //Nell'header vi sono solo i prototipi dei metodi
                     //In questo file, vi sono le definizioni

void Graph::ReadFile()  //Metodo che legge il grafo da analizzare in base al nome del file passato nel costruttore
{
    FILE *fp;                       //Dichiarazione del puntatore al file
    fp=fopen(filename, "r");        //Apertura del file in "read", dal momento che non serve scrivere
    if(fp==nullptr)                 //Controllo del puntatore, per verificare che il file sia stato trovato
        exit(-1);
    //Dichiarazione variabili
    char carattere;
    string nome, posizione, variabile, valore;
    int fase=1, i=0, posizione_intero=0, valore_intero=0, end_ciclo;
    //Inizio della lettura del file
    while(1)
    {
        fread(&carattere, sizeof(char), 1, fp);
        fseek(fp, 0, SEEK_CUR);
        //Controllo per la fine del ciclo, alternativa alla feof(), scomoda da usare in questo caso
        //Con questa funzione, si controlla se la fase è l'ultima e se si è trovato un secondo \n per individuare la fine del file
        if(fase==5&&carattere=='\n')
            break;  //Fine del file trovata
        else if(fase==5&&carattere!='\n')
            fase=1; //C'è ancora da leggere e si torna alla fase 1
        switch(carattere)//Switch sul carattere letto
        {
            case '[' : fase=2; fread(&carattere, sizeof(char), 1, fp); break;//Se c'è "[", si sta leggendo un edge, e si deve trovare la posizione sul vettore dell'edge
            case '.' : fase=3; fread(&carattere, sizeof(char), 1, fp); break;//Se c'è ".", si sta leggendo a quale variabile dell'edge verrà assegnato il valore
            case '=' : fase=4; fread(&carattere, sizeof(char), 1, fp); break;//Se c'è "=", si sta leggendo il valore
            case '\n': fase=5; break;                                        //Se c'è "\n", si sta ricominciando dalla fase 1, oppure il file è finito
            default  : break;                                                //Per tutti gli altri caratteri, non si cambiano fasi e li si lascia immagazzinare in memoria dalla fase attuale
        }
        switch(fase)    //Switch sulla fase da eseguire
        {
            //Fase 1: lettura del nome della variabile della class Graph (V, E o edge)
            case 1 : nome.push_back(carattere); i++; break;
            //Fase 2: Lettura della posizione dell'edge sul vector
            case 2 : while(carattere!=']'){posizione.push_back(carattere); i++; fread(&carattere, sizeof(char), 1, fp);} break;
            //Fase 3: lettura del nome della variabile della classe Edge (i_source, i_destination, weight)
            case 3 : variabile.push_back(carattere); i++; break;
            //Fase 4: Lettura del valore
            case 4 : valore.push_back(carattere); break;
            //Fase 5: Lettura IN MEMORIA delle variabili precedentemente prelevate dal file, per l'effettiva assegnazione
            case 5 : for(i=posizione.size()-1; i>=0; i--)
                     {
                         //Conversione da char a intero del valore di posizione
                         posizione_intero+=((posizione.at(i)&207)*pow(10,(posizione.size()-i-1)));
                     }
                     for(i=valore.size()-1; i>=0; i--)
                     {
                         //Conversione da char a intero del valore effettivo
                         if(valore.at(i)!='-')
                             valore_intero+=((valore.at(i)&207)*pow(10,(valore.size()-i-1)));
                         else //Se trova il carattere "-", rende il valore negativo
                             valore_intero= -valore_intero;
                     }
                     if(nome=="V") //Se il nome trovato è "V", assegna il valore effettivo alla variabile V della class Graph
                     {
                         V=valore_intero;
                         valore_intero=0;
                         for(i=1; i<V; i++)
                             distances.push_back(0xfffffff);
                     }
                     else if(nome=="E") //Se il nome trovato è "E", assegna il valore effettivo alla variabile E della class Graph
                     {
                         E=valore_intero;
                         valore_intero=0;
                         for(i=0; i<E; i++)
                             edge.push_back(Edge());
                     }
                     else if(nome=="edge") //Se il nome trovato è "edge", assegna il valore effettivo alla variabile edge della class Graph...
                     { //...trovando prima a quale variabile della class Edge si sta facendo riferimento
                         if(variabile=="i_source")
                         {
                             edge.at(posizione_intero).SetSource(valore_intero);
                             valore_intero=0;
                             posizione_intero=0;
                         }
                         else if(variabile=="i_destination")
                         {
                             edge.at(posizione_intero).SetDestination(valore_intero);
                             valore_intero=0;
                             posizione_intero=0;
                         }
                         else if(variabile=="weight")
                         {
                             edge.at(posizione_intero).SetWeight(valore_intero);
                             valore_intero=0;
                             posizione_intero=0;
                         }
                         else
                         {
                             valore_intero=0;
                             posizione_intero=0;
                         }
                     }
                     else //Exit in caso di file corrotto o con scrittura errata
                     {
                         cout << "Non e' stato possibile individuare a quale variabile assegnare il dato" << endl << "L'esecuzione finisce" << endl;
                         exit(-2);
                     }
                     //Cancellazione dalla memoria di tutte le variabili precedentemente lette da file
                     for(i=nome.size(); i>=0; i--)
                        nome.erase(i);
                     for(i=variabile.size(); i>=0; i--)
                        variabile.erase(i);
                     for(i=valore.size(); i>=0; i--)
                        valore.erase(i);
                     for(i=posizione.size(); i>=0; i--)
                        posizione.erase(i);
        }
    }
    fclose(fp); //Chiusura del file
    cout << "Il file contenente il grafo e' stato letto correttamente" << endl << endl; //Output che segnala l'arrivo alla fine della funzione senza essere incorsi in errore
}

void Graph::BellmanFord() //L'unico parametro passato è il nome del file, da passare di conseguenza al metodo ReadFile(char*)
{
    //Dichiarazione delle variabili
    int s, d, w;
    char alfabeto[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvz0123456789"; //Alfabeto per la scrittura dei nodi sottoforma di lettere
    cout << "--------------------------------------------------" << endl;
    cout << filename << endl;
    for(int i=0; i<V; i++) //Ciclo che rilassa gli archi V-1 volte in modo tale da riuscire a trovare la minima distanza dalla sorgente
    {
        for(int j=0; j<E; j++)//Ciclo interno che copre tutti gli archi del grafo
        {
            s=edge.at(j).GetSource();
            d=edge.at(j).GetDestination();
            w=edge.at(j).GetWeight();
            if(distances.at(s)!=0xfffffff && distances.at(d)>distances.at(s)+w)
                distances.at(d)=distances.at(s)+w;
        }
    }
    for(int i=0; i<E; i++) //Ciclo per l'individuazione dei ciclì di peso negativo nel grafo
    {
        s=edge.at(i).GetSource();
        d=edge.at(i).GetDestination();
        w=edge.at(i).GetWeight();
        if(distances.at(s)!=0xfffffff && distances.at(d)>distances.at(s)+w)
            cout << "C'e' un ciclo negativo tra " << alfabeto[s] << " e " << alfabeto[d] << endl; //output che segnala il ciclo negativo per quali archi passa
    }
    for(int i=0; i<distances.size(); i++) //Ciclo cbe scrive le distanze minime trovate per ogni nodo dalla sorgente
    {
        cout << "Il nodo " << alfabeto[i] << " dista da " << alfabeto[0] << " di " << distances.at(i) << endl;
    }
    cout << "--------------------------------------------------" << endl;
}
