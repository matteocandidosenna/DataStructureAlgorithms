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
    Paciente pac;
    bool ocupado;
    
    Quarto(Paciente pac, bool ocupado);
};

class Hospicio{
    public:
    int num_pacientes;
    int num_quartos_ocupados;
    vector<Quarto> quartos;
    
    Hospicio(int num_pacientes, int num_quartos_ocupados, int qtd_quartos);
};



#endif