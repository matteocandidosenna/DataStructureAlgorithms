#include "Functions.hpp"
#include <bits/stdc++.h>
using namespace std;

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

Hospicio::Hospicio(int num_quartos){
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
    cout << "4. Editar paciente" <<endl;
    int op;
    cin >> op;

    switch(op){
        case 1:{
            //funcao cadastrar
        }
        case 2:{
            //funcao listar
        }

        case 3:{
            //funcao excluir
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

    cout << "Digite o diagnostico do paciente: " << endl;
    string diag;
    getline(cin, diag);
    pac.diagnostico = diag;
    cout << endl;

    pac.id_paciente = hosp.num_pacientes; //ID auto increment
    
    pac.num_quarto = num_qt;
    hosp.quartos[num_qt].ocupado = true;
    hosp.quartos[num_qt].paciente = pac;

    hosp.num_pacientes++;
    return;
}

void removerPaciente(Hospicio &hosp){
    cout << "Lista dos paciente atualmente alocados no hospicio: " <<endl;
    for(auto quartos : hosp.quartos){
        cout << "ID:" <<quartos.paciente.id_paciente <<", nome: "<< quartos.paciente.nome << endl;
    }
    cout << "Digite o ID do paciente que queres dar alta: ";
    int id;
    cin >> id;
    
    //busca sequencial
    for(int i =0; i < hosp.quartos.size(); i++){
        if(hosp.quartos[i].paciente.id_paciente == id){
            hosp.quartos[i].ocupado = false;
            Paciente pac(0, 0, 0, "nenhum", "nenhum");
            hosp.quartos[i].paciente = pac;
            hosp.num_pacientes --;
        }
    }

    return;
}

