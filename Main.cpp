#include<bits/stdc++.h>
#include "Functions.hpp"
using namespace std;

int main(){
    Hospicio hosp(50); //setando o hospicio
    hosp.grafo.gerarRandomPath(50, 33); //seed simples
    showMenu(hosp); //menu
    return 0;
}