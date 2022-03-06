#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include "Textbox.h"
#include <vector>

using namespace std;

ifstream f("decodare.in");
ofstream g("decodare.out");

#define numarFillere 12

int contorLinii;
int ce_functie;
int ce_functieAcolada;
int numar_intabulari;
bool primaAcolada;
string cod[40];
string cod_pseudocod[40];
string declarari[20] = { "int","char","long long", "bool", "using", "return", "main",
                         "include", "ifstream", "ofstream", "double", "float" };
string functii[20] = { "cin>>",  "cout<<",  "if",  "for",  "while",  "do",  "else", "<<", ">>" };
string inlocuiri[20] = { "~",      "`",      "@",     "^",    "_",      "?",   "$", ",", "," };
string functii_pseudocod[20] =              { "citire", "afisare","daca",   "pentru",  "cat timp",  "executa",  "altfel" };
string functii_pseudocod_continuari[20] =   { ""      , ""  ,     " atunci"," executa"," executa" , ""       , "" };
string functii_pseudocod_sfarsit[20] =      { "1","1",  "sfarsit daca", "sfarsit pentru", "sfarsit cat timp", "1", "sfarsit altfel" };


stack <int> stiva_acolade;                                ///Retine indicele sfarsitului de functie

 bool esteFiller(string &linie) {
     for (int i = 0; i < numarFillere; i++) {
         size_t found = linie.find(declarari[i]);      //Cauta aparitia uneia dintre functiile de tip filler pentru pseudocod
         if (found != string::npos) {
             return 0;
         }
     }

     if (linie[0] == '\0') {
         return 0;
     }

     if (primaAcolada == 0 and linie[0] == '{') {
         primaAcolada = 1;                                   //Sterge acolada main-ului
         return 0;
     }
     return 1;
 }

 void citire(string &input) {
     stringstream check1(input);
     string intermediate;

     while (getline(check1, intermediate, '\n'))
     {
         if (intermediate.find("\r") != std::string::npos || intermediate == "\r") {
             intermediate = intermediate.substr(0, intermediate.size() - 1);
         }
         if (esteFiller(intermediate))
         {
             cod[contorLinii++] = intermediate;
         }
     }
     contorLinii--;
 }

 //void citire() {
 //    contorLinii = 0;
 //    while (getline(f, cod[contorLinii])) {
 //        if (esteFiller(contorLinii) == 1) {
 //            contorLinii++;
 //        }  
 //    }
 //    contorLinii--;
 //}

 void afisare() {
     for (int i = 0; i < contorLinii; i++) {
         g << cod[i] << '\n';
     }
 }

 void afisare_pseudocod() {
     for (int i = 0; i < contorLinii; i++) {
         g << cod_pseudocod[i] << '\n';
     }  
 }

 void stergeSpatiile() {

     for (int i = 0; i < contorLinii; i++) {
         for (int j = 0; cod[i][j]; j++) {
             if (cod[i][j] == '"' || int(cod[i][j]) == 39) {
                 j++;
                 while (cod[i][j] != '"' and int(cod[i][j]) != 39)      ///  Daca sunt gasite spatii intre ' ' inseamna ca acestea
                     j++;                                               ///nu trebuiesc sterse fiind folosite probabil la afisari
             }
             else if (cod[i][j] == ' ') {
                 cod[i].erase(j, 1);
                 j--;
             }
         }
     }    
 }

 void inlocuireFunctii() {
     for (int i = 0; i < contorLinii; i++) {
         for (int j = 0; j < 9; j++) {
             size_t found = cod[i].find(functii[j]);                         ///  Salvam pozitia primei aparitii a functiei
             while (found != string::npos) {                                 ///daca aceasta este pe acel rand.
                 cod[i].replace(found, functii[j].length(), inlocuiri[j]);
                 found = cod[i].find(functii[j], found + 1);                 ///  Cautam pozitia urmatoarei functii daca aceasta exista
             }                                                               ///pornind de la pozitia ultimei functii gasite +1
         }
     }
         
 }

 void Decodare() {
     stergeSpatiile();
     inlocuireFunctii();
 }

 void Reconstruire() {
     numar_intabulari = 0;
     for (int i = 0; i < contorLinii; i++) {
         ce_functie = 0;
         for (int k = 0; k < 6; k++) {
             if (cod[i][0] == inlocuiri[k][0]) {
                 cod_pseudocod[i] = functii_pseudocod[k];                    ///Inlocuim fiecare functie din C++ in afara de else/else if prescurtata cu echivalentul sau in pseudocod
                 cod[i][0] = ' ';                                            ///Inlocuim in codul C++ functiile cu un spatiu pentru a putea forma mai usor codul in pseudocod
                 ce_functie = ce_functieAcolada = k;
             }
         }
         if (cod[i][0] == inlocuiri[6][0]) {
             if (cod[i][1] == inlocuiri[2][0]) {
                 cod_pseudocod[i] = functii_pseudocod[6] + " " + functii_pseudocod[2];    ///Inlocuim else/else if cu echivalentele lor in pseudocod
                 cod[i].erase(1, 1);
                 ce_functie = ce_functieAcolada = 2;
             }
             else {
                 cod_pseudocod[i] = functii_pseudocod[6];
             } 
             cod[i][0] = ' ';
         }
         cod_pseudocod[i] = cod_pseudocod[i] + cod[i] + functii_pseudocod_continuari[ce_functie];


         for (int j = 0; j < cod_pseudocod[i].length(); j++)
         {
             if (cod_pseudocod[i][j] == '{') {
                 cod_pseudocod[i].clear();
                 stiva_acolade.push(ce_functieAcolada);
                 numar_intabulari++;
             }

             else if (cod_pseudocod[i][j] == '}') {
                 cod_pseudocod[i].replace(j,1,functii_pseudocod_sfarsit[stiva_acolade.top()]);
                 stiva_acolade.pop();
                 numar_intabulari--;
             }

             else if (cod_pseudocod[i][j] == ';') {
                 cod_pseudocod[i].replace(j, 1, " ");                  ///Stergem caracterul ;
             }  

             else if (cod_pseudocod[i][j] == '=') {
                 if (cod_pseudocod[i][j + 1] != '=' && cod_pseudocod[i][j - 1] != '<' && cod_pseudocod[i][j - 1] != '>' && cod_pseudocod[i][j - 1] != '!') {
                     cod_pseudocod[i].replace(j, 1, "<-");              ///Inlocuim '=' cu '<-' si '==' cu '='
                 }
                 else if (cod_pseudocod[i][j + 1] == '=') {
                     cod_pseudocod[i].replace(j, 2, "=");
                 }
             } 

             else if (cod_pseudocod[i][j] == '(' || cod_pseudocod[i][j] == ')') {           ///Stergem parantezele functiilor
                 cod_pseudocod[i].erase(j, 1);
             }  
         }
     }
 }

 vector<string> translareCod(string inputText) {
     citire(inputText);
     Decodare();
     Reconstruire();
     //afisare_pseudocod();
     vector<string> returnValue;
     for (auto aux : cod_pseudocod)
     {
         returnValue.push_back(aux);
     }
     return returnValue;
 }
