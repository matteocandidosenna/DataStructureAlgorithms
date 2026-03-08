#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <bits/stdc++.h>
using namespace std;

struct TabelaHash{
    vector<vector<string>> table;

    TabelaHash(int num_loucos);
    int funcaoHash(string nome);
    void inserirTabela(string nome);
    void removerTabela(string nome);
};

struct Grafo{
    int num_vertices;
    vector<vector<int>> adj;

    Grafo(int n);

    void adicionarCaminho(int u, int v);
    void gerarRandomPath(int quantidade, unsigned int seed);
    void printAdjacencias();

    int src_pd(int u, int destino, int k, std::vector<std::vector<int>>& memo); //numero de corredores do caminho mais curto
    // u -> onde estou
    // destino -> para onde vou
    // k -> quanto folego resta
    // memo -> matriz de memorização

    void exibirCaminhoPD(int u, int destino, int k, std::vector<std::vector<int>>& memo);
};

class Farmacia{
    public:
    vector<string> remedios;
    Farmacia();
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
    string log;
    Farmacia farm;
    TabelaHash table_hash;
    
    Hospicio(int num_quartos);
};

//Assinaturas
void showMenu(Hospicio &hosp);
void inserirPaciente(Hospicio &hosp);
void removerPaciente(Hospicio &hosp);
void exibirPacientes(Hospicio &hosp);
void medicarPaciente(Hospicio &hosp);   
void exibirLog(Hospicio &hosp);
void farmaciaArrasada(Hospicio &hosp);
#endif