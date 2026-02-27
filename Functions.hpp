#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <bits/stdc++.h>
using namespace std;

class Paciente{
    int id_paciente;
    int idade;
    int num_quarto;
    string nome;
    string diagnostico;

    Paciente(int id_paciente, int idade, int num_quarto, string nome, string diagnostico); 
};

class Hospicio{
    int num_pacientes;
    int num_quartos_ocupados;
    
};

#endif