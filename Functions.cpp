#include "functions.hpp"
#include <bits/stdc++.h>
using namespace std;

Paciente::Paciente(int id_paciente, int idade, int num_quarto, string nome, string diagnostico){
    this -> id_paciente = id_paciente;
    this -> idade = idade;
    this -> num_quarto = num_quarto;
    this -> nome = nome;
    this -> diagnostico = diagnostico;
}


