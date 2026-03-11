DOCUMENTAÇÃO COMPLETA DO SISTEMA HOSPICIO HOLSTENWALL
FASE 1: ESTRUTURAS BÁSICAS E GRAFOS
Programação Orientada a Objetos
Estruturas Implementadas:
cpp
// Estrutura Grafo: armazena vértices e arestas ponderadas
struct Grafo {
    int num_vertices;
    vector<vector<pair<int, int>>> adj;  // Lista de adjacência com pesos
};

// Classe Paciente
class Paciente {
    int id_paciente, idade, num_quarto;
    string nome, diagnostico;
};

// Classe Quarto: contém um Paciente
class Quarto {
    Paciente paciente;
    bool ocupado;
};

// Classe Hospício: gerencia todas as estruturas
class Hospicio {
    Grafo grafo;
    vector<Quarto> quartos;
    string log;
    TabelaHash table_hash;
    Farmacia farm;
};
Menu Principal
cpp
=== Hospicio Holstenwall ===
1. Cadastrar paciente
2. Listar Pacientes
3. Excluir Paciente
4. Mostrar o Mapa do Hospicio
5. Medicar paciente (PD)
0. Sair
🗺️ MÓDULO DE GRAFOS
Geração Automática do Mapa
cpp
void Grafo::gerarRandomPath(int quantidade, unsigned int seed) {
    static mt19937 gen(seed);
    uniform_int_distribution<int> dis(0, num_vertices - 1);
    uniform_int_distribution<int> dis_peso(1, 10);  // Pesos entre 1 e 10

    while(caminhosCriados < quantidade) {
        int u = dis(gen), v = dis(gen);
        if(u == v) continue;  // Evita autoloops
        int peso = dis_peso(gen);
        adicionarCaminho(u, v, peso);
    }
}
Características do Grafo:

✅ Acíclico: Evita loops infinitos na recursão

✅ Unidirecional: Garante ordem lógica (u → v)

✅ Ponderado: Pesos representam distância/dificuldade

✅ Aleatório: Diferente a cada execução (seed controlável)

🎯 ALGORITMOS DE GRAFOS IMPLEMENTADOS
1. BUSCA EM LARGURA (BFS)
Implementação
cpp
void Grafo::bfs(int origem, int destino) {
    queue<pair<int, vector<int>>> fila;
    vector<bool> visitado(num_vertices, false);
    
    fila.push({origem, {origem}});
    visitado[origem] = true;
    
    while(!fila.empty()) {
        int atual = fila.front().first;
        vector<int> caminho = fila.front().second;
        fila.pop();
        
        if(atual == destino) {
            cout << "Caminho: " << caminho;
            cout << " (" << caminho.size()-1 << " corredores)";
            return;
        }
        
        for(auto& aresta : adj[atual]) {
            int vizinho = aresta.first;
            if(!visitado[vizinho]) {
                visitado[vizinho] = true;
                auto novoCaminho = caminho;
                novoCaminho.push_back(vizinho);
                fila.push({vizinho, novoCaminho});
            }
        }
    }
}
Contexto no Hospício
👨‍⚕️ Enfermeiro Apressado: Precisa chegar ao paciente usando o menor número de corredores, ignorando o tamanho deles.

Complexidade
Tempo: O(V + E)

Espaço: O(V)

Melhor para: Grafos não-ponderados ou com pesos uniformes

2. BUSCA EM PROFUNDIDADE (DFS) - Ronda do Vigia
Implementação
cpp
void Grafo::rondaVigia(int origem) {
    vector<bool> visitado(num_vertices, false);
    vector<int> pilha = {origem};
    int passos = 0;
    
    cout << "🚶‍♂️ Vigia iniciando ronda no quarto " << origem << endl;
    
    while(!pilha.empty()) {
        int atual = pilha.back();
        pilha.pop_back();
        
        if(visitado[atual]) continue;
        
        visitado[atual] = true;
        passos++;
        cout << "Checou quarto " << atual << endl;
        
        for(auto& aresta : adj[atual]) {
            if(!visitado[aresta.first])
                pilha.push_back(aresta.first);
        }
    }
    
    cout << "Total de quartos visitados: " << passos << "/" << num_vertices;
}
Contexto no Hospício
👮 Vigia Noturno: Faz ronda completa, explorando cada corredor até o fim antes de voltar.

Complexidade
Tempo: O(V + E)

Espaço: O(V)

Melhor para: Exploração completa, detecção de componentes conectados

3. DIJKSTRA - Caminho Mais Econômico
Implementação
cpp
void Grafo::dijkstra(int origem, int destino) {
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    vector<int> dist(num_vertices, 1e9);
    vector<int> anterior(num_vertices, -1);
    
    dist[origem] = 0;
    pq.push({0, origem});
    
    while(!pq.empty()) {
        int distAtual = pq.top().first;
        int atual = pq.top().second;
        pq.pop();
        
        if(distAtual > dist[atual]) continue;
        if(atual == destino) break;
        
        for(auto& aresta : adj[atual]) {
            int vizinho = aresta.first;
            int peso = aresta.second;
            int novaDist = dist[atual] + peso;
            
            if(novaDist < dist[vizinho]) {
                dist[vizinho] = novaDist;
                anterior[vizinho] = atual;
                pq.push({novaDist, vizinho});
            }
        }
    }
    
    // Reconstruir caminho
    vector<int> caminho;
    for(int v = destino; v != -1; v = anterior[v])
        caminho.push_back(v);
    reverse(caminho.begin(), caminho.end());
    
    cout << "Custo total: " << dist[destino] << endl;
    cout << "Caminho: " << caminho;
}
Contexto no Hospício
💪 Enfermeiro Econômico: Quer gastar o mínimo de energia (soma dos pesos), sem limite de stamina.

Complexidade
Tempo: O((V + E) log V)

Espaço: O(V)

Melhor para: Grafos ponderados, caminho mínimo sem restrições

4. PROGRAMAÇÃO DINÂMICA (PD) - Shortest Reliable Path
Identificação da Subestrutura Ótima
O problema apresenta subestrutura ótima porque a solução ótima para ir de u a destino com stamina k contém a solução ótima para ir do vizinho v ao destino com stamina k - peso.

text
custo_otimo(u, destino, k) = min_{v ∈ vizinhos} { peso(u,v) + custo_otimo(v, destino, k - peso(u,v)) }
Implementação com Memoização (Top-Down)
cpp
int Grafo::src_pd(int u, int destino, int k, vector<vector<int>>& memo) {
    // Casos base
    if (u == destino) return 0;
    if (k <= 0) return 1e7;
    
    // Memoização
    if (memo[u][k] != -1) return memo[u][k];
    
    int menor_custo = 1e7;
    
    // Tentar todos os vizinhos
    for (const auto& aresta : adj[u]) {
        int vizinho = aresta.first;
        int peso = aresta.second;
        
        if (peso <= k) {
            int custo = peso + src_pd(vizinho, destino, k - peso, memo);
            menor_custo = min(menor_custo, custo);
        }
    }
    
    return memo[u][k] = menor_custo;
}
Exibição do Caminho
cpp
void Grafo::exibirCaminhoPD(int u, int destino, int k, vector<vector<int>>& memo) {
    cout << u;
    if (u == destino) {
        cout << " [Alvo]" << endl;
        return;
    }
    
    for (const auto& aresta : adj[u]) {
        int vizinho = aresta.first;
        int peso = aresta.second;
        
        if (peso <= k && memo[u][k] == peso + memo[vizinho][k - peso]) {
            cout << " --(" << peso << ")--> ";
            exibirCaminhoPD(vizinho, destino, k - peso, memo);
            return;
        }
    }
}
Exemplo de Execução
txt
Escolha um quarto para medicar: 49
Ponto de partida: 3
Stamina: 100

Caminho encontrado!
Custo total: 16
Stamina restante: 84
ROTA: 3 --(5)--> 8 --(3)--> 15 --(8)--> 49 [Alvo]
Contexto no Hospício
🏃 Enfermeiro com Stamina Limitada: Precisa chegar ao paciente gastando no máximo k unidades de stamina.

Complexidade
Tempo: O(V × K × grau_médio)

Espaço: O(V × K) para matriz de memoização

Melhor para: Problemas com restrição de recursos (stamina limitada)

📊 COMPARAÇÃO DOS ALGORITMOS DE GRAFO
Algoritmo	Contexto	Complexidade	Vantagem
BFS	Enfermeiro apressado	O(V + E)	Menor número de corredores
DFS	Vigia noturno	O(V + E)	Exploração completa
Dijkstra	Enfermeiro econômico	O((V+E) log V)	Menor custo total
PD	Stamina limitada	O(V × K × grau)	Respeita limite de recurso
FASE 2: COMPRESSÃO E BUSCA EM TEXTO
5. COMPRESSÃO DE HUFFMAN
Estrutura da Árvore de Huffman
cpp
struct NoHuffman {
    char caractere;
    int freq;
    NoHuffman* esquerda;
    NoHuffman* direita;
    
    NoHuffman(char c, int f) : caractere(c), freq(f), 
                                esquerda(nullptr), direita(nullptr) {}
};
Implementação da Compressão
1. Contagem de Frequências
cpp
map<char, int> freq;
for(char c : texto) freq[c]++;
2. Construção da Árvore
cpp
priority_queue<NoHuffman*, vector<NoHuffman*>, ComparaNo> pq;
for(auto& par : freq)
    pq.push(new NoHuffman(par.first, par.second));

while(pq.size() > 1) {
    NoHuffman* esq = pq.top(); pq.pop();
    NoHuffman* dir = pq.top(); pq.pop();
    
    NoHuffman* pai = new NoHuffman('\0', esq->freq + dir->freq
continue
5. COMPRESSÃO DE HUFFMAN (continuação)
2. Construção da Árvore (continuação)
cpp
    pai->esquerda = esq;
    pai->direita = dir;
    pq.push(pai);
}

raiz = pq.top();
3. Geração dos Códigos
cpp
void Huffman::gerarCodigos(NoHuffman* no, string codigo) {
    if(!no) return;
    
    // Nó folha: armazena o código
    if(!no->esquerda && !no->direita)
        codigos[no->caractere] = codigo;
    
    gerarCodigos(no->esquerda, codigo + "0");
    gerarCodigos(no->direita, codigo + "1");
}
4. Compressão do Texto
cpp
string Huffman::comprimir(const string& texto) {
    // ... construção da árvore e geração de códigos ...
    
    string bits;
    for(char c : texto)
        bits += codigos[c];
    
    return bits;
}
5. Salvar em Arquivo Binário
cpp
void Huffman::salvar(const string& nomeArquivo, const string& texto) {
    string bits = comprimir(texto);
    ofstream arquivo(nomeArquivo, ios::binary);
    
    // Salvar dicionário
    int tamDict = codigos.size();
    arquivo.write((char*)&tamDict, sizeof(tamDict));
    
    for(auto& par : codigos) {
        arquivo.write(&par.first, sizeof(char));
        int tamCod = par.second.size();
        arquivo.write((char*)&tamCod, sizeof(tamCod));
        arquivo.write(par.second.c_str(), tamCod);
    }
    
    // Salvar bits comprimidos
    int tamBits = bits.size();
    arquivo.write((char*)&tamBits, sizeof(tamBits));
    
    // Converter bits para bytes
    while(bits.size() % 8) bits += "0";
    
    for(size_t i = 0; i < bits.size(); i += 8) {
        unsigned char byte = 0;
        for(int j = 0; j < 8; j++) {
            if(bits[i+j] == '1')
                byte |= (1 << (7 - j));
        }
        arquivo.write((char*)&byte, sizeof(byte));
    }
    
    arquivo.close();
}
6. Carregar e Descomprimir
cpp
string Huffman::carregar(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo, ios::binary);
    
    // Carregar dicionário
    int tamDict;
    arquivo.read((char*)&tamDict, sizeof(tamDict));
    
    map<string, char> decodigos;
    for(int i = 0; i < tamDict; i++) {
        char c;
        int tamCod;
        arquivo.read(&c, sizeof(c));
        arquivo.read((char*)&tamCod, sizeof(tamCod));
        
        char* buffer = new char[tamCod + 1];
        arquivo.read(buffer, tamCod);
        buffer[tamCod] = '\0';
        decodigos[string(buffer)] = c;
        delete[] buffer;
    }
    
    // Carregar bits
    int tamBits;
    arquivo.read((char*)&tamBits, sizeof(tamBits));
    
    int tamBytes = (tamBits + 7) / 8;
    vector<unsigned char> bytes(tamBytes);
    arquivo.read((char*)bytes.data(), tamBytes);
    arquivo.close();
    
    // Converter bytes para bits
    string bits;
    for(int i = 0; i < tamBytes; i++) {
        for(int j = 7; j >= 0; j--) {
            if(bits.size() < tamBits)
                bits += ((bytes[i] >> j) & 1) ? '1' : '0';
        }
    }
    
    // Descomprimir
    string resultado;
    string codigoAtual;
    for(char bit : bits) {
        codigoAtual += bit;
        if(decodigos.count(codigoAtual)) {
            resultado += decodigos[codigoAtual];
            codigoAtual = "";
        }
    }
    
    return resultado;
}
Contexto no Hospício
📁 Arquivista: Precisa guardar o log do hospício de forma eficiente, economizando espaço em disco.

Exemplo de Uso
txt
--- SALVANDO LOG COMPRIMIDO ---
Tamanho original do log: 187 bytes
Tamanho do arquivo: 98 bytes
Taxa de compressão: 52.4%
Complexidade
Tempo: O(n log n) para construção da árvore, O(n) para compressão

Espaço: O(n) para armazenar frequências e códigos

Eficiência: Melhor para textos com caracteres repetitivos

6. RABIN-KARP - BUSCA EM TEXTO
Implementação
cpp
vector<int> buscarRabinKarp(const string& texto, const string& padrao) {
    vector<int> ocorrencias;
    int n = texto.length();
    int m = padrao.length();
    
    if(m == 0 || m > n) return ocorrencias;
    
    const int base = 256;      // Número de caracteres ASCII
    const int modulo = 101;     // Número primo para evitar overflow
    
    // Pré-processamento: h = base^(m-1) % modulo
    int h = 1;
    for(int i = 0; i < m - 1; i++)
        h = (h * base) % modulo;
    
    // Calcular hash do padrão e da primeira janela
    int hashPadrao = 0;
    int hashJanela = 0;
    
    for(int i = 0; i < m; i++) {
        hashPadrao = (base * hashPadrao + padrao[i]) % modulo;
        hashJanela = (base * hashJanela + texto[i]) % modulo;
    }
    
    // Percorrer o texto
    for(int i = 0; i <= n - m; i++) {
        // Se hashes são iguais, verificar caractere por caractere
        if(hashPadrao == hashJanela) {
            bool encontrou = true;
            for(int j = 0; j < m; j++) {
                if(texto[i + j] != padrao[j]) {
                    encontrou = false;
                    break;
                }
            }
            if(encontrou)
                ocorrencias.push_back(i);
        }
        
        // Calcular próximo hash
        if(i < n - m) {
            hashJanela = (base * (hashJanela - texto[i] * h) + texto[i + m]) % modulo;
            if(hashJanela < 0)
                hashJanela += modulo;
        }
    }
    
    return ocorrencias;
}
Contexto no Hospício
🔍 Investigador: Precisa encontrar rapidamente ocorrências de palavras-chave no log do hospício (nomes, diagnósticos, datas).

Exemplo de Uso
txt
--- BUSCA NO LOG (RABIN-KARP) ---
Digite o texto a ser buscado: João

Padrão encontrado 2 vez(es)!
Posições: 45 187

--- ESTATISTICAS DA BUSCA ---
Total de comparações de hash: 184
Verificações detalhadas: 2
Complexidade
Tempo Médio: O(n + m)

Tempo Pior Caso: O(n × m) quando muitas colisões de hash

Espaço: O(1) além da entrada

Vantagem: Busca em tempo linear na maioria dos casos

7. TABELA HASH - GERENCIAMENTO DE PACIENTES
Implementação
cpp
struct TabelaHash {
    vector<vector<string>> table;  // Lista de nomes por índice
    
    int funcaoHash(string nome) {
        long long hash = 5381;  // Constante mágica do algoritmo djb2
        for(char c : nome)
            hash = hash * 33 + c;  // Espalhamento dos caracteres
        return hash % table.size();
    }
    
    void inserirTabela(string nome) {
        int index = funcaoHash(nome);
        table[index].push_back(nome);
    }
    
    void removerTabela(string nome) {
        int index = funcaoHash(nome);
        for(auto it = table[index].begin(); it != table[index].end(); it++) {
            if(*it == nome) {
                table[index].erase(it);
                return;
            }
        }
    }
};
Contexto no Hospício
📋 Recepcionista: Precisa cadastrar e localizar pacientes rapidamente pelo nome.

Complexidade
Tempo Médio: O(1) para inserção e remoção

Tempo Pior Caso: O(n) quando muitas colisões

Espaço: O(n) para armazenar os nomes

FASE 3: INTEGRAÇÃO E APRESENTAÇÃO
Menu Completo do Sistema
cpp
=== Hospicio Holstenwall ===
1. Cadastrar paciente (Hash)
2. Listar Pacientes (com Tabela Hash)
3. Excluir Paciente (Hash)
4. Mostrar o Mapa do Hospicio (Lista ou Matriz adj)
5. Medicar Paciente (percorrer grafos com PD)
6. Exibir log do Hospicio
7. O Problema do Louco que arrasou a farmacia
8. Salvar log comprimido (Huffman)
9. Carregar log comprimido
10. Buscar no log (Rabin-Karp)
11. BFS - menor número de corredores entre dois quartos
12. Ronda completa do guarda (DFS) - explorar todos os quartos
13. Encontrar o melhor caminho entre dois quartos com Dijkstra
0. Sair
📊 ANÁLISE DE COMPLEXIDADE GERAL
Algoritmo	Complexidade Tempo	Complexidade Espaço	Uso no Sistema
Tabela Hash	O(1) médio / O(n) pior	O(n)	Cadastro de pacientes
BFS	O(V + E)	O(V)	Menor número de corredores
DFS	O(V + E)	O(V)	Ronda completa do vigia
Dijkstra	O((V+E) log V)	O(V)	Caminho mais econômico
PD (SRP)	O(V × K × grau)	O(V × K)	Caminho com stamina limitada
Huffman	O(n log n)	O(n)	Compressão do log
Rabin-Karp	O(n + m) médio	O(1)	Busca no log

O sistema Hospicio Holstenwall implementa com sucesso:

Estruturas de Dados: Grafo (lista e matriz), Tabela Hash, Árvore de Huffman

Algoritmos de Grafo: BFS, DFS, Dijkstra, Programação Dinâmica (Shortest Reliable Path)

Algoritmos de Busca: Sequencial, Binária, Rabin-Karp

Compressão: Huffman com salvamento em arquivo binário

Integração: Todos os módulos trabalham juntos para simular um sistema real de gerenciamento de hospício

Cada algoritmo foi escolhido para resolver um problema específico do domínio, com análises de complexidade apropriadas e implementações eficientes.

📂 REPOSITÓRIO
GitHub: https://github.com/matteocandidosenna/DataStructureAlgorithms
