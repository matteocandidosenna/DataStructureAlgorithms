#include "Functions.hpp"
#include <bits/stdc++.h>
using namespace std;
//Compressao HUffman
Huffman::Huffman() : raiz(nullptr){}

Huffman::~Huffman(){
    destruir(raiz);
}

void Huffman::destruir(NoHuffman* no){
    if(no){
        destruir(no -> esquerda);
        destruir(no -> direita);
        delete no;
    }
}

void Huffman::gerarCodigos(NoHuffman* no, string codigo){
   if(!no) return;

   if(!no -> esquerda && !no -> direita) codigos[no->caractere] = codigo;

   gerarCodigos(no->esquerda, codigo + "0");
   gerarCodigos(no->direita, codigo + "1");
}

string Huffman::comprimir(const string& texto){
    if(texto.empty()) return "";

    //contandofrequencias
    map<char, int> freq;
    for(char c : texto) freq[c]++;

    //criar fila de prioridade
    priority_queue<NoHuffman*, vector<NoHuffman*>, ComparaNo> pq;
    for(auto& par: freq){
        pq.push(new NoHuffman(par.first, par.second));
    }

    //construir arvore
    while(pq.size() > 1){
        NoHuffman* esq = pq.top(); pq.pop();
        NoHuffman* dir = pq.top(); pq.pop();

        NoHuffman* pai = new NoHuffman('\0', esq->freq + dir->freq);
        pai->esquerda = esq;
        pai->direita = dir;

        pq.push(pai);

    }

    raiz = pq.top();

    //gerar codigos
    codigos.clear();
    gerarCodigos(raiz, "");

    //comprimir
    string bits;
    for(char c: texto){
        bits += codigos[c];
    }

    return bits;

}

void Huffman::salvar(const string& nomeArquivo, const string& texto){
    string bits = comprimir(texto);

    ofstream arquivo(nomeArquivo, ios::binary);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    //salvar dicionario
    int tamDict = codigos.size();
    arquivo.write((char*)&tamDict, sizeof(tamDict));

    for (auto& par : codigos){
        arquivo.write(&par.first, sizeof(char));
        int tamCod = par.second.size();
        arquivo.write((char*)&tamCod, sizeof(tamCod));
        arquivo.write(par.second.c_str(), tamCod);
    }

    //salvar bits
    int tamBits = bits.size();
    arquivo.write((char*)&tamBits, sizeof(tamBits));

    //converter bits para bytes
    while(bits.size() % 8) bits+="0";

    for(size_t i = 0; i<bits.size(); i+=8){
        unsigned char byte = 0;
        for(int j =0; j <8; j++){
            if(bits[i+j] == '1') byte |= (1 << (7 - j));
        }
        arquivo.write((char*)&byte, sizeof(byte));
    }

    arquivo.close();
    cout << "Log salvo em " << nomeArquivo <<endl;
}

string Huffman::carregar(const string& nomeArquivo){
    ifstream arquivo(nomeArquivo, ios::binary);
    if(!arquivo.is_open()){
        cout << "Erro ao carregar arquivo" << endl;
        return "";
    }

    //carregar dicionario
    int tamDict;
    arquivo.read((char*)&tamDict, sizeof(tamDict));

    map<string, char> decodigos;
    for(int i =0; i<tamDict; i++){
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

    //Carregar bits
    int tamBits;
    arquivo.read((char*)&tamBits, sizeof(tamBits));

    int tamBytes = (tamBits + 7) / 8;
    vector<unsigned char> bytes(tamBytes);
    arquivo.read((char*)bytes.data(), tamBytes);

    arquivo.close();

    //converter bytes para bits
    string bits;
    for(int i = 0; i < tamBytes; i++){
        for(int j = 7; j >= 0; j--){
            if(bits.size() < tamBits){
                bits += ((bytes[i]>>j)& 1 ) ? '1' : '0';
            }
        }
    }

    //Descomprimir
    string resultado;
    string codigoAtual;
    for(char bit : bits){
        codigoAtual += bit;
        if(decodigos.count(codigoAtual)){
            resultado += decodigos[codigoAtual];
            codigoAtual ="";
        }
    }

    return resultado;
}

//Métodos de Classes

void Grafo::dijkstra(int origem, int destino){
    //fila de prioridade: (distancia acumulada, vertice)
    //usamos a pq com maior distancia no topo, entao negativamos
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    //vetor de distancias
    vector<int>dist(num_vertices, 1e9);

    //vetor para guardar o caminho antecessor de cada vertice
    vector<int> anterior(num_vertices, -1);

    //inicializa
    dist[origem] = 0;
    pq.push({0, origem});

    cout << "\n DIJKSTRA procurands o caminho mais economico" << endl;
    cout << "Do quarto " << origem << " ate o quarto " <<destino <<endl;
    cout << "==============================="<< endl;
    
    while(!pq.empty()){
        int distanciaAtual = pq.top().first;
        int atual = pq.top().second;
        pq.pop();

        //se a distancia atual for maior que a registrada, ignora
        if(distanciaAtual > dist[atual]) continue;

        cout << "Visitando o quarto " << atual << " (custo acumulado: " << distanciaAtual << ")"<<endl;

        //se chegou no destino, podemos parar (otimizacao)
        if(atual == destino) break;

        //explorando os vizinhos
        for(const auto& aresta : adj[atual]){
            int vizinho = aresta.first;
            int peso = aresta.second;

            int novaDistancia = dist[atual] + peso;

            //se encontrou um caminh melhor
            if(novaDistancia < dist[vizinho]){
                dist[vizinho] = novaDistancia;
                anterior[vizinho] = atual;
                pq.push({novaDistancia, vizinho});
                cout << "Indo para o quarto " << vizinho << "(custo : " << distanciaAtual << " + " <<peso;
                cout << " = " << novaDistancia << ")" << endl;
            }
        }
    }

    //mostra o resultado
    cout <<"\n================================="<<endl;

    if(dist[destino]==1e9){
        cout << "nao existe caminho de " << origem << " ate " << destino <<endl;
    }else{
        cout << "CAMINHO MAIS RAPIDO ENCONTRADO!" << endl;
        cout << "custo total (soma dos pesos) de " << dist[destino] << endl;

        //reconstruindo o caminho
        vector<int>caminho;
        int passo = destino;
        while(passo != -1){
            caminho.push_back(passo);
            passo  = anterior[passo];
        }

        cout << "Caminho: "<<endl;
        for(int i = caminho.size() -1 ;i>=0; i-- ){
            cout << caminho[i];
            if(i > 0){
                //mostra o peso da aresta
                for(const auto& aresta : adj[caminho[i]]){
                    if(aresta.first == caminho[i-1]){
                        cout << "--(" <<aresta.second << ")-->";
                        break;
                    }
                }
            }
        }
        cout <<endl;
    }
}

void Grafo::rondaVigia(int origem){
    //marcando os quartos visitados 
    vector<bool> visitado(num_vertices, false);

    //pilha do dfs com vetor
    vector<int>pilha;

    //comeca a ronda pelo quarto escolhido
    pilha.push_back(origem);

    cout << "\n VIGA NOTURNO INICIANDO A RONDA..." << endl;
    cout << "ponto de partida no quarto " << origem<< endl;
    cout <<" ----------------------------------------- " <<endl;

    int passos = 0;

    while(!pilha.empty()){
        int atual = pilha.back();
        pilha.pop_back();

        //se ja visitou, pula
        if(visitado[atual]) continue;

        //marca como visitado e registra a visita
        visitado[atual] = true;
        passos++;
        cout << "Vigia checou o quarto" << atual <<endl;

        //adiciona os vizinhos na pilha (pra explorar em profundidade)
        for(const auto& aresta : adj[atual]){
            int vizinho = aresta.first;
            if(!visitado[vizinho]){
                pilha.push_back(vizinho);
                cout << "corredor para quarto " << vizinho << " (sera explorado) " <<endl;
            }
        }
    }

      // Estatísticas da ronda
    cout << "\nRONDA CONCLUIDA!" << endl;
    cout << "--------------------------------" << endl;
    cout << "Total de quartos visitados: " << passos << " de " << num_vertices << endl;
}

void Grafo::bfs(int origem, int destino){
    //fila para bfs que guarda o quarto e o caminho até ele
    queue<pair<int, vector<int>>> fila;

    //vetor para marcar os visitados
    vector<bool> visitado(num_vertices, false);

    //comeca pela origem
    vector<int>caminhoInicial;
    caminhoInicial.push_back(origem);
    fila.push({origem, caminhoInicial});
    visitado[origem] = true;

    cout << "\n BFS: Procurando caminho de " << origem << " ate " << destino <<endl;

    while(!fila.empty()){
        int atual = fila.front().first;
        vector<int> caminhoAtual = fila.front().second;
        fila.pop();

        cout << "visitando quarto " << atual <<endl;

        //se chegou no destino, mostra o caminho
        if(atual == destino){
            cout << "Caminho encontrado!" << endl;
            cout << "Numero de corredores: " << caminhoAtual.size() - 1<<endl;
            cout << "Caminho: ";
            for(int i=0; i<caminhoAtual.size(); i++){
                cout << caminhoAtual[i];
                if(i < caminhoAtual.size() - 1) cout << "->";
            }
            cout <<endl;
            return;
        }

        //explorando os vizinhos
        for(const auto& aresta: adj[atual]){
            int vizinho = aresta.first;

            if(!visitado[vizinho]){
                visitado[vizinho] = true;


                //cria nova caminho adicionado ao vizinho
                vector<int>novoCaminho = caminhoAtual;
                novoCaminho.push_back(vizinho);

                fila.push({vizinho, novoCaminho});
            }
        }
    }
    cout << "Nao ha caminhao de " << origem << " para "<< destino <<endl;
}

void Grafo::printMatrizAdjacencia(){
    cout << endl << "=== MATRIZ DE ADJACeNCIA ===" << endl;
    cout << "Valor na posicao [i][j] = peso da aresta i -> j" << endl;
    cout << endl;

    //criar matriz preenchida com 0
    vector<vector<int>>matriz(num_vertices, vector<int>(num_vertices, 0));

    //preencher com os pesos das arestas
    for(int i = 0; i < num_vertices; i++){
        for(const auto& aresta : adj[i]){
            int vizinho  = aresta.first;
            int peso = aresta.second;
            matriz[i][vizinho] = peso;
        }
    }

    cout << "   ";
    for(int j = 0; j < num_vertices; j++){
        cout << setw(3) << j << " ";
    }
    cout <<endl;

    //divisoria
    cout << "   ";
    for(int j = 0; j<num_vertices; j++){
        cout <<"-------";
    }

    cout <<endl;

    //imprimir cada linha da matriz
    for(int i =0; i < num_vertices; i++){
        cout << setw(2) << i << " |"; //indice da linha

        for(int j = 0; j<num_vertices; j++){
            if(matriz[i][j] !=0){
                cout << setw(3) << matriz[i][j] << " "; //mostra o peso
            }else{
                cout <<setw(3) << "." << " "; //ponto para zero  
            }
        }
        cout <<endl;
    }

    cout << "=======================" << endl;


}

void Hospicio::buscarNoLog(){
    if(log.empty()){
        cout << "Log vazio, nada para buscar!" << endl;
        return;
    }

    cout << "\n--- BUSCA NO LOG (RABIN-KARP) ---" << endl;
    cout << "Digite o texto a ser buscado: ";
    cin.ignore();
    string padrao;
    getline(cin, padrao);

    if(padrao.empty()){
        cout << "padrao vazio!" <<endl;
        return;
    }

    //executa a busca
    vector<int> posicoes = buscarRabinKarp(log, padrao);

    //mostra resultados da busca
    if(posicoes.empty()) cout << "\n padrao nao encontrado no log" << endl;
    else{
        cout << "\nPadrao encontrado " << posicoes.size() << " vez(es)!" << endl;
        cout << "Posicoes: ";
        for (int pos : posicoes) {
            cout << pos << " ";
        }
        cout << endl;
    }

    // Estatísticas adicionais
    cout << "\n--- ESTATISTICAS DA BUSCA ---" << endl;
    cout << "Total de comparações de hash: " << (log.length() - padrao.length() + 1) << endl;
    cout << "Verificacoes detalhadas: " << posicoes.size() << endl;
}

void Hospicio::salvarLogComprimido(){
    if(log.empty()){
        cout << endl << "Log vazio! Nao ha o que salvar!" <<endl;
        return;
    }

    cout << "\n--- SALVANDO LOG COMPRIMIDO ---" << endl;
    cout << "Tamanho original do log: " << log.length() << " bytes" << endl;
    cout << "Conteudo do log:" << endl;
    cout << "================================" << endl;
    cout << log << endl;
    cout << "================================" << endl;

    Huffman compressor;
    compressor.salvar("log_hospicio.bin", log);

    //estatisticas do arquivo
    ifstream arquivo("log_hospicio.bin", ios::binary | ios::ate);
    if (arquivo.is_open()) {
        int tamanhoArquivo = arquivo.tellg();
        cout << "\nLog comprimido salvo em 'log_hospicio.bin'" << endl;
        cout << "Tamanho do arquivo: " << tamanhoArquivo << " bytes" << endl;
        cout << "Taxa de compressao: " 
             << (100.0 * tamanhoArquivo / log.length()) << "%" << endl;
        arquivo.close();
    }
}

void Hospicio::carregarLogComprimido(){
    cout << endl << "CARREGANDO LOG COMPRIMIDO" <<endl;

    Huffman compressor;
    string logCarregado = compressor.carregar("log_hospicio.bin");

    if(logCarregado.empty()){
        cout <<endl<< "Erro ao carregar log ou arquivo vazio!" <<endl;
        return;
    }

    cout << "Log carregado com sucesso!" << endl;
    cout << "Tamanho: " << logCarregado.length() << " bytes" << endl;
    cout << "\n--- CONTEuDO DO LOG ---" << endl;
    cout << logCarregado << endl;
    cout << "------------------------" << endl;

    cout << "Deseja substituir o log atual pelo log carregado? (1-Sim / 0-Nao): ";
    int op;
    cin >> op;
    if (op == 1) {
        log = logCarregado;
        cout << "Log substituido com sucesso!" << endl;
    }
}

void TabelaHash::inserirTabela(string nome){
    int index = funcaoHash(nome);
    table[index].push_back(nome);
}

void TabelaHash::removerTabela(string nome){
    int index = funcaoHash(nome);
    for(auto it = table[index].begin(); it != table[index].end(); it++){
        if(*it == nome) table[index].erase(it);
        return;
    }
    return;
}

int TabelaHash::funcaoHash(string nome){
    long long hash = 5381;
    for(auto c : nome){
        hash = hash * 33 + c;
    }

    return hash % table.size();
}

int Grafo::src_pd(int u, int destino, int k, vector<vector<int>>& memo) {
    //caso Base: Chegou no destino? Distância é zero.
    if (u == destino) return 0;

    //caso base sem folego/stamina: caminho impossível
    if (k <= 0) return 1e7; //stamina negativa

    //ja calculamos isso antes?
    if (memo[u][k] != -1) return memo[u][k];

    int menor_custo = 1e7;

    //tenta todos os vizinhos
    for (const auto& aresta : adj[u]) {
        int vizinho = aresta.first;
        int peso = aresta.second;
        
        //só pode ir se tiver stamina suficiente para esta aresta
        if (peso <= k) {    
            //gasta o peso da aresta da stamina
            int custo = peso + src_pd(vizinho, destino, k - peso, memo);

            if(custo < menor_custo) menor_custo = custo;
        }
    }

    // salva na matriz e retorna
    return memo[u][k] = menor_custo;
}

void Grafo::exibirCaminhoPD(int u, int destino, int k, vector<vector<int>>& memo) {
    if (u == destino) {
        cout << " [Alvo]" << endl;
        return;
    }

    //verifica se tem stamina ainda
    if(k<=0){
        cout << "Stamina esgotada! " << endl;
        return;
    } 

    for (const auto& aresta : adj[u]) {
        int vizinho = aresta.first;
        int peso = aresta.second;
        
        //verifica se este vizinho foi a escolha ótima
        if (peso <= k && memo[u][k] == peso + memo[vizinho][k - peso]) {
            //mostra o atual e o peso da aresta que vai usar
            if(u == destino) cout << destino;
            else cout << u << " --(" << peso << ")--> ";
            
            // Continua a partir do vizinho (que PODE ser o destino)
            exibirCaminhoPD(vizinho, destino, k - peso, memo);
            return;
        }
    }
}

void Grafo::adicionarCaminho(int u, int v, int peso) {
    adj[u].push_back({v, peso}); //unidirecional, propicio para PD
}

void Grafo::gerarRandomPath(int quantidade, unsigned int seed){     
    //aleatoriedade
    static mt19937 gen(seed); //gerando aleatoriedade da seed
    uniform_int_distribution<int> dis(0, num_vertices - 1); 
    uniform_int_distribution<int> dis_peso(1, 10);  // pesos entre 1 e 10

    int caminhosCriados = 0;
    while(caminhosCriados < quantidade){
        int u = dis(gen); // sorteia o primeiro quarto
        int v = dis(gen); // sorteia o segundo quarto

        //evitando ciclos - menores se conectam somente a maiores, e o oposto é impossivel
        if(u == v) continue; //evita laço

        int peso = dis_peso(gen); //pesoa aleatorio

        adicionarCaminho(u, v, peso);
        caminhosCriados++;
    }
}

void Grafo::printAdjacencias(){
    cout << endl << "--- Mapa de Conexoes do Hospicio: ---" <<endl;

    for(int i = 0; i < num_vertices; i++){
        //numero do quarto atual
        cout << "Quarto [" << i << "]: ";

        //percorre lista de vizinhos do quarto i
        for(const auto& aresta : adj[i]){
            cout << "-> " << aresta.first << "(peso:" << aresta.second << ") ";
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
    : grafo(num_quartos), num_pacientes(0), farm(), table_hash(num_quartos * 2){ //inicializando o grafo e a farmacia
        static mt19937 gen(33); //gerando aleatoriedade da seed
        uniform_int_distribution<int> dis(0, num_quartos - 1); 
        uniform_int_distribution<int> dis_peso(1, 10);  // pesos entre 1 e 10
    for (int i = 0; i < num_quartos - 1; i++) {
        grafo.adicionarCaminho(i, i + 1, dis_peso(gen)); // peso aleatorio para os corredores
    }    
    this->num_pacientes = 0;
    Paciente pac(0, 0, 0, "nenhum", "nenhum");
    Quarto qrt(pac, false);
    quartos.assign(num_quartos, qrt);
    // cria um hospicio com quartos padrão contendo pacientes padrão

    //inicializando o log
    log = "";
    
}

Grafo::Grafo(int n) : num_vertices(n), adj(n) {}

Farmacia::Farmacia(){
    remedios = {"Fluoxetina",
    "Sertralina",
    "Escitalopram",
    "Venlafaxina",
    "Bupropiona",
    "Paroxetina",
    "Amitriptilina",
    "Clonazepam",
    "Diazepam",
    "Alprazolam",
    "Lorazepam",
    "Quetiapina",
    "Risperidona",
    "Olanzapina",
    "Aripiprazol",
    "Carbonato de lítio",
    "Valproato",
    "Lamotrigina",
    "Gardenal",
    "Rivotril"};
}

TabelaHash::TabelaHash(int num_loucos) : table(num_loucos){}

//Outras funcoes
void showMenu(Hospicio &hosp){
    int op;
    while(true){
        cout << "=== Hospicio Holstenwall ===" << endl;
        cout << "1. Cadastrar paciente (Hash)" << endl;
        cout << "2. Listar Pacientes (com Tabela Hash)" << endl;
        cout << "3. Excluir Paciente" << endl;
        cout << "4. Mostrar o Mapa do Hospicio (Lista ou Matriz adj)" <<endl;
        cout << "5. Medicar Paciente (percorrer grafos com PD)" <<endl;
        cout << "6. Exbir log do Hospicio" <<endl;
        cout << "7. O Problema do Louco que arrasou a farmacia" <<endl;
        cout << "8. Salvar log comprimido (Huffman)" << endl;
        cout << "9. Carregar log comprimido" << endl;
        cout << "10. Buscar no log (Rabin-Karp)" << endl;
        cout << "11. BFS - encontrar menor numero de corredores entre dois quartos"<<endl;
        cout << "12. Ronda completa do guarda (DFS) - explorar todos os quartos" << endl;
        cout << "13. Encontrar o melhor caminho entre dois quartos" <<endl;
        cout << "0. Sair" <<endl;
        cin >> op;
        
        if(op == 0){
            cout << "Encerrando..." << endl;
            break;
        }

        switch(op){
            case 1:{
                inserirPaciente(hosp);
                break;
            }case 2:{
                exibirPacientes(hosp);
                break;
            }case 3:{
                removerPaciente(hosp);
                break;
            } case 4:{
                cout << "1. Lista" <<endl << "2. Matriz" << endl;
                int option;
                cin >> option;
                if(option == 1){
                    hosp.grafo.printAdjacencias();
                }else if(option = 2) hosp.grafo.printMatrizAdjacencia();
                else cout << "opcao invalida" << endl;
                break;
            }case 5:{
                medicarPaciente(hosp);
                break;
            }case 6:{
                exibirLog(hosp);
                break;
            }case 7:{
                farmaciaArrasada(hosp);
                break;
            }
            case 8:{
                hosp.salvarLogComprimido();
                break;
            }
            case 9:{
                hosp.carregarLogComprimido();
                break;
            }
            case 10:{
                hosp.buscarNoLog();
                break;
            }
            case 11:{
                cout << "Digite o quarto de partida (entre 0 e " << hosp.quartos.size() - 1<< ")"<<endl;
                int qt;
                cin >> qt;
                cout << "Digite o quarto de chegada (entre 0 e " << hosp.quartos.size() - 1<< ")"<<endl;
                int qt2;
                cin >> qt2;
                if(qt == qt2){
                    cout << "Quartos iguais! entrada invalida!" <<endl;
                    break;
                }           
                hosp.grafo.bfs(qt, qt2);
                break;
            }
            case 12:{
                int quarto;
                cout << "Digite o quarto onde o vigia vai começar a ronda (entre 0 e "<< hosp.quartos.size() -1 <<")" <<endl;
                cin >> quarto;
                hosp.grafo.rondaVigia(quarto);
                break;
            }
            case 13:{
                cout << "Digite o quarto de partida (entre 0 e " << hosp.quartos.size() - 1<< ")"<<endl;
                int qt;
                cin >> qt;
                cout << "Digite o quarto de chegada (entre 0 e " << hosp.quartos.size() - 1<< ")"<<endl;
                int qt2;
                cin >> qt2;
                if(qt == qt2){
                    cout << "Quartos iguais! entrada invalida!" <<endl;
                    break;
                } 

                hosp.grafo.dijkstra(qt, qt2);
            }
            default:{
                cout << "Digite um numero valido! "<<endl;
                break;
            }
        }
    }
    return;
}

void inserirPaciente(Hospicio &hosp){
    if(hosp.num_pacientes == hosp.quartos.size()){
        cout << endl << "Hospicio cheio, nao ha vagas mais!" << endl;
        return;
    }

    Paciente pac;
    cout << endl << "Digite o numero do quarto em que deseja alocar o paciente (entre 0 e " << hosp.quartos.size() << ") " << endl;
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

    //registrando o paciente no log
    hosp.log += "[INSERCAO]: \nNome: " +pac.nome + "\nIdade: "+to_string(pac.idade) + 
    "\nDiagnostico: "+pac.diagnostico+"\n------------\n";

    hosp.table_hash.inserirTabela(name);
    return;
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
    string nome;
    for(int i =0; i < hosp.quartos.size(); i++){
         if(hosp.quartos[i].ocupado && hosp.quartos[i].paciente.id_paciente == id){
            nome = hosp.quartos[i].paciente.nome;
            hosp.quartos[i].ocupado = false;
            hosp.quartos[i].ocupado = false;
            Paciente pac(0, 0, 0, "nenhum", "nenhum");
            hosp.quartos[i].paciente = pac;
            hosp.num_pacientes --;
            encontrado = true;
        }
    }
    if(!encontrado) cout << "Paciente nao encontrado!" << endl <<endl;
    else{
        hosp.log += "[ALTA]: \nNome: " +nome +"\n------------\n";
        hosp.table_hash.removerTabela(nome);
        return;
    }
    return;
}

void exibirPacientes(Hospicio &hosp){
    if(hosp.num_pacientes == 0){
        cout << "\nNao ha pacientes internados! \n" <<endl;
        return;
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
    return;
}

void medicarPaciente(Hospicio &hosp){
    cout << "Escolhe um quarto entre 0 e " <<hosp.quartos.size() - 1<<" para medicar: ";
    int nqt;
    cin >> nqt;
    cout << endl;

    cout << "Digite o seu ponto de partida: ";
    int ptp;
    cin>>ptp;
    cout << endl;

    cout <<"Digite a sua stamina, quantos corredores / tuneis voce quer andar no maximo: ";
    int stm;
    cin>>stm;
    cout <<endl;

    //matriz de memorizacao
    vector<vector<int>> memo(hosp.quartos.size(), vector<int>(stm + 1, -1));
    int distancia = hosp.grafo.src_pd(ptp, nqt, stm, memo);

    if(distancia >= 1e7){
        cout << "Nao ha caminho possível com essa stamina!" <<endl;
    }else{
        cout << "Caminho Econtrado! " << endl;
        cout << "Custo total (paso percorrido): "<<  distancia << endl;
        cout << "Stamina restante: " << stm-distancia  << endl;
        cout << "ROTA: ";
        hosp.grafo.exibirCaminhoPD(ptp, nqt, stm, memo);
        cout << nqt;
        cout <<endl;
    }
    return;
}

void exibirLog(Hospicio &hosp){
    cout << hosp.log << endl;
    return;
}

void farmaciaArrasada(Hospicio &hosp){
    //aleatoriedade
    random_device rd;
    mt19937 g(rd());
    shuffle(hosp.farm.remedios.begin(), hosp.farm.remedios.end(), g); //embaralhando o vetor

    cout << endl << "Um louco fugiu do quarto e baguncou os medicamentos da farmacia! Escolha entre busca sequencial" <<
    "ou binaria para encontrar os remedios: " <<endl;

    cout << "1. Busca sequencial" << endl;
    cout << "2. Busca binaria" << endl;

    int op;
    cin >> op;

    if(op == 1){
        cout << "Digite o nome do remedio (indicado pelo manual de uso do software (documentacao)): " << endl;
        string remedio;
        cin.ignore();
        getline(cin, remedio);
        bool flag;
        for(int i = 0; i < hosp.farm.remedios.size(); i++){
            if(remedio == hosp.farm.remedios[i]){
                cout << "remedio encontrado na prateleira " << i << endl <<endl;
                flag = true;
                break;
            } 
        }
        if(!flag) cout << "Remedio nao encontrado! " << endl <<endl;
    }
    else if(op == 2){
        cout << "Digite o nome do remedio (indicado pelo manual de uso do software (documentacao)): " << endl;
        cin.ignore();
        string remedio;
        getline(cin, remedio);
        sort(hosp.farm.remedios.begin(), hosp.farm.remedios.end());
        int esquerda = 0;
        int direita = hosp.farm.remedios.size() - 1;
        bool flag = false;
        while(esquerda <= direita){
            int meio = esquerda + (direita - esquerda) / 2;

            if(hosp.farm.remedios[meio] == remedio){
                cout << "Remedio encontrado e pronto para ser usado num paciente!" << endl << endl;
                flag= true;
                break;
            }
            else if(hosp.farm.remedios[meio] < remedio) esquerda = meio + 1;
            else direita = meio -1;
        }
        if(!flag) cout << "Remedio nao encontrado! " << endl <<endl;
        return;
    }

    else{
        cout << "Opcao invalida! Retornando ao menu... " << endl;
    }

    return;
}

vector<int> buscarRabinKarp(const string& texto, const string& padrao){
    vector<int> ocorrencias;
    int n = texto.length();
    int m = padrao.length();

    //casos especiais
    if(m == 0 || m > n){
        return ocorrencias;
    }

    const int base = 256; //Número de caracteres possíveis
    const int modulo = 101; //primo para evvitar overflow

    //pre processamento
    int h = 1;
    for(int i = 0; i<m -1; i ++){
         h = (h * base) % modulo;
    }

    //calcula hash do padrao e da primeira janela
    int hashPadrao = 0;
    int hashJanela = 0;

    for(int i  =0; i< m; i++){
        hashPadrao = (base * hashPadrao + padrao[i]) % modulo;
        hashJanela = (base * hashJanela  + texto[i]) % modulo;
    }

    //percorre o texto deslizando a janela
    for (int i = 0; i <= n - m; i++) {
        // Se os hashes são iguais, verifica caractere por caractere
        if (hashPadrao == hashJanela) {
            bool encontrou = true;
            for (int j = 0; j < m; j++) {
                if (texto[i + j] != padrao[j]) {
                    encontrou = false;
                    break;
                }
            }
            if (encontrou) {
                ocorrencias.push_back(i); // Posição onde o padrão começa
            }
        }
        
        // Calcula hash da próxima janela (se não for a última)
        if (i < n - m) {
            hashJanela = (base * (hashJanela - texto[i] * h) + texto[i + m]) % modulo;
            
            // Ajusta hash negativo
            if (hashJanela < 0) {
                hashJanela += modulo;
            }
        }
    }
    return ocorrencias;
}
