#include "Declarari.h"
#include "Textbox.h"

ifstream f("decodare.in");
ofstream g("decodare.out");

bool esteFiller(int& linie)
{
    for (int i = 0; i < numarFillere; i++)
    {
        size_t found = cod[linie].find(declarari[i]);      //Cauta aparitia uneia dintre functiile de tip filler pentru pseudocod
        if (found != string::npos)
            return 0;
    }

    if (cod[linie][0] == '\0')
        return 0;

    if (primaAcolada == 0 and cod[linie][0] == '{')
    {
        primaAcolada = 1;                                   //Sterge acolada main-ului
        return 0;
    }

    return 1;
}

void citire(string clipboard[])
{
    while(clipboard[contorLinii] != '\0'){
    }
    while (getline(f, cod[contorLinii]))
        if (esteFiller(contorLinii) == 1)
            contorLinii++;
    contorLinii--;
    return;
}

void afisare()
{
    for (int i = 0; i < contorLinii; i++)
        g << cod[i] << '\n';
    return;
}

void afisare_pseudocod()
{
    for (int i = 0; i < contorLinii; i++)
        g << cod_pseudocod[i] << '\n';
    return;
}

void stergeSpatiile()
{

    for (int i = 0; i < contorLinii; i++)
        for (int j = 0; cod[i][j]; j++)
        {
            if (cod[i][j] == '"' || int(cod[i][j]) == 39)
            {
                j++;
                while (cod[i][j] != '"' and int(cod[i][j]) != 39)      ///  Daca sunt gasite spatii intre ' ' inseamna ca acestea
                    j++;                                               ///nu trebuiesc sterse fiind folosite probabil la afisari
            }
            else if (cod[i][j] == ' ')
            {
                cod[i].erase(j, 1);
                j--;
            }
        }
}

void inlocuireFunctii()
{
    for (int i = 0; i < contorLinii; i++)
        for (int j = 0; j < 9; j++)
        {
            size_t found = cod[i].find(functii[j]);                       ///  Salvam pozitia primei aparitii a functiei
            while (found != string::npos)                                 ///daca aceasta este pe acel rand.
            {
                cod[i].replace(found, functii[j].length(), inlocuiri[j]);
                found = cod[i].find(functii[j], found + 1);                  ///  Cautam pozitia urmatoarei functii daca aceasta exista
            }                                                             ///pornind de la pozitia ultimei functii gasite +1
        }
}

void Decodare()
{
    stergeSpatiile();
    inlocuireFunctii();
}

void Reconstruire()
{
    for (int i = 0; i < contorLinii; i++)
    {
        ce_functie = 0;
        for (int k = 0; k < 6; k++)
        {
            if (cod[i][0] == inlocuiri[k][0])
            {
                cod_pseudocod[i] = functii_pseudocod[k];                    ///Inlocuim fiecare functie din C++ in afara de else/else if prescurtata cu echivalentul sau in pseudocod
                cod[i][0] = ' ';                                            ///Inlocuim in codul C++ functiile cu un spatiu pentru a putea forma mai usor codul in pseudocod
                ce_functie = k;
            }

        }
        if (cod[i][0] == inlocuiri[6][0])
        {
            if (cod[i][1] == inlocuiri[2][0])
            {
                cod_pseudocod[i] = functii_pseudocod[6] + " " + functii_pseudocod[2];    ///Inlocuil else/else if cu echivalentele lor in pseudocod
                cod[i].erase(1, 1);
                ce_functie = 2;
            }
            else cod_pseudocod[i] = functii_pseudocod[6];
            cod[i][0] = ' ';
        }
        cod_pseudocod[i] = cod_pseudocod[i] + cod[i] + functii_pseudocod_continuari[ce_functie];


        for (int j = 0; cod_pseudocod[i][j]; j++)
        {
            if (cod_pseudocod[i][j] == ';')
                cod_pseudocod[i].replace(j, 1, " ");                                            ///Stergem caracterul ;
            else if (cod_pseudocod[i][j] == '=')
            {
                if (cod_pseudocod[i][j + 1] != '=' &&
                    cod_pseudocod[i][j - 1] != '<' && cod_pseudocod[i][j - 1] != '>' &&
                    cod_pseudocod[i][j - 1] != '!')                                             ///Inlocuim '=' cu '<-' si '==' cu '='
                    cod_pseudocod[i].replace(j, 1, "<-");
                else if (cod_pseudocod[i][j + 1] == '=')
                    cod_pseudocod[i].replace(j, 2, "=");
            }
            else if (cod_pseudocod[i][j] == '(' || cod_pseudocod[i][j] == ')')           ///Stergem parantezele functiilor
                cod_pseudocod[i].erase(j, 1);
        }
    }
}

void translareCod(string clipboard[]) {
    citire(clipboard);
    Decodare();
    Reconstruire();
    afisare_pseudocod();
}