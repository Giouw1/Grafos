#include <bits/stdc++.h>
//inclui as bibliotecas padrões de C ++
using namespace std;
class Graph {
private:
    int numVertices;
    int numArestas;
    bool usaMatriz;
    vector<vector<int>> listaAdj;
    vector<vector<int>> matrizAdj; 
    vector<int> graus; // Lista com os graus de cada vértice
    //função para gerar a matriz de adjacência


    vector<vector<int>> geraMatriz (string grafopath){
        ifstream arquivo(grafopath); //abre o arquivo
        string line; //recebe a primeira linha, para saber o número de vértices
        getline(arquivo, line);
        numArestas = stoi(line); 
        vector<vector<int>> matrix(numVertices, vector<int>(numVertices,0));
        while(getline(arquivo,line)){
            for(int i = 0; i<line.length();i++){
                if (line[i] == ' '){
                this->numArestas ++;
                matrix[stoi(line.substr(0,i))-1][stoi(line.substr(i+1))-1] = 1;
                matrix[stoi(line.substr(i+1))-1][stoi(line.substr(0,i))-1] = 1;
                    };
                };
            };
            arquivo.close();
            return matrix;
        };


        vector<vector<int>> geraLista(string grafopath){
            ifstream arquivo(grafopath); //abre o arquivo
            string line; //recebe a primeira linha, para saber o número de vértices
            getline(arquivo, line);
            numVertices = stoi(line);
            vector<vector<int>> lista(numVertices);
            while(getline(arquivo,line)){
                for(int i = 0; i<line.length();i++){
                    if (line[i] == ' '){
                    this->numArestas ++;
                    lista[stoi(line.substr(0,i))-1].push_back(stoi(line.substr(i+1))-1);
                    lista[stoi(line.substr(i+1))-1].push_back(stoi(line.substr(0,i))-1);
                        };
                    };
                }; 
            arquivo.close();
            return lista;
        };

    //faz uma lista com o grau de cada vértice i no índice i

    void calculaGraus() {
        graus.resize(numVertices, 0);
        if (usaMatriz) {
            for (int i = 0; i < numVertices; i++) {
                graus[i] = 0;
                for (int j = 0; j < numVertices; j++) {
                    if (matrizAdj[i][j] == 1) {
                        graus[i]++;
                    }
                }
            }
        } else {
            for (int i = 0; i < numVertices; i++) {
                graus[i] = listaAdj[i].size();
            }
        }
    }
    // Função recursiva para DFS
    void DFSRecursivo(int v, std::vector<int>& nivel, std::vector<int>& pai, int profundidade) {
        for (int vizinho : listaAdj[v]) {
            if (nivel[vizinho] == -1) {  // Ainda não visitado
                pai[vizinho] = v;
                nivel[vizinho] = profundidade + 1;
                DFSRecursivo(vizinho, nivel, pai, profundidade + 1);
            }
        }
    }

    // DFS Recursivo para a matriz de adjacência
    void DFSRecursivoMatriz(int v, std::vector<int>& nivel, std::vector<int>& pai, int profundidade) {
        for (int i = 0; i < numVertices; ++i) {
            if (matrizAdj[v][i] == 1 && nivel[i] == -1) {  // Vizinho não visitado
                pai[i] = v;
                nivel[i] = profundidade + 1;
                DFSRecursivoMatriz(i, nivel, pai, profundidade + 1);
            }
        }
    }

public:
    Graph(string grafopath, bool matriz = false) : usaMatriz(matriz) {
        if (usaMatriz) {
            matrizAdj = geraMatriz(grafopath);

        } else {
            listaAdj = geraLista(grafopath);
        }
    }

    void adicionaAresta(int u, int v) {
        if (usaMatriz) {
            if (matrizAdj[u][v] == 0) {
                matrizAdj[u][v] = matrizAdj[v][u] = 1;
                numArestas++;
            }
        } else {
            listaAdj[u].push_back(v);
            listaAdj[v].push_back(u);
            numArestas++;
        }
    }

    int getNumVertices() const {
        return numVertices;
    }

    int getNumArestas() const {
        return numArestas;
    }

    int grauMinimo() {
        calculaGraus();
        int grau_min = graus[0];
        for(int i = 0; i < numVertices; i++){
            if(graus[i] < grau_min) grau_min = graus[i];
        }
        return grau_min;
        }

    int grauMaximo() {
        calculaGraus();
        int grau_max = graus[0];
        for(int i = 0; i < numVertices; i++){
            if(graus[i] > grau_max) grau_max = graus[i];
        }
        return grau_max;
    }

    double grauMedio() {
        calculaGraus();
        double somaGraus = 0;
        for(int i = 0; i < numVertices; i++) somaGraus += graus[i];
        return somaGraus/numVertices;
    }

    double medianaGrau() {
        calculaGraus();
        //colocar a lista em ordem através de uma ordenação
        sort(graus.begin(), graus.end());
        if (numVertices % 2 == 0) {
            return (graus[numVertices / 2 - 1] + graus[numVertices / 2]) / 2.0;
        } else {
            return graus[numVertices / 2];
        }
    }
     std::vector<int> BFS(int verticeInicial) {
        //faz o BFS e retorna uma lista em que o número da lista[i] é o nível do iésimo termo
        std::vector<int> nivel(numVertices, -1);  // Nível de cada vértice
        std::vector<int> pai(numVertices, -1);    // Pai de cada vértice
        std::queue<int> fila;

        nivel[verticeInicial] = 0;
        fila.push(verticeInicial);

        while (!fila.empty()) {
            int v = fila.front();
            fila.pop();
            if (usaMatriz){
                for (int i = 0; i < numVertices; ++i) {
                    if (matrizAdj[v][i] == 1 && nivel[i] == -1) {
                        fila.push(i);
                        nivel[i] = nivel[v] + 1;
                        pai[i] = v;}
                        }
            } else{
                for (int vizinho : listaAdj[v]) {
                    if (nivel[vizinho] == -1) {  // Ainda não foi visitado
                        fila.push(vizinho);
                        nivel[vizinho] = nivel[v] + 1;
                        pai[vizinho] = v;
                    }
                }
            }
    } 
        return nivel;
    }

    void DFS(int verticeInicial) {
        std::vector<int> nivel(numVertices, -1);   //Nível de cada vértice
        std::vector<int> pai(numVertices, -1);    // Pai de cada vértice

        nivel[verticeInicial] = 0;

        if (usaMatriz) {
            DFSRecursivoMatriz(verticeInicial, nivel, pai, 0);
        }else {
            DFSRecursivo(verticeInicial, nivel, pai, 0);
        }
    }
    
    int distancia(int partida, int chegada){
        return BFS(partida)[chegada];
        }

    int diametro(){
        int maxDistancia = 0;
        for (int i = 0; i < numVertices; i++) {
            std::vector<int> nivel = BFS(i);
            for (int dist : nivel) {
                if (dist > maxDistancia) {
                    maxDistancia = dist;
                }
            }
        }
        return maxDistancia;
    }
    
};

int main() {
    int numVertices, u, v;
    bool usaMatriz;

    cout << "Digite o numero de vertices: ";
    cin >> numVertices;

    cout << "Usar matriz de adjacencia: ";
    cin >> usaMatriz;

    Graph g(numVertices, usaMatriz);
    int n = numVertices;
    cout << "Digite as arestas e depois um caractere para encerrar\n";
    while (cin>>u>>v) {
        g.adicionaAresta(u-1, v-1);
    }

    cout << "Numero de vertices: " << g.getNumVertices() << '\n';
    cout << "Numero de arestas: " << g.getNumArestas() << '\n';
    cout << "Grau minimo: " << g.grauMinimo() << '\n';
    cout << "Grau maximo: " << g.grauMaximo() << '\n';
    cout << "Grau medio: " << g.grauMedio() << '\n';
    cout << "Mediana dos graus: " << g.medianaGrau() << '\n';
    for(int i = 0; i < numVertices; i++){
        for (int j = 0; j < numVertices; j++){
            if(i != j) cout << i + 1 << " ate " << j + 1 << " " << g.distancia(i,j) << '\n';
            else continue;
        }
    }
    cout << "Diametro " << g.diametro() << '\n';
    return 0;
}
