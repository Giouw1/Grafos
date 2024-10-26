#include <bits/stdc++.h>  // Inclui todas as bibliotecas padrão do C++

using namespace std;

class Heap {
private:
    // Mapeia chaves para seus índices no heap
    int qtd = 0;  // Quantidade de elementos no heap
    int numVert;
    vector<vector<int>> heap{numVert, vector<int>(2)};// Armazena os elementos do heap: heap [0] é a chave, heap [1] é o vértice/índice
    // troca os elementos no heap e atualiza o indiceMapa, para praticidade
    void trocar(int i, int j) {
        swap(heap[i], heap[j]);
        indiceMapa[heap[i][1]] = i;
        indiceMapa[heap[j][1]] = j;
    }

    // Função para fazer subir elementos e balancear o heap Não sei pq i-1, mas é assim que se faz para heaps que começam no índice 0 (que é o caso)
    void subir(int i) {
        while (i > 0 && heap[i][0] < heap[( i - 1 ) / 2][0]) {
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
            if (dir < qtd && heap[dir][0] < heap[esq][0]) {
                menor = dir;
            }
            if (heap[i][0] <= heap[menor][0]) {
                break;
            }
            trocar(i, menor);
            i = menor;
        }
    }
    // Redimensiona o array do heap dobrando sua capacidade (precisa ou inicializa direto com o número de vértices?)
    void redimensionar() {
        heap.resize(heap.size() * 2);
    }
public: 
    vector<int> indiceMapa{numVert};
    Heap(int n) : qtd(-1) {
        numVert = n;
        heap.resize(n);      
    }
    // Insere um novo elemento no heap
    void insert(int chave, int index) {
        qtd++;
        if (qtd > heap.size()-1) {
            redimensionar();
        }
        heap[qtd] = {chave,index};
        indiceMapa[index] = qtd;
        subir(qtd);
    }
    bool estaVazia(){
        if(qtd == -1)return true;
        return false;
    }
    // Remove o elemento do topo (mínimo, pois é MinHeap), e retorna o valor dele(índice)
    int extrairMin() {
        if (qtd == -1) throw runtime_error("Heap esta vazia");
        int minVal = heap[0][1];
        if(qtd != 0) trocar(0, qtd);
        qtd--;
        descer(0);
        return minVal;
    }
    // Atualiza a chave de um elemento no heap
    void atualizarChave(int indice, int novaChave) {
        int posicao = indiceMapa[indice];
        int velhaChave = heap[posicao][0];
        heap[posicao][0] = novaChave;
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
/*int main(){
    Heap heapo(6);
    for (int i = 0;i<6; i++){
        heapo.insert(i,i);
    }
    heapo.atualizarChave(3,4);
    heapo.extrairMin();
    heapo.insert(7,7);
    return 0;
}*/
