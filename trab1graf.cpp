#include <bits/stdc++.h>
#include <chrono>
using namespace std;
class Graph {
private:
    list<vector<int>> componenteconexa;
    int numVertices;
    int numArestas;
    bool usaMatriz;
    vector<vector<int>> listaAdj;
    vector<vector<int>
    > matrizAdj; 
    vector<int> graus; // Lista com os graus de cada vértice


    //função para gerar a matriz de adjacência
    vector<vector<int>> geraMatriz (string grafopath){
        ifstream arquivo(grafopath); //abre o arquivo
        string line; //recebe a primeira linha, para saber o número de vértices
        getline(arquivo, line);
        numVertices = stoi(line); 
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

        //Função que gera lista de adjacência
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

/*    void adicionaAresta(int u, int v) {
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
*/
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
     vector<vector<int>> BFS(int verticeInicial) {
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
        vector<vector<int>> arvore = {nivel,pai};
        return arvore;
    }

    vector<vector<int>> DFS(int verticeInicial) {
        std::vector<int> nivel(numVertices, -1);   //Nível de cada vértice
        std::vector<int> pai(numVertices, -1);    // Pai de cada vértice

        nivel[verticeInicial] = 0;

        if (usaMatriz) {
            DFSRecursivoMatriz(verticeInicial, nivel, pai, 0);
        }else {
            DFSRecursivo(verticeInicial, nivel, pai, 0);
        }
        vector<vector<int>> arvore = {nivel,pai};
        return arvore;
    }
    
    int distancia(int partida, int chegada){
        return BFS(partida)[0][chegada];
        }

    int diametro(){
        int maxDistancia = 0;
        for (int i = 0; i < numVertices; i++) {
            std::vector<int> nivel = BFS(i)[0];
            for (int dist : nivel) {
                if (dist > maxDistancia) {
                    maxDistancia = dist;
                }
            }
        }
        return maxDistancia;
    }
    int diametro_aprox(){
        int maxDistancia = 0;
        int vertice2 = 0;
        std::vector<int> nivel = BFS(0)[0];
        for (int i = 0; i<nivel.size();i++) {
            if (nivel[i] > maxDistancia) {
                maxDistancia = nivel[i];
                vertice2 = i;
                }
            }
        std::vector<int> nivel2 = BFS(vertice2)[0];
        for (int i = 0; i<nivel2.size();i++) {
            if (nivel[i] > maxDistancia) {
                maxDistancia = nivel2[i];
                vertice2 = i;
                }   
        }
    return maxDistancia;
    }

    // O(n^2), duvidoso. ver agora distância máxima
    list<vector<int>> compconexa(){
        list<vector<int>> componentes;
        vector vertices_compo(numVertices,1);
        vector<vector<int>> arvore;
        for (int i=0;i<numVertices;i++){
            if (vertices_compo[i] == 0){continue;}
            vector<int>comp = {};
            arvore = BFS(i);
            for (int j = 0;j<arvore[0].size();j++){
                if (arvore[0][j] !=-1){
                    vertices_compo[j] = 0;
                    comp.push_back(j);
                    };
                };  
            auto it = componentes.begin();
            while (it != componentes.end()) {
                if (it->size() > comp.size()) {
                    break; // Found the right position to insert
                }
                ++it;
            }
            componentes.insert(it, comp);


            };  
        this-> componenteconexa = componentes;
        return componentes;
    };
    
};

int main() {
    int numVertices, u, v;
    bool usaMatriz;
    string path;

    cout << "De o nome do arquivo: ";
    cin >> path;
    cout << "Usar matriz de adjacencia: ";
    cin >> usaMatriz;

    Graph g("grafo_4.txt", usaMatriz);
    ofstream outfile("arquivogerado.txt");
    outfile << "Numero de vertices: " << g.getNumVertices() << endl;
    outfile << "Numero de arestas: " << g.getNumArestas() << endl;
    outfile << "Grau minimo: " << g.grauMinimo() << endl;
    outfile << "Grau maximo: " << g.grauMaximo() << endl;
    outfile << "Grau medio: " << g.grauMedio() << endl;
    outfile << "Mediana dos graus: " << g.medianaGrau() << endl;
    list<vector<int>>componentes = g.compconexa();
    outfile << "Quantidade de componentes conexas " << componentes.size() << endl;
    auto it = componentes.begin();
    for (int i = 0 ; i< componentes.size();i++){advance(it,i); outfile << "Componente "<< i <<(*it).size() << endl;}
//Informação acima da outfile


//Script para o estudo de caso
    chrono::duration<double> tempod;
    chrono::duration<double> tempob;
    for (int i = 0; i< 100; i++){
        auto start = chrono::high_resolution_clock::now();
        g.BFS(i);
        auto end = std::chrono::high_resolution_clock::now();
        tempob += end - start;
        auto start1 = chrono::high_resolution_clock::now();
        g.DFS(i);
        auto end1 = std::chrono::high_resolution_clock::now();
        tempod += end1-start1;

    }
    cout << "Duração BFS: " << tempob.count()/100 << endl;
    cout << "Duração DFS: " << tempod.count()/100 << endl;
    vector<vector<int>> paisbfs = {{},{},{}};
    vector<vector<int>> paisdfs = {{},{},{}};
    for (int i = 0; i<3;i++){
     vector<vector<int>> aux = g.BFS(i);
        paisbfs[i].push_back(aux[1][9]);
        paisbfs[i].push_back(aux[1][19]);
        paisbfs[i].push_back(aux[1][29]);
        paisdfs[i].push_back(aux[1][9]);
        paisdfs[i].push_back(aux[1][19]);
        paisdfs[i].push_back(aux[1][29]);
}
    vector<int> distancias;
    distancias.push_back(g.distancia(9,19));
    distancias.push_back(g.distancia(9,29));
    distancias.push_back(g.distancia(19,29));
/*    for(int i = 0; i < numVertices; i++){
        for (int j = 0; j < numVertices; j++){
            if(i != j) cout << i + 1 << " ate " << j + 1 << " " << g.distancia(i,j) << '\n';
            else continue;
        }
    }*/
    int tamanho = componentes.size();
    vector<int> maiorcomp = componentes.front();
    vector<int> menorcomp = componentes.back();
    int diametroo = g.diametro();
    //Implementar algoritmo aproximativo.
    return 0;
}