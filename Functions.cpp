#include "Functions.hpp"
#include <bits/stdc++.h>
using namespace std;

Paciente::Paciente(int id_paciente, int idade, int num_quarto, string nome, string diagnostico){
    this->id_paciente = id_paciente;
    this->num_quarto = num_quarto;
    this->nome = nome;
    this->diagnostico = diagnostico;
}

Quarto::Quarto(Paciente pac, bool ocupado){
    this->pac = pac;
    this->ocupado = ocupado;
}

Paciente::Paciente(int id_paciente, int idade, int num_quarto, string nome, string diagnostico){
    this -> id_paciente = id_paciente;
    this -> idade = idade;
    this -> num_quarto = num_quarto;
    this -> nome = nome;
    this -> diagnostico = diagnostico;
}

//Menu
void showMenu(Hospicio hosp){
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

void inserirPaciente(Hospicio hosp){
    cout << "Digite o numero do quarto em que deseja alocar o paciente (entre 0 e " << hosp.quartos.size() << ") " << endl;
    int num_qt;
    cin >> num_qt;
    if(num_qt > hosp.quartos.size() && num_qt >= 0 && !hosp.quartos[num_qt].ocupado) hosp.quartos[num_qt].ocupado = true;
    else{
        cout << "Quarto ja ocupado! "<< endl;
        return;
    }

    Paciente pac;

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
    
    pac.num_quarto = num_qt;
}



