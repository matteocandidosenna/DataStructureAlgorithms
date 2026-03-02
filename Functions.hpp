#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <bits/stdc++.h>
using namespace std;  

//Construtores

class Paciente{
    public:
    int id_paciente;
    int idade;
    int num_quarto;
    string nome;
    string diagnostico;

    Paciente(){}
    Paciente(int id_paciente, int idade, int num_quarto, string nome, string diagnostico);
};

class Quarto{
    public:
    Paciente paciente;
    bool ocupado;
    
    Quarto(Paciente paciente, bool ocupado);
};

class Hospicio{
    public:
    int num_pacientes;
    vector<Quarto> quartos;
    
    Hospicio(int num_quartos);
};

//Assinaturas
void showMenu(Hospicio hosp);
void inserirPaciente(Hospicio hosp);

#endif