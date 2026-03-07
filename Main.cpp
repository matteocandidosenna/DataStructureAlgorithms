#include<bits/stdc++.h>
#include "Functions.hpp"
using namespace std;

int main(){
    cout << "Antes de tudo, antes de mais nada... digite quantos loucos tu acha que existe na UFU..." << endl;
    int loucos;
    cin >> loucos;
    Hospicio hosp(loucos); //setando o hospicio
    hosp.grafo.gerarRandomPath(loucos, 33); //seed simples, para reproducibilidade
    showMenu(hosp); //menu
    return 0;
}