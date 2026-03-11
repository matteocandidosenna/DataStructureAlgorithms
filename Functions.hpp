#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <bits/stdc++.h>
using namespace std;

struct NoHuffman{
    char caractere;
    int freq;
    NoHuffman* esquerda;
    NoHuffman* direita;

    NoHuffman(char c, int f) : caractere(c), freq(f), esquerda(nullptr), direita(nullptr){}
};

struct ComparaNo{
    bool operator()(NoHuffman* a, NoHuffman* b){ //operator(): Permite que o objeto seja "chamado" como se fosse uma função (ex: objeto(a, b)).
        return a -> freq > b -> freq;
    }
};

class Huffman{
    private: 
    NoHuffman* raiz;
    map<char, string> codigos;
    
    void gerarCodigos(NoHuffman* no, string codigo);
    void destruir(NoHuffman* no);

    public:
    Huffman();
    ~Huffman(); //~ é um destrutor Definir funções que limpam recursos de um objeto antes dele ser deletado

    string comprimir(const string& texto);
    void salvar(const string& nomeArquivo, const string& texto);
    string carregar(const string& nomeArquivo);

};

struct TabelaHash{
    vector<vector<string>> table;

    TabelaHash(int num_loucos);
    int funcaoHash(string nome);
    void inserirTabela(string nome);
    void removerTabela(string nome);
};

struct Grafo{
    int num_vertices;
    vector<vector<pair<int, int>>> adj;

    Grafo(int n);

    void adicionarCaminho(int u, int v, int peso);
    void gerarRandomPath(int quantidade, unsigned int seed);
    void printAdjacencias();//lista
    void printMatrizAdjacencia();

    int src_pd(int u, int destino, int k, vector<vector<int>>& memo); //numero de corredores do caminho mais curto
    // u -> onde estou
    // destino -> para onde vou
    // k -> quanto folego resta
    // memo -> matriz de memorização

    void exibirCaminhoPD(int u, int destino, int k, vector<vector<int>>& memo);


    //BFS
    void bfs(int origem, int destino);

    //DFS
    void rondaVigia(int origem);

    //DIJSKTRA
    void dijkstra(int origem, int destino);
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
    void salvarLogComprimido();
    void carregarLogComprimido();
    void buscarNoLog();
};

//Assinaturas
void showMenu(Hospicio &hosp);
void inserirPaciente(Hospicio &hosp);
void removerPaciente(Hospicio &hosp);
void exibirPacientes(Hospicio &hosp);
void medicarPaciente(Hospicio &hosp);   
void exibirLog(Hospicio &hosp);
void farmaciaArrasada(Hospicio &hosp);
vector<int> buscarRabinKarp(const string& texto, const string& padrao);
#endif