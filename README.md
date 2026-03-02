# Apresentação do Software: Data Structure Algorithms

## Programação Orientada a Objeto, para organização das entidades, compreensão e manutenibilidade
#### Estrutura Grafo: armazena os atributos e métodos que um grafo deve possuir;
#### Classe Paciente;
#### Classe Quarto: contém um objeto da Classe Paciente;
#### Classe Hospício: que contém um objeto de estrutura Grafo, um vetor de objetos da classe Quarto; 

## Menu: no menu é possível: 
```c++
cout << "=== Hospicio Holstenwall ===" << endl;
    cout << "1. Cadastrar paciente" << endl;
    cout << "2. Listar Pacientes" << endl;
    cout << "3. Excluir Paciente" << endl;
    cout << "4. Mostrar o Mapa do Hospicio" <<endl;
    cout << "5. Medicar paciente" <<endl;
    cout << "0. Sair" <<endl;
```
As três primeiras opções são triviais, mas a quarta e quinta não...

## Mostrar mapa do Hospício:
No main, temos:
```c++
int main(){
    Hospicio hosp(50); //setando o hospicio
    hosp.grafo.gerarRandomPath(50, 33); //seed simples
    showMenu(hosp); //menu
    return 0;
}
```
Podemos observar a função gerar RandomPath, que recebe a quantidade de quartos do hospício (vertices do grafo) e a seed para reproducilidade.
Seu escopo é o seguinte: 
```c++
void Grafo::gerarRandomPath(int quantidade, unsigned int seed){
    static std::mt19937 gen(seed); //gerando aleatoriedade da seed
    uniform_int_distribution<int> dis(0, num_vertices - 1); //garante que o numero gerado esteja no intervalo valido

    int caminhosCriados = 0;
    while(caminhosCriados < quantidade){
        int u = dis(gen); // sorteia o primeiro quarto
        int v = dis(gen); // sorteia o segundo quarto

        //evitando ciclos, fazendo com que quartos de menor numeração se liguem apenas a quartos de maior numeração
        //trata-se de um grafo acíclico
        if(u < v){
            adicionarCaminho(u, v);
            caminhosCriados++;
        }
    }
}
```

## Evitando Loops Infinitos com Algoritmos Recursivos
Deste modo, produzimos um mapa/grafo do hospício de maneira automática, sem ter que definir as adjacências. O grafo é acíclico e unidirecional para garantir a subestrutura ótima da PD e evitar loops infinitos em recursão (como no Shortest Reliable Path). Com ciclos, a função tentaria calcular o caminho de A, que pede o de B, que pede o de A novamente... Isso causaria um Stack Overflow (estouro da memória) e o programa travaria.

## Garantindo Subestrutura ótima
A PD baseia-se na ideia de que você pode resolver um problema maior usando soluções de problemas menores que já foram resolvidos.
- Em um grafo unidirecional (sempre para números maiores), quando você está no Quarto 40, você tem certeza de que as decisões tomadas nos quartos 41, 42, 43 etc., são finais e não serão alteradas por algo que acontecer "lá atrás" no Quarto 10.
- Existe uma ordem lógica de dependência.

## Shortest Reliable Path (Caminho com Restrição de Saltos):
```c++
int Grafo::src_pd(int u, int destino, int k, std::vector<std::vector<int>>& memo) {
    //caso Base: Chegou no destino? Distância é zero.
    if (u == destino) return 0;

    //caso base sem folego/stamina: caminho impossível
    if (k <= 0) return 1e7; //um numero muy longo, mas nem tanto,pq int max causa estouro

    //PD (memoização): Ja calculamos isso antes para este quarto e esta stamina?
    if (memo[u][k] != -1) return memo[u][k];

    int menor_distancia = 1e7;

    //recusão tenta todos os vizinhos
    for (int vizinho : adj[u]) {
        int dist = 1 + src_pd(vizinho, destino, k - 1, memo);
        if (dist < menor_distancia) {
            menor_distancia = dist;
        }
    }
    // salva na matriz e retorna
    return memo[u][k] = menor_distancia;
}
```

Um ponto importante no algoritmo acima: o uso de 1e7, que é 1*10^7. Este número serve para distinguirmos se há caminho ou não. Pensei em usar INT_MAX, mas a operações de soma estouram o limite. Então, utilizei um numero alto sem ser infinito e maior que a stamina. Deste modo podemos checar de maneira simples se foi possível obter caminho sem cansar

## Shortest Reliable Path - Traçando a rota:
Finalmente, usando a tabela de memorização produzida pela função anterior, fazemos o seguinte para verificar o caminho produzido:
```c++
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
```
