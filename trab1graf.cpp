#include <bits/stdc++.h>  // Inclui todas as bibliotecas padrão do C++
#include <chrono>
#include <windows.h>
#include <psapi.h>      

using namespace std;

class Grafo {
private:
    list<vector<int>> componenteconexa;  // Lista de componentes conexas
    int numVertices;     // Número de vértices
    int numArestas;      // Número de arestas
    bool usaMatriz;      // Define se usa matriz de adjacência ou lista de adjacência
    vector<vector<int>> listaAdj;   // Lista de adjacência
    vector<vector<bool>> matrizAdj; // Matriz de adjacência
    vector<vector<vector<float>>> listaAdjP;
    vector<vector<float>> matrizAdjP;
    vector<int> graus;   // Vetor com os graus de cada vértice, onde o índice i terá o grau do vértice i+1

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
                    // Incrementa o grau dos vértices conectados
                    graus[stoi(line.substr(0, i)) - 1]++;
                    graus[stoi(line.substr(i + 1)) - 1]++;
                    // Marca a conexão na matriz de adjacência
                    matrix[stoi(line.substr(0, i)) - 1][stoi(line.substr(i + 1)) - 1] = 1;
                    matrix[stoi(line.substr(i + 1)) - 1][stoi(line.substr(0, i)) - 1] = 1;
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
                    graus[stoi(line.substr(0, i)) - 1]++; //Incrementa o grau dos vértices lidos
                    graus[stoi(line.substr(i + 1)) - 1]++;
                    // Adiciona as conexões nos vértices correspondentes na lista de adjacência
                    lista[stoi(line.substr(0, i)) - 1].push_back(stoi(line.substr(i + 1)) - 1);
                    lista[stoi(line.substr(i + 1)) - 1].push_back(stoi(line.substr(0, i)) - 1);
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
        graus.resize(numVertices, 0);  // Inicializa o vetor de graus com todos os elementos iguais a 0
        vector<vector<float>> matrix(numVertices, vector<float>(numVertices, 0));  // Inicializa a matriz de adjacência
        // Processa cada linha do arquivo para preencher a matriz
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {  // Identifica a separação entre vértices conectados
                for (int j = i+1; j<line.length(); j++){
                    if (line[j] == ' '){
                        this->numArestas++;  // Incrementa o número de arestas
                        // Incrementa o grau dos vértices conectados
                        graus[stoi(line.substr(0, i)) - 1]++;
                        graus[stoi(line.substr(i + 1)) - 1]++;
                        // Marca a conexão na matriz de adjacência
                        matrix[stoi(line.substr(0, i)) - 1][stoi(line.substr(i + 1,j)) - 1] = stof(line.substr(j+1));
                        matrix[stoi(line.substr(i + 1,j)) - 1][stoi(line.substr(0, i)) - 1] = stof(line.substr(j+1));
                        }
                    }
                    break;
                }
            }
        }
        arquivo.close();  // Fecha o arquivo
        return matrix;    
        
    }
    vector<vector<vector<float>>> geraListaPeso(string grafopath){
        ifstream arquivo(grafopath); // Abre o arquivo do grafo
        string line;                 
        getline(arquivo, line);    
        numVertices = stoi(line);    // Converte a linha em número de vértices
        graus.resize(numVertices, 0);  // Inicializa o vetor de graus com todos os elementos iguais a 0
        vector<vector<vector<float>>> listaAdjP(numVertices);  // Inicializa a lista de adjacencia, cada elemento dado um vértice (elemento do vetor maior) é um outro vértice e seu peso
        // Processa cada linha do arquivo para preencher a matriz
        while (getline(arquivo, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ') {  // Identifica a separação entre vértices conectados
                for (int j = i+1; j<line.length(); j++){
                    if (line[j] == ' '){
                        this->numArestas++;  // Incrementa o número de arestas
                        // Incrementa o grau dos vértices conectados
                        graus[stoi(line.substr(0, i)) - 1]++;
                        graus[stoi(line.substr(i + 1)) - 1]++;
                        // Marca a conexão na matriz de adjacência
                        listaAdjP[stoi(line.substr(0, i)) - 1].push_back({stof(line.substr(i + 1,j)) - 1 , stof(line.substr(j+1))});
                        listaAdjP[stoi(line.substr(i + 1,j)) - 1].push_back({stof(line.substr(0, i)) - 1 , stof(line.substr(j+1))}); //Vou ter que lidar float to integer no caso dos vertices)
                        }
                    }
                    break;
                }
            }
        }
        arquivo.close();  // Fecha o arquivo
        return listaAdjP;    
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
    // Constr utor da classe Grafo
     Grafo(string grafopath, bool matriz = false, bool usaPeso = false) : usaMatriz(matriz) {
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

    double grauMedio() {
        double somaGraus = 0;
        //soma todos os graus e divide pelo número de graus
        for (int i = 0; i < numVertices; i++) somaGraus += graus[i];
        return somaGraus / numVertices;
    }
    //função para cálculo da mediana dos graus
    double medianaGrau() {
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
    vector<vector<float>> dijkstra_matriz(int verticeInicial){
        int v = verticeInicial; // v vai guardar o vértice que será analisado no dijkstra
        vector<int> pai(numVertices,-1);
        vector<int> descobertos(numVertices,0);
        vector<int> explorado(numVertices,0);
        vector<float> distancia(numVertices,100000000000000000000000000000);//distância de todos igual a "infinito"
        pai[v] = v;
        distancia[v] = 0;
        while (true){
            explorado[v] = 1
            for(int j = 0; j < numVertices; j++){
                if(matrizAdj[v][j] != 0 && explorado[j]!= 1){
                    if(descobertos[j] != 1) descobertos[j] = 1;
                    if(distancia[j] > (distancia[v] + matrizAdj[v][j])){distancia[j] = distancia[v] + matrizAdj[v][j];}
                }
            }
            int menor_vertice = -1; //vai guardar o vértice com menor distância e não explorado
            for(int i = 0; i < descobertos.size(); i++){
                if(descobertos[i] == 1 && explorado[i] != 1){
                    if(menor_vertice == -1) menor_vertice = i;
                    else{
                        if(distancia[menor_vertice] > distancia[i]){menor_vertice = i;}
                    }
                }
            }
            v = menor_vertice;
            if(menor_vertice == -1) break; //significa que não tem mais vértices descobertos não explorados
        }
        vector<vector<float>> resultado = {pai,distancia};
        return resultado;
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
            } else {  // Se está usando lista de adjacência
                for (int vizinho : listaAdj[v]) {
                    if (nivel[vizinho] == -1) {
                        fila.push(vizinho);
                        nivel[vizinho] = nivel[v] + 1;
                        pai[vizinho] = v;
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
};

int main() {
    int numVertices, u, v;
    bool usaMatriz;
    bool peso;
    string path;
    cout << "Usar matriz de adjacencia: (0/1)";
    cin >> usaMatriz;
    cout <<  "Com peso? (0/1)";
    cin >> peso;
    Grafo g("grafo_W_1.txt", usaMatriz, peso);
    //Script para o estudo de caso 1
/*    chrono::duration<double> tempod;
    chrono::duration<double> tempob;
    for (int i = 0; i< 100; i++){
        auto start = chrono::high_resolution_clock::now();
        g.BFS(i);
        auto end = chrono::high_resolution_clock::now();
        tempob += end - start;
        auto start1 = chrono::high_resolution_clock::now();
        g.DFS(i);
        auto end1 = chrono::high_resolution_clock::now();
        tempod += end1-start1;
    }
    cout << "Duração BFS: " << tempob.count() << endl;
    cout << "Duração DFS: " << tempod.count() << endl;
    vector<vector<int>> paisbfs = {{},{},{}};
    vector<vector<int>> paisdfs = {{},{},{}};
    for (int i = 0; i<3;i++){
     vector<vector<int>> aux = g.BFS(i);
     vector<vector<int>> aux2 = g.DFS(i);
        paisbfs[i].push_back(aux[1][9]);
        paisbfs[i].push_back(aux[1][19]);
        paisbfs[i].push_back(aux[1][29]);
        paisdfs[i].push_back(aux2[1][9]);
        paisdfs[i].push_back(aux2[1][19]);
        paisdfs[i].push_back(aux2[1][29]);
}

    vector<int> distancias;
    distancias.push_back(g.distancia(9,19));
    distancias.push_back(g.distancia(9,29));
    distancias.push_back(g.distancia(19,29));

    list<vector<int>>componentes = g.compconexa();
    int tamanho = componentes.size();
    vector<int> maiorcomp = componentes.front();
    vector<int> menorcomp = componentes.back();
    cout <<maiorcomp.size() << "MENOR " << menorcomp.size() << endl;
    //  int diametroo = g.diametro();
   
    cout << g.diametro_aprox() << endl;
    //Implementar algoritmo aproximativo.
    //Informação abaixo da outfile
    g.printtree(0);
    ofstream outfile("arquivogerado.txt");
    outfile << "Numero de vertices: " << g.getNumVertices() << endl;
    outfile << "Numero de arestas: " << g.getNumArestas() << endl;
    outfile << "Grau minimo: " << g.grauMinimo() << endl;
    outfile << "Grau maximo: " << g.grauMaximo() << endl;
    outfile << "Grau medio: " << g.grauMedio() << endl;
    outfile << "Mediana dos graus: " << g.medianaGrau() << endl;
    outfile << "Quantidade de componentes conexas  " << componentes.size() << endl;
    auto it = componentes.begin();
    for (int i = 0 ; i< componentes.size();i++){advance(it,i); outfile << "Componente "<< i <<" "<< (*it).size() << endl;}
    outfile.close();
    //Informação acima da outfile
*/
    return 0;
}    
