#include <bits/stdc++.h>  // Inclui todas as bibliotecas padrão do C++
#include <chrono>
#include <windows.h>
#include <psapi.h>     
#include <random> 
using namespace std;

#include <bits/stdc++.h>  // Inclui todas as bibliotecas padrão do C++

using namespace std;
//Aqui tem a definição da classe heap que foi implementada por nós mesmos
class heap {
private:
    // Mapeia chaves para seus índices no heap
    int qtd = 0;  // Quantidade de elementos no heap
    int numVert;
    vector<int> indiceMapa;
    vector<pair<float,int>> heapo{static_cast<size_t>(numVert)};// Armazena os elementos do heap: heap [0] é a chave, heap [1] é o vértice/índice
    // troca os elementos no heap e atualiza o indiceMapa, para praticidade
    void trocar(int i, int j) {
        swap(heapo[i], heapo[j]);
        indiceMapa[heapo[i].second] = i;
        indiceMapa[heapo[j].second] = j;
    }

    // Função para fazer subir elementos e balancear o heap Não sei pq i-1, mas é assim que se faz para heaps que começam no índice 0 (que é o caso)
    void subir(int i) {
        while (i > 0 && heapo[i].first < heapo[( i - 1 ) / 2].first) {
            trocar(i, ( i - 1) / 2);
            i = ( i - 1 ) / 2;
        }
    }
    // Função para fazer descer elementos e balancear o heap
    void descer(int i) {
        while (2 * i + 1 < qtd) {
            int esq = 2 * i+1;
            int dir = 2 * i + 2;
            int menor = esq;
            if (dir < qtd && heapo[dir].first < heapo[esq].first) {
                menor = dir;
            }
            if (heapo[i].first <= heapo[menor].first) {
                break;
            }
            trocar(i, menor);
            i = menor;
        }
    }
    // Redimensiona o array do heap dobrando sua capacidade (precisa ou inicializa direto com o número de vértices?)

public: 
    heap(int n) : qtd(0), numVert(n), indiceMapa(n) {
        heapo.resize(n);      
    }
    // Insere um novo elemento no heap
    void insert(float chave, int index) {
        if (qtd > heapo.size()-1) {
            throw runtime_error("Estourou o tamanho");
        }
        heapo[qtd] = {chave,index};
        indiceMapa[index] = qtd;
        subir(qtd);
        qtd++;
    }
    bool estaVazia(){
        if(qtd == 0)return true;
        return false;
    }
    // Remove o elemento do topo (mínimo, pois é MinHeap), e retorna o valor dele(índice)
    int extrairMin() {
        if (qtd == 0) throw runtime_error("Heap esta vazia");
        int minVal = heapo[0].second;
        trocar(0, qtd-1);
        qtd--;
        descer(0);
        return minVal;
    }
    // Atualiza a chave de um elemento no heap
    void atualizarChave(int indice, float novaChave) {
        int posicao = indiceMapa[indice];
        float velhaChave = heapo[posicao].first;
        heapo[posicao].first = novaChave;
        // Ajusta o heap
        if (novaChave < velhaChave) {
            subir(posicao);
        } else {
            descer(posicao);
        }
    }
    /* Imprime o heap (para checar resultados)
    void printHeap() {
        for (int i = 1; i <= qtd; i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }*/
};

















//Agora, lidando com o grafo diretamente
class Grafo {
private:
    list<vector<int>> componenteconexa;  // Lista de componentes conexas
    int numVertices;     // Número de vértices
    int numArestas;      // Número de arestas
    bool usaMatriz;
    bool pesosNeg;      // Define se usa matriz de adjacência ou lista de adjacência
    vector<vector<int>> listaAdj;   // Lista de adjacência
    vector<vector<bool>> matrizAdj; // Matriz de adjacência
    vector<vector<pair<int,float>>> listaAdjP;
    vector<vector<float>> matrizAdjP;
    vector<vector<pair<int,pair<int,int>>>> listafluxo;
    vector<vector<pair<int,int>>> graforesidual; //grafo residual criado: só tem capacidades: rever isso depois, o 1 elemento é a capacidade da aresta orignal, e o segundo é o ponteiro para o fluxo da aresta original: rever isso depois, ver na bfs, no caminho minimo, e na funcao, tentar terminar, se nao rolar, mudar a aabordgaem
    //Optei por criar o grafo residual usando ponteiros: para o pair int float, o int, se for 1, é uma aresta original, se for 2, é uma aresta reversa, ver isso nas funções. comentar corretamente para evitar erros
    vector<int> graus;   // Vetor com os graus de cada vértice, onde o índice i terá o grau do vértice i+1
    bool dag;
    bool fluxo;

    // Função para gerar a matriz de adjacência a partir de um arquivo
    vector<vector<bool>> geraMatriz(string grafopath) {
        ifstream arquivo(grafopath); // Abre o arquivo do grafo
        string line;                 
        getline(arquivo, line);    
        numVertices = stoi(line);    // Converte a linha em número de vértices
        graus.resize(numVertices, 0);  // Inicializa o vetor de graus com todos os elementos iguais a 0
        vector<vector<bool>> matrix(numVertices, vector<bool>(numVertices, 0));  // Inicializa a matriz de adjacência
        // Processa cada linha do arquivo para preencher a matriz
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {  // Identifica a separação entre vértices conectados
                    this->numArestas++;  // Incrementa o número de arestas
                    // Incrementa o grau dos vértices conectados, se não for dag (se for, teria que ter grau de entrada e saída, e isso não é requisitado)
                    if (not (dag)){
                        graus[stoi(line.substr(0, i)) - 1]++;
                        graus[stoi(line.substr(i + 1)) - 1]++;
                    }
                    // Marca a conexão na matriz de adjacência
                    matrix[stoi(line.substr(0, i)) - 1][stoi(line.substr(i + 1)) - 1] = 1;
                    if (not (dag)){
                        matrix[stoi(line.substr(i + 1)) - 1][stoi(line.substr(0, i)) - 1] = 1; //Só marca a volta se não for dag: ij != ji
                    }
                }
            }
        }
        arquivo.close();  // Fecha o arquivo
        return matrix;    
    }
//mudar o dfs e mexer no geralista, depois disso pensar no dijkstra
    // Função para gerar a lista de adjacência a partir de um arquivo
    vector<vector<int>> geraLista(string grafopath) {
        ifstream arquivo(grafopath);  // Abre o arquivo do grafo
        string line;
        getline(arquivo, line);
        numVertices = stoi(line);  // Converte a primeira linha no número de vértices
        graus.resize(numVertices, 0);  // Inicializa o vetor de graus
        vector<vector<int>> lista(numVertices);  // Inicializa a lista de adjacência
        // Processa cada linha do arquivo para preencher a lista de adjacência
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {
                    this->numArestas++;  // Incrementa o número de arestas
                    // Incrementa o grau dos vértices conectados, se não for dag (se for, teria que ter grau de entrada e saída, e isso não é requisitado)
                    if (not (dag)){
                        graus[stoi(line.substr(0, i)) - 1]++; 
                        graus[stoi(line.substr(i + 1)) - 1]++;
                        }
                    // Adiciona as conexões nos vértices correspondentes na lista de adjacência
                    lista[stoi(line.substr(0, i)) - 1].push_back(stoi(line.substr(i + 1)) - 1);
                    if (not (dag)){
                    lista[stoi(line.substr(i + 1)) - 1].push_back(stoi(line.substr(0, i)) - 1);//Só marca a volta se não for dag: ij != ji
                    }
                }
            }
        }
        arquivo.close();  // Fecha o arquivo
        return lista;
    }

    vector<vector<float>> geraMatrizPeso(string grafopath){
        ifstream arquivo(grafopath); // Abre o arquivo do grafo
        string line;                 
        getline(arquivo, line);    
        numVertices = stoi(line);    // Converte a linha em número de vértices
        pesosNeg = false;
        graus.resize(numVertices, 0);  // Inicializa o vetor de graus com todos os elementos iguais a 0
        vector<vector<float>> matrix(numVertices, vector<float>(numVertices, 0));  // Inicializa a matriz de adjacência
        // Processa cada linha do arquivo para preencher a matriz
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {  // Identifica a separação entre vértices conectados
                for (int j = i+1; j<line.length(); j++){
                    if (line[j] == ' '){
                        this->numArestas++;  // Incrementa o número de arestas
                        // Incrementa o grau dos vértices conectados, se não for dag (se for, teria que ter grau de entrada e saída, e isso não é requisitado)
                        if (not (dag)){
                            graus[stoi(line.substr(0, i)) - 1]++;
                            graus[stoi(line.substr(i + 1)) - 1]++;
                        }
                        if ((stod(line.substr(j+1))) < 0.0){pesosNeg = true;}
                        // Marca a conexão na matriz  de adjacência
                        matrix[stoi(line.substr(0, i)) - 1][stoi(line.substr(i + 1,j)) - 1] = stod(line.substr(j+1));
                        if (not (dag)){
                            matrix[stoi(line.substr(i + 1,j)) - 1][stoi(line.substr(0, i)) - 1] = stod(line.substr(j+1));//Só marca a volta se não for DAG: ij != ji
                            }
                        }
                    }
                    break;
                }
            }
        }
        arquivo.close();  // Fecha o arquivo
        return matrix;    
    
    }
    vector<vector<pair<int,float>>> geraListaPeso(string grafopath){
        ifstream arquivo(grafopath); // Abre o arquivo do grafo
        string line;                 
        getline(arquivo, line);    
        numVertices = stoi(line);  
        pesosNeg = false;  // Converte a linha em número de vértices
        graus.resize(numVertices, 0);  // Inicializa o vetor de graus com todos os elementos iguais a 0
        vector<vector<pair<int,float>>> listaAdjP(numVertices);  // Inicializa a lista de adjacencia, cada elemento dado um vértice (elemento do vetor maior) é um outro vértice e seu peso
        // Processa cada linha do arquivo para preencher a matriz
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {  // Identifica a separação entre vértices conectados
                for (int j = i+1; j<line.length(); j++){
                    if (line[j] == ' '){
                        this->numArestas++;  // Incrementa o número de arestas
                        // Incrementa o grau dos vértices conectados, se não for dag (se for, teria que ter grau de entrada e saída, e isso não é requisitado)
                        if (not (dag)){
                            graus[stoi(line.substr(0, i)) - 1]++;
                            graus[stoi(line.substr(i + 1)) - 1]++;
                            }

                        if ((stof(line.substr(j+1))) < 0.0){pesosNeg = true;}

                        listaAdjP[stoi(line.substr(0, i)) - 1].push_back({stof(line.substr(i + 1,j)) - 1 , stod(line.substr(j+1))});
                        if (not (dag)){
                            listaAdjP[stoi(line.substr(i + 1,j)) - 1].push_back({stof(line.substr(0, i)) - 1 , stod(line.substr(j+1))}); //Só marca a volta se não for dag: ij != ji
                            }   
                        }
                    }
                    break;
                }
            }
        }
        arquivo.close();  // Fecha o arquivo
        return listaAdjP;    
    }

    pair<vector<vector<pair<int,pair<int,int>>>>,vector<vector<pair<int,int>>>> geraListaFluxo(string grafopath){
        ifstream arquivo(grafopath); // Abre o arquivo do grafo
        string line;                 
        getline(arquivo, line);    
        numVertices = stoi(line);  
        pesosNeg = false;  // Converte a linha em número de vértices
        vector<vector<pair<int,pair<int,int>>>> listafluxo(numVertices);  // Inicializa a lista de adjacencia, cada elemento dado um vértice (elemento do vetor maior) é um outro vértice e seu peso        vector<vector<pair<int,pair<int,int *>>>> graforesidual(numVertices);
        // Processa cada linha do arquivo para preencher a matriz
        vector<vector<pair<int,int>>> graforesidual(numVertices);
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {  // Identifica a separação entre vértices conectados
                for (int j = i+1; j<line.length(); j++){
                    if (line[j] == ' '){ //Testar a geração do grafo residual, se funcionar, continuar para o fordo fulkerso
                        this->numArestas++;  // Incrementa o número de arestas
                        if ((stof(line.substr(j+1))) < 0.0){pesosNeg = true;}

                        listafluxo[stoi(line.substr(0, i)) - 1].push_back({stoi(line.substr(i + 1,j)) - 1 , {stoi(line.substr(j+1)),0}});//ponteiro da errado pois o vetor que contem as arestas nao tem tamanho estatico
                        graforesidual[stoi(line.substr(0, i)) - 1].push_back({stoi(line.substr(i + 1,j)) - 1 , stoi(line.substr(j+1))}); //Cria a aresta original, capacidade inicial c(e)
                        //Cria a aresta original: c(e) - f(e), como ponteiros da aresta orignal
                        graforesidual[stoi(line.substr(i + 1,j)) - 1].push_back({stoi(line.substr(0, i)) - 1 , 0}); //Cria a aresta reversa, capacidade inicial 0 (f(e) inicial).
                        //Cria a aresta reversa f(e)
                        }
                    }
                    break;
                }
            }
        }
        arquivo.close();  // Fecha o arquivo
        return {listafluxo,graforesidual};    
    }
    
    
    // Ver se pode mexer na forma que coloca na stack
    // Função DFS iterativa usando lista de adjacência
    void DFSLista(int v, vector<int>& nivel, vector<int>& pai) {
        vector<int> pilha;  // Pilha para a DFS
        int da_vez;
        vector<int> visitado (numVertices,-1);
        pilha.push_back(v);
        while (!pilha.empty()) {
            da_vez = pilha.back();
            pilha.pop_back();
            if (visitado[da_vez]== 1){continue; // Evitar ficar "revisitando"  vértices que foram descobertos mais de uma vez
            }else{visitado[da_vez] = 1;}
            for (int vizinho : listaAdj[da_vez]) {  // Percorre os vizinhos
                if (visitado[vizinho] == -1) {  // Se o vizinho ainda não foi visitado
                    pai[vizinho] = da_vez;   // Define o pai
                    nivel[vizinho] = nivel[da_vez] + 1;  // Define o nível
                    pilha.push_back(vizinho);  // Adiciona o vizinho à pilha
                }
            }
        }
    }

    // Função DFS iterativa usando matriz de adjacência
    void DFSMatriz(int v, vector<int>& nivel, vector<int>& pai) {
        vector<int> visitado (numVertices,-1);
        vector<int> pilha;  // Pilha para a DFS
        int da_vez;
        pilha.push_back(v);
        while (!pilha.empty()) {
            da_vez = pilha.back();
            pilha.pop_back();
            if (visitado[da_vez]== 1){continue; // Evitar ficar "revisitando"  vértices que foram descobertos mais de uma vez
            }else{visitado[da_vez] = 1;}
            for (int i = 0; i < numVertices; i++) {
                if (matrizAdj[da_vez][i] && visitado[i] == -1) {  // Se o vizinho ainda não foi visitado
                    pai[i] = da_vez;
                    nivel[i] = nivel[da_vez] + 1;
                    pilha.push_back(i);  // Adiciona o vizinho à pilha
                }
            }
        }
    }

public:
    // Construtor da classe Grafo     
    Grafo(string grafopath, bool matriz = false, bool usaPeso = true,bool DAG = false, bool fluxo = false) : usaMatriz(matriz), dag(DAG), fluxo(fluxo){
        if (fluxo){
            matriz = false;
            tie(listafluxo,graforesidual) = geraListaFluxo(grafopath); 
        }else{
            if (usaPeso){
                if (usaMatriz) {
                    matrizAdjP = geraMatrizPeso(grafopath);  // Gera matriz de adjacência
                }else {
                    listaAdjP = geraListaPeso(grafopath);    // Gera lista de adjacência
                }
            }
            else{
                if (usaMatriz) {
                    matrizAdj = geraMatriz(grafopath);  // Gera matriz de adjacência
                }else {
                    listaAdj = geraLista(grafopath);    // Gera lista de adjacência
                }
            }
        }
        SIZE_T memoryUsage = getMemoryUsage();  // Obtém uso de memória
        cout << "Memória sendo utilizada: " << memoryUsage << " Bytes" << endl;
    }

    // Funções getter para número de vértices e arestas
    int getNumVertices() const { return numVertices; }
    int getNumArestas() const { return numArestas; }

    // Funções para calcular o grau mínimo, máximo e médio
    int grauMinimo() {
        int grau_min = graus[0];
        for (int i = 0; i < numVertices; i++) {
            //percorre a lista de graus e guarda o menor
            if (graus[i] < grau_min) grau_min = graus[i];
        }
        return grau_min;
    }

    int grauMaximo() {
        int grau_max = graus[0];
        for (int i = 0; i < numVertices; i++) {
            //percorre a lista de graus e guarda o maior
            if (graus[i] > grau_max) grau_max = graus[i];
        }
        return grau_max;
    }

    float grauMedio() {
        float somaGraus = 0;
        //soma todos os graus e divide pelo número de graus
        for (int i = 0; i < numVertices; i++) somaGraus += graus[i];
        return somaGraus / numVertices;
    }
    //função para cálculo da mediana dos graus
    float medianaGrau() {
        vector<int> copia = graus;
        sort(copia.begin(), copia.end());  // Ordena a lista de graus
        //retorna o valor do meio da lista, ou a soma dos valores do meio dividido por 2 se
        //o número de vértices for par
        if (numVertices % 2 == 0) {
            return (copia[numVertices / 2 - 1] + copia[numVertices / 2]) / 2.0;
        } else {
            return copia[numVertices / 2];
        }
    }

    // Função BFS que retorna o nível e o pai de cada vértice
    vector<vector<int>> BFS(int verticeInicial) {
        vector<int> nivel(numVertices, -1);  // Vetor com o nível de cada vértice
        vector<int> pai(numVertices, -1);    // Vetor com o pai de cada vértice
        queue<int> fila;
        nivel[verticeInicial] = 0;
        fila.push(verticeInicial);
        //usa fila para fazer a BFS, adicionando os vizinhos do primeiro vértice da lista
        // e removendo-o após todos os seus vizinhos terem sido visitados
        while (!fila.empty()) {
            int v = fila.front();
            fila.pop();
            //guarda o primeiro vértice da fila e o remove, para começar a percorrer os vizinhos
            if (usaMatriz) {  // Se está usando matriz de adjacência
                
                    for (int i = 0; i < numVertices; ++i) {
                    if (matrizAdj[v][i] == 1 && nivel[i] == -1) {
                        //adiciona na fila caso não tenha sido visitado
                        fila.push(i);
                        nivel[i] = nivel[v] + 1;
                        pai[i] = v;
                        }
                    }
                
            } else { // Se está usando lista de adjacência
                if (!fluxo){
                    for (int vizinho : listaAdj[v]) {
                        if (nivel[vizinho] == -1) {
                            fila.push(vizinho);
                            nivel[vizinho] = nivel[v] + 1;
                            pai[vizinho] = v;
                        }
                    }
                }
                else{ //se é um grafo de fluxo, vizinho.first é o vértice ligado ao vértice v. vizinho.second é o pair que contém capacidade e fluxo do grafo residual ( só calculo fluxo com ford fulkerson, então só o grafo residual vale mesmo)
                    for (pair<int,int> vizinho : graforesidual[v]) {
                        if ((nivel[vizinho.first] == -1) and (vizinho.second != 0)) { //se a capacidade for zero, é como se não tivesse aresta
                            fila.push(vizinho.first);
                            nivel[vizinho.first] = nivel[v] + 1;
                            pai[vizinho.first] = v;
                        }
                    }

                }
            }
        }
        vector<vector<int>> arvore = {nivel, pai};
        return arvore;
    }

    // Função para obter o uso de memória
    SIZE_T getMemoryUsage() {
        PROCESS_MEMORY_COUNTERS memInfo;
        GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
        return memInfo.WorkingSetSize;  // Retorna o uso de memória em bytes
    }

    // Função DFS que retorna o nível e o pai de cada vértice
    vector<vector<int>> DFS(int verticeInicial) {
        vector<int> nivel(numVertices, -1);
        vector<int> pai(numVertices, -1);
        nivel[verticeInicial] = 0;
        if (usaMatriz) {
            DFSMatriz(verticeInicial, nivel, pai);  // Usa DFS com matriz
        } else {
            DFSLista(verticeInicial, nivel, pai);   // Usa DFS com lista
        }
        vector<vector<int>> arvore = {nivel, pai};
        return arvore;
    }

    // Função para calcular a distância entre dois vértices usando BFS
    //para calcular a distância, basta fazer a BFS de um elemento
    //retornar o nível para o outro elemento, assim obtendo a distância mínima
    int distancia(int partida, int chegada) {
        return BFS(partida)[0][chegada];
    }
    // Função para calcular o diâmetro do grafo
    int diametro() {
        //Executa a BFS em cada vértice e guarda a distância máxima
        int maxDistancia = 0;
        for (int i = 0; i < numVertices; i++) {
            vector<int> nivel = BFS(i)[0];
            for (int dist : nivel) {
                if (dist > maxDistancia) {
                    maxDistancia = dist;
                }
            }
        }
        return maxDistancia;
    }
    // Função para calcular uma aproximação do diâmetro
    int diametro_aprox() {
        //Calcula a aproximação do diâmetro executando o BFS duas vezes
        int maxDistancia = 0;
        int vertice2 = 0;
        vector<int> nivel = BFS(0)[0];
        for (int i = 0; i < nivel.size(); i++) {
            if (nivel[i] > maxDistancia) {
                maxDistancia = nivel[i];
                vertice2 = i;
            }
        }
        vector<int> nivel2 = BFS(vertice2)[0];
        for (int i = 0; i < nivel2.size(); i++) {
            if (nivel[i] > maxDistancia) {
                maxDistancia = nivel2[i];
                vertice2 = i;
            }
        }
        return maxDistancia;
    }
    void printtree(int v){
        ofstream arvorefile("arvore.txt");
        arvorefile<< "   " << "Vértice" << "     |    " << "Nível"<< "    |   " << "Pai" << "\n";
        vector<vector<int>> de = DFS(v);
        vector<vector<int>> be = BFS(v);
        for (int i = 0;i <numVertices;i++){
            arvorefile << "   " << i << "     |    " << be[0][i] << "    |   " << be[1][i] << "\n";
            arvorefile << "   " << i << "     |    " << de[0][i] << "    |   " << de[1][i] << "\n";
        }
        arvorefile.close();


    }
    // Função para calcular os componentes conexos do grafo
    list<vector<int>> compconexa() {
        //Identifica e retorna todos os componentes conexos do grafo
        list<vector<int>> componentes;
        vector<int> vertices_compo(numVertices, 1);
        vector<vector<int>> arvore;
        for (int i = 0; i < numVertices; i++) {
            if (vertices_compo[i] == 0) continue;
            vector<int> comp;
            arvore = BFS(i);
            for (int j = 0; j < arvore[0].size(); j++) {
                if (arvore[0][j] != -1) {
                    vertices_compo[j] = 0;
                    comp.push_back(j);
                }
            }
            auto it = componentes.begin();
            while (it != componentes.end()) {
                if (it->size() > comp.size()) {
                    break;
                }
                ++it;
            }
            componentes.insert(it, comp);
        }
        this->componenteconexa = componentes;
        return componentes;
    }


    vector<vector<float>> dijkstra(int v){
        //Algo está dando errado, dar uma olhada por que o i não sai do 0
        //Amanha testar isso e lista
        if (pesosNeg == true){throw runtime_error("Não pode ter pesos negativos para dijkstra!");}
        vector<float> distancias(numVertices, -1);
        vector<float> pai(numVertices, -1);
        vector<float> explorado(numVertices, 0);
        float distmindavez = -1;
        int da_vez;
        distancias[v]= 0;
        //Dijkstra para matriz
        if (usaMatriz == 1){
            for (int i = 0; i < numVertices; i++){
                distmindavez = -1;
                for (int j = 0 ; j < numVertices; j++){
                    if((explorado[j] != 1) and (((distancias[j] < distmindavez) or (distmindavez == -1)) and (distancias[j] != -1))){distmindavez = distancias[j];da_vez = j;} //Se ainda não foi explorado, já foi descoberto, e for a menor distancia da vez, é o que vai ser explorado da vez
                }
                if (distmindavez == -1){break;} /* Se não achou uma nova distância mínima, já explorou todos que conseguiu, não é grafo conexo, e a componente conexa já foi percorrida*/
                explorado[da_vez] = 1; //Está sendo explorado

                for (int k = 0; k<numVertices; k++){
                    if ((matrizAdjP[da_vez][k] != 0) and ( ( (distancias[da_vez] + matrizAdjP[da_vez][k])  <   distancias[k]) or (distancias[k] == -1) ) )  { //Se houver aresta, verificar as distancias
                        distancias[k] = (distancias[da_vez] + matrizAdjP[da_vez][k]);
                        pai[k] = da_vez;
                    }
                }
                //Pegar o menor valor do vetor distancias que nao esteja explorado
            }
        }
        //Dijkstra para L. Adjacência
        else{
        for (int i = 0; i<numVertices;i++){
                distmindavez = -1;
                for (int j=0;j<numVertices;j++){
                    if((explorado[j] != 1) and ((distancias[j] < distmindavez) or (distmindavez == -1)) and (distancias[j] != -1)){distmindavez = distancias[j];da_vez = j;}
                }
                
                if (distmindavez == -1){break;}

                explorado[da_vez] = 1; //Está sendo explorado

                for (int k =0; k<listaAdjP[da_vez].size();k++){

                    int vizinho = listaAdjP[da_vez][k].first;

                    float pesovizinho = listaAdjP[da_vez][k].second;

                    if(( ( (distancias[da_vez] + pesovizinho)< distancias[vizinho]) or (distancias[vizinho] == -1) ) ) {
                        distancias[vizinho] = (distancias[da_vez] + pesovizinho);
                        pai[vizinho] = da_vez;
                        }
                    }
                }
        }
            
    return {distancias,pai};
};
    //Retorna uma lista com o caminho mínimo até cada vértice, e uma lista pai.
    //vector<vector<float>> dijkstraHeap(){}
    vector<vector<float>> dijkstraHeap(int verticeInicial) {
    vector<float> distancia(numVertices, numeric_limits<float>::infinity());
    vector<float> pai(numVertices, -1);
    vector<float> explorado(numVertices, 0);
    int v = verticeInicial;

    heap heape(numVertices);
    distancia[v] = 0;
    heape.insert(0, v);  // Inserir o vértice inicial no heap com distância 0
    
    if (usaMatriz == 1){
        while (!heape.estaVazia()) {
            v = heape.extrairMin();  // Extrai o vértice com a menor distância
            explorado[v] = 1;
            for (int k = 0; k < numVertices; k++) {
                if (matrizAdjP[v][k] != 0 && (explorado[k] != 1)) {
                    float novaDist = distancia[v] + matrizAdjP[v][k];
                    if (novaDist < distancia[k]) {

                        if (distancia[k] == numeric_limits<float>::infinity()) {
                            distancia[k] = novaDist;
                            pai[k] = v;
                            heape.insert(novaDist, k);  // Insere no heap se for descoberto
                        } else {
                            distancia[k] = novaDist;
                            pai[k] = v;
                            heape.atualizarChave(k, novaDist);  // Atualiza se já está no heap
                        }
                    }
                }
                continue;
            }
        }
    }
    else{
        int nodevizinho;
        float peso_vizinho;
        while (!heape.estaVazia()) {
            v = heape.extrairMin();  // Extrai o vértice com a menor distância
            explorado[v] = 1;
            for (pair<int,float> vizinho : listaAdjP[v]) {
                nodevizinho = vizinho.first;
                peso_vizinho = vizinho.second;
                if (explorado[nodevizinho] != 1) {
                    float novaDist = distancia[v] + peso_vizinho;
                    if (novaDist < distancia[nodevizinho]) {

                        if (distancia[nodevizinho] == numeric_limits<float>::infinity()) {
                            distancia[nodevizinho] = novaDist;
                            pai[nodevizinho] = v;
                            heape.insert(novaDist, nodevizinho);  // Insere no heap se for descoberto
                        } else {
                            distancia[nodevizinho] = novaDist;
                            pai[nodevizinho] = v;
                            heape.atualizarChave(nodevizinho, novaDist);  // Atualiza se já está no heap
                        }
                    }
                }
                continue;
            }
        }
    }
    return {distancia,pai};
    };
    
    pair<unordered_map<string,int>,unordered_map<int,string>> mapeador_nome_vertice(string caminho) {
        ifstream arquivo(caminho); // Abre o arquivo do grafo
        string line;
        unordered_map<string,int> nomes_map;
        unordered_map<int,string> vert_map;
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {

                if (line[i] == ','){
                    nomes_map[line.substr(i+1)] = stoi(line.substr(0,i));
                    vert_map[stoi(line.substr(0,i))] = line.substr(i+1);
                    break;
                }
                continue;
            }
        };
        arquivo.close();
        return {nomes_map,vert_map};
    };
    vector<int> caminhominimo(int v, vector<int> vetorpai){
         // adicionar um elemento ao final que é o gargalo, para evitar percorrer o vetor caminho em um momento posterior.
        vector<int> caminho ={};
        int da_vez = v;
        caminho.push_back(v);
        if (fluxo){ //a função caminho mínimo mudou para o caso de fluxo só para evitar recalcular o gargalo
            if (not(usaMatriz)){
                int capacidade_min = numeric_limits<int>::max();
                while(vetorpai[da_vez] != -1){
                    for (pair<int,int> vizinho : graforesidual[vetorpai[da_vez]]) {
                        if (vizinho.first == da_vez) {
                            capacidade_min = min(capacidade_min, vizinho.second);
                                 
                            }
                        }
                    caminho.push_back(vetorpai[da_vez]);
                    da_vez = vetorpai[da_vez];
                    }
                    caminho.push_back(capacidade_min);
                }
            }
        else{
            while(vetorpai[da_vez] != -1){
                caminho.push_back(vetorpai[da_vez]);
                da_vez = vetorpai[da_vez];
            }
        }
        return caminho;
    }
    //Na matriz de adjacencia, a checagem das capacidades e arestas é O(1), na lista, é O(grau(v)), por isso usei M de Adj, se nao usaria lista.
    //Talvez tenha que implementar a matriz de adjacancia: talvez seja melhor na matriz e na lista, 1 do par é capacidade 2 do par é fluxo
    pair<int,vector<vector<pair<int,pair<int,int>>>>> fordfulkerso(int fonte, int sumidouro,bool escreve){ //até agora mexi na bfs, no caminho mínimo e fiz a função gargalo nessa função vou ter que ralar: atualizar os valores do grafo residual
        vector<int> caminho;
        int real; //Se original for 1, o fluxo aumenta em gargo, se oirignal for -1, fluxo diminui em gargo: não preciso ver os dois, se é original ou se é reversa, basta ver 1 e deixar um marcador que vê se foi efetivo ou não
        int gargo;
        tie(gargo,caminho) =gargalo(fonte,sumidouro); // o vetor caminho é organizado do sumidouro até a fonte
        int fluxomaximo = gargo;
        while (gargo != 0){
            for (int i = (caminho.size()-1) ; i>=1 ; i--){ // para cada aresta do caminho, preciso saber se ela é original ou não não precisava fazer esses dois checks, mas achei válido
                real = 0;
                for (pair<int,pair<int,int>>& j : listafluxo[caminho[i]]){
                    if (j.first == caminho[i-1]){//se achar a aresta aqui, ela é não original no grafo residual, pois a aresta real é i-1 -> i.
                        j.second.second = j.second.second - gargo; //tirar o gargalo do fluxo dessa aresta
                        real = -1;
                    }
                }
                if (real == 0){
                    for (pair<int,pair<int,int>>& j : listafluxo[caminho[i-1]]){ 
                        if (j.first == caminho[i]){//se achar a aresta aqui, ela é uma aresta original no grafo residual, pois a aresta real é i-1 -> i.
                            j.second.second = j.second.second + gargo;
                            real = 1;
                            }
                        }
                }
                    //agora, atualizar a capacidade do graforesidual: mexer na aresta original e reversa
                    for (pair<int,int>& original : graforesidual[caminho[i-1]]){
                        if (original.first == caminho[i]){ //procurar a aresta original
                            original.second = original.second - (gargo*real); //Aresta original é c(e) - f(e): se f(e) aumenta em gargo, a capacidade da aresta original se reduz em gargo
                        }
                    }
                    for (pair<int,int>& reversa : graforesidual[caminho[i]]){
                        if (reversa.first == caminho[i-1]){ //procurar a aresta original
                            reversa.second = reversa.second + (gargo*real); //Aresta reversa é f(e): se f(e) aumenta em gargo, a capacidade da aresta reversa aumenta em gargo
                        }
                    }                   
                }
            tie(gargo,caminho) = gargalo(fonte,sumidouro);
            fluxomaximo += gargo;
        }
        fluxo_aloc(escreve,listafluxo);
        return {fluxomaximo,listafluxo};
        //até agora mudei bfs, caminho mínimo ecriei a gargalo

    //Ideia do ford fulkerso: para cada iteração, achar o caminho-gargalo. satura-lo, até que não haja mais caminhos gargalo?
};        

    pair<int,vector<int>> gargalo(int fonte, int sumidouro){
        vector<int> caminho_gargalo;
        int garga;
        caminho_gargalo = caminhominimo(sumidouro,  BFS(fonte)[1]);//bfs foi alterada de forma à conseguir lidar com grafos fluxo
        garga = caminho_gargalo[caminho_gargalo.size()-1];
        caminho_gargalo.pop_back(); //tiro o valor do gargalo, calculado na função caminho_mínimo
        if (caminho_gargalo.size() == 1){
            garga = 0;
        }
        return {garga,caminho_gargalo};
    }
    
    void fluxo_aloc(bool escreve, vector<vector<pair<int,pair<int,int>>>> grafofluxo){
        ofstream alocacao("alocfluxo.txt");
        alocacao << "Vértice saída" <<"  "<< "Vértice entrada" << "  " << "Fluxo"<< endl;
        for (int i = 0; i< numVertices; i++){
            for (int j = 0; j< grafofluxo[i].size();j++){
                alocacao << i+1 <<"  "<< grafofluxo[i][j].first+1 << "  "<<grafofluxo[i][j].second.second << endl; 
            }
        }
        alocacao.close();
    }
//rever depois do almoço: erro pois elementos de capacidade 0 estão sendo considerados
};
int main() {
    int numVertices, u, v;
    bool usaMatriz;
    bool peso;
    bool fluxo;
    vector<int> caminho;
    vector<vector<float>> aux;
    string path;
    int fluxomaximo;
    vector<vector<pair<int,pair<int,int>>>> graforesultado;

    //Obtenção dos dados, usando apenas lista de adjacencia, por questões de memória
    Grafo g("grafo_rf_1.txt", 0, 1, 1,1);
    tie(fluxomaximo,graforesultado) =g.fordfulkerso(0,1,1);
    cout << fluxomaximo << endl;
    // K = 100
    //Para gerar aleatoriedade com base em uma distribuição uniforme (probabilidade igual para todos)
    random_device dev; 
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0,numVertices);
    int verticei;
    //Para contar os tempos das execuções (Não conseguimos contar do grafo 3 para frente)
        chrono::duration<float> tempo;
        chrono::duration<float> tempoheap;
        for (int i = 0; i<100; i++){
            verticei = dist(rng);
            auto start = chrono::high_resolution_clock::now();
            auto end = chrono::high_resolution_clock::now();
            tempo += end - start;
        }
    //cout << tempo.count() << "\n";
    cout << tempoheap.count() << "\n";
    }
