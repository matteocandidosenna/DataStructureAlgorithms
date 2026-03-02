#include "Functions.hpp"
#include <bits/stdc++.h>
using namespace std;

Grafo::Grafo(int n) : num_vertices(n), adj(n) {} 

int Grafo::src_pd(int u, int destino, int k, std::vector<std::vector<int>>& memo) {
    //caso Base: Chegou no destino? Distância é zero.
    if (u == destino) return 0;

    //caso base sem folego/stamina: caminho impossível
    if (k <= 0) return 1e7; //um numero muy longo, mas nem tanto,pq int max causa estouro

    //PD (memoização): Ja calculamos isso antes para este quarto e esta stamina?
    if (memo[u][k] != -1) return memo[u][k];

    int menor_distancia = 1e7;

    // 4. recusão: Tenta todos os vizinhos
    for (int vizinho : adj[u]) {
        int dist = 1 + src_pd(vizinho, destino, k - 1, memo);
        if (dist < menor_distancia) {
            menor_distancia = dist;
        }
    }

    // 5. Salva na tabela e retorna
    return memo[u][k] = menor_distancia;
}

void Grafo::exibirCaminhoPD(int u, int destino, int k, std::vector<std::vector<int>>& memo) {
    std::cout << u;
    if (u == destino) {
        std::cout << " [Alvaro]" << std::endl;
        return;
    }
    std::cout << " -> ";

    for (int vizinho : adj[u]) {
        // se adistncia saindo do vizinho for exatamente (minha_distancia - 1),
        // descobrimos que este vizinho foi a escolha ótima da PD!
        if (k > 0 && memo[u][k] == 1 + memo[vizinho][k - 1]) {
            exibirCaminhoPD(vizinho, destino, k - 1, memo);
            return; 
        }
    }
}

void Grafo::adicionarCaminho(int u, int v) {
    adj[u].push_back(v); //unidirecional, propicio para PD
}

void Grafo::gerarRandomPath(int quantidade, unsigned int seed){
    static std::mt19937 gen(seed); //segunda fase da aleatoriedade
    uniform_int_distribution<int> dis(0, num_vertices - 1); //garante que o numero gerado esteja no intervalo valido

    int caminhosCriados = 0;
    while(caminhosCriados < quantidade){
        int u = dis(gen); // sorteia o primeiro quarto
        int v = dis(gen); // sorteia o segundo quarto

        //evitando ciclos
        if(u < v){
            adicionarCaminho(u, v);
            caminhosCriados++;
        }
    }
}

void Grafo::printAdjacencias(){
    cout << endl << "--- Mapa de Conexoes do Hospicio: ---" <<endl;

    for(int i = 0; i < num_vertices; i++){

        //numero do quarto atual
        cout << "Quarto [" << i << "]: ";

        //percorre lista de vizinhos do quarto i
        for(int vizinho : adj[i]){
            cout << "-> " << vizinho <<" ";
        }

        //se o quarto não possuir vizinhos
        if(adj[i].empty()) cout << "(Isolado)";
        cout << endl;
    }
    cout << "----------------------------------" << endl << endl;
}

Paciente::Paciente(int id_paciente, int idade, int num_quarto, string nome, string diagnostico){
    this->id_paciente = id_paciente;
    this->num_quarto = num_quarto;
    this->nome = nome;
    this->diagnostico = diagnostico;
}

Quarto::Quarto(Paciente paciente, bool ocupado){
    this->paciente = paciente;
    this->ocupado = ocupado;
}

Hospicio::Hospicio(int num_quartos)
    : grafo(num_quartos), num_pacientes(0){ //inicializando o grafo
    for (int i = 0; i < num_quartos - 1; i++) {
        grafo.adicionarCaminho(i, i + 1); // garante pelo menos um corredor entre cada quarto
    }    
    this->num_pacientes = 0;
    Paciente pac(0, 0, 0, "nenhum", "nenhum");
    Quarto qrt(pac, false);
    quartos.assign(num_quartos, qrt);
    // cria um hospicio com quartos padrão contendo pacientes padrão
}

//Menu
void showMenu(Hospicio &hosp){
    cout << "=== Hospicio Holstenwall ===" << endl;
    cout << "1. Cadastrar paciente" << endl;
    cout << "2. Listar Pacientes" << endl;
    cout << "3. Excluir Paciente" << endl;
    cout << "4. Mostrar o Mapa do Hospicio" <<endl;
    cout << "5. Medicar paciente" <<endl;
    cout << "0. Sair" <<endl;
    int op;
    cin >> op;

    switch(op){
        case 1:{
            inserirPaciente(hosp);
            break;
        }
        case 2:{
            exibirPacientes(hosp);
            break;
        }

        case 3:{
            removerPaciente(hosp);
            break;
        }

        case 4:{
            hosp.grafo.printAdjacencias();
            showMenu(hosp);
        }

        case 5:{
            medicarPaciente(hosp);
            showMenu(hosp);
        }

        case 0: return;

        default:{
            cout << "Digite um numero valido! "<<endl;
            showMenu(hosp);
        }
    }
}

void inserirPaciente(Hospicio &hosp){
    if(hosp.num_pacientes == hosp.quartos.size()){
        cout << "Hospicio cheio, nao ha vagas mais!" << endl;
        return;
    }

    Paciente pac;
    cout << "Digite o numero do quarto em que deseja alocar o paciente (entre 0 e " << hosp.quartos.size() << ") " << endl;
    int num_qt;
    cin >> num_qt;
    if(num_qt < 0 || num_qt >= hosp.quartos.size() || hosp.quartos[num_qt].ocupado){
        cout << "Quarto invalido ou ja ocupado!" << endl;
        return;
    }

    cin.ignore();
    cout << "Digite o nome do paciente: ";
    string name;
    getline(cin, name);
    pac.nome = name;
    cout << endl;

    cout << "Digite a idade do paciente: ";
    int age;
    cin >> age;
    pac.idade = age;
    cout <<endl;

    cin.ignore();
    cout << "Digite o diagnostico do paciente: " << endl;
    string diag;
    getline(cin, diag);
    pac.diagnostico = diag;
    cout << endl;

    pac.id_paciente = hosp.num_pacientes + 1; //ID auto increment
    
    pac.num_quarto = num_qt;
    hosp.quartos[num_qt].ocupado = true;
    hosp.quartos[num_qt].paciente = pac;

    hosp.num_pacientes++;
    cout << endl;
    showMenu(hosp);
}

void removerPaciente(Hospicio &hosp){
    cout << "Lista dos paciente atualmente alocados no hospicio: " <<endl;
    for(const auto &quartos : hosp.quartos){
        if(quartos.paciente.nome != "nenhum"){
            cout << "ID: " <<quartos.paciente.id_paciente <<", nome: "<< quartos.paciente.nome << endl;
        }
        
    }
    cout << "Digite o ID do paciente que queres dar alta: ";
    int id;
    cin >> id;
    
    //busca sequencial
    bool encontrado = false;
    for(int i =0; i < hosp.quartos.size(); i++){
         if(hosp.quartos[i].ocupado && hosp.quartos[i].paciente.id_paciente == id){
            hosp.quartos[i].ocupado = false;
            hosp.quartos[i].ocupado = false;
            Paciente pac(0, 0, 0, "nenhum", "nenhum");
            hosp.quartos[i].paciente = pac;
            hosp.num_pacientes --;
            encontrado = true;
        }
    }
    if(!encontrado) cout << "Paciente nao encontrado!" << endl <<endl;
    showMenu(hosp);
}

void exibirPacientes(Hospicio &hosp){
    if(hosp.num_pacientes == 0){
        cout << "Nao ha pacientes internados! " <<endl;
        showMenu(hosp);
    }
    else{
        for(const auto &elemento : hosp.quartos){
            if(elemento.paciente.nome != "nenhum"){
                cout << endl << "Nome: " << elemento.paciente.nome <<endl;
                cout << "Diagnostico: " << elemento.paciente.diagnostico <<endl;
                cout << "Idade:" <<elemento.paciente.idade <<endl <<endl;
            }
        }
    }
    
    cout << endl;
    showMenu(hosp);
}

void medicarPaciente(Hospicio &hosp){
    cout << "Escolhe um quarto entre 0 e " <<hosp.quartos.size() <<" no qual desejas medicar o paciente: ";
    int nqt;
    cin >> nqt;
    cout << endl;

    cout << "Digite o seu ponto de partida: ";
    int ptp;
    cin>>ptp;
    cout << endl;

    cout <<"Dogite a sua stamina, quantos corredores / tuneis voce quer andar no maximo";
    int stm;
    cin>>stm;
    cout <<endl;

    vector<vector<int>> memo(hosp.quartos.size(), vector<int>(stm + 1, -1));
    int distancia = hosp.grafo.src_pd(ptp, nqt, stm, memo);

    if(distancia >= 1e7){
        cout << "Nao ha caminho, o paciente está fora de alcance com essa stamina!" <<endl;
    }else{
        cout << "Caminho encontrado com " << distancia << " tuneis/corredores " << endl;
        cout << "ROTA: ";
        hosp.grafo.exibirCaminhoPD(ptp, nqt, stm, memo);
        cout << nqt;
        cout <<endl;
    }
}