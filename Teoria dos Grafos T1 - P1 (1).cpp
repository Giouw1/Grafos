#include <bits/stdc++.h>
#include <chrono>
#include <windows.h>
#include <psapi.h>

using namespace std;
class Graph {
private:
    list<vector<int>> componenteconexa;
    int numVertices;
    int numArestas;
    bool usaMatriz;
    vector<vector<int>> listaAdj;
    vector<vector<bool>> matrizAdj; 
    vector<int> graus; // Lista com os graus de cada vértice


    //função para gerar a matriz de adjacência
    vector<vector<bool>> geraMatriz (string grafopath){
        ifstream arquivo(grafopath); //abre o arquivo
        string line; //recebe a primeira linha, para saber o número de vértices
        getline(arquivo, line);
        numVertices = stoi(line); 
        graus.resize(numVertices, 0);
        vector<vector<bool>> matrix(numVertices, vector<bool>(numVertices,0));
        while(getline(arquivo,line)){
            for(int i = 0; i<line.length();i++){
                if (line[i] == ' '){
                this->numArestas ++;
                graus[stoi(line.substr(0,i))-1]+= 1;
                graus[stoi(line.substr(i+1))-1]+= 1;
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
            graus.resize(numVertices, 0);
            vector<vector<int>> lista(numVertices);
            while(getline(arquivo,line)){
                for(int i = 0; i<line.length();i++){
                    if (line[i] == ' '){
                    this->numArestas ++;
                    graus[stoi(line.substr(0,i))-1]+= 1;
                    graus[stoi(line.substr(i+1))-1]+= 1;
                    lista[stoi(line.substr(0,i))-1].push_back(stoi(line.substr(i+1))-1);
                    lista[stoi(line.substr(i+1))-1].push_back(stoi(line.substr(0,i))-1);
                        };
                    };
                }; 
            arquivo.close();
            return lista;
        };
    // Função recursiva para DFS
    void DFSLista(int v, std::vector<int>& nivel, std::vector<int>& pai) {
        vector<int> pilha;
        int da_vez;
        pilha.push_back(v);
        while (pilha.size() != 0){
        da_vez = pilha.back();
        pilha.pop_back();
        for (int vizinho : listaAdj[da_vez]) {
            if (nivel[vizinho] == -1) {  // Ainda não visitado
                pai[vizinho] = da_vez;
                nivel[vizinho] = nivel[da_vez]+1;
                pilha.push_back(vizinho);
            }
            }
        }
    }

    // DFS Recursivo para a matriz de adjacência
    void DFSMatriz(int v, std::vector<int>& nivel, std::vector<int>& pai) {
        vector<int> pilha;
        int da_vez;
        pilha.push_back(v);
        while (pilha.size() != 0){
            da_vez = pilha.back();
            pilha.pop_back();
            for (int i = 0; i < numVertices; i++) {
                if (matrizAdj[da_vez][i] == 1 && nivel[i] == -1) {  // Vizinho não visitado
                    pai[i] = da_vez;
                    nivel[i] = nivel[da_vez] + 1;
                    pilha.push_back(i);
                }
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
        SIZE_T memoryUsage = getMemoryUsage();
        std::cout << "Memória sendo utilizada: " << memoryUsage<< " Bytes" << std::endl;
    }

    int getNumVertices() const {
        return numVertices;
    }

    int getNumArestas() const {
        return numArestas;
    }

    int grauMinimo() {
        int grau_min = graus[0];
        for(int i = 0; i < numVertices; i++){
            if(graus[i] < grau_min) grau_min = graus[i];
        }
        return grau_min;
        }

    int grauMaximo() {
        int grau_max = graus[0];
        for(int i = 0; i < numVertices; i++){
            if(graus[i] > grau_max) grau_max = graus[i];
        }
        return grau_max;
    }

    double grauMedio() {
        double somaGraus = 0;
        for(int i = 0; i < numVertices; i++) somaGraus += graus[i];
        return somaGraus/numVertices;
    }

    double medianaGrau() {
        vector<int> copia = graus;
        //colocar a lista em ordem através de uma ordenação
        sort(copia.begin(), copia.end());
        if (numVertices % 2 == 0) {
            return (copia[numVertices / 2 - 1] + copia[numVertices / 2]) / 2.0;
        } else {
            return copia[numVertices / 2];
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
    SIZE_T getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize; // Retorna o uso de memória em bytes
}

    vector<vector<int>> DFS(int verticeInicial) {
        std::vector<int> nivel(numVertices, -1);   //Nível de cada vértice
        std::vector<int> pai(numVertices, -1);    // Pai de cada vértice

        nivel[verticeInicial] = 0;

        if (usaMatriz) {
            DFSMatriz(verticeInicial, nivel, pai);
        }else {
            DFSLista(verticeInicial, nivel, pai);
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


    cout << "Usar matriz de adjacencia: ";
    cin >> usaMatriz;

    Graph g("grafo_6.txt", usaMatriz);

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
 //   int diametroo = g.diametro();
   
    cout << g.diametro_aprox() << endl;
    //Implementar algoritmo aproximativo.
    //Informação abaixo da outfile

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
    //Informação acima da outfile

    return 0;
}    
