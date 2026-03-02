#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <bits/stdc++.h>
using namespace std;

struct Grafo {
    int num_vertices;
    vector<vector<int>> adj;

    Grafo(int n) : num_vertices(n), adj(n) {}

    void adicionarCaminho(int u, int v);
};

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
    Grafo grafo;
    
    Hospicio(int num_quartos);
};

//Assinaturas
void showMenu(Hospicio &hosp);
void inserirPaciente(Hospicio &hosp);
void removerPaciente(Hospicio &hosp);
void exibirPacientes(Hospicio &hosp);
#endif