#include <bits/stdc++.h>  // Inclui todas as bibliotecas padrão do C++

using namespace std;
class Heap {
private:
    vector<int> heap;  // Armazena os elementos do heap
    unordered_map<int, int> indiceMapa;  // Mapeia chaves para seus índices no heap
    int qtd;  // Quantidade de elementos no heap
    // troca os elementos no heap e atualiza o indiceMapa, para praticidade
    void trocar(int i, int j) {
        swap(heap[i], heap[j]);
        indiceMapa[heap[i]] = i;
        indiceMapa[heap[j]] = j;
    }
    // Função para fazer subir elementos e balancear o heap
    void subir(int i) {
        while (i > 1 && heap[i] < heap[i / 2]) {
            // trocar o elemento com o pai
            trocar(i, i / 2);
            i = i / 2;
        }
    }
    // Função para fazer descer elementos e balancear o heap
    void descer(int i) {
        while (2 * i <= qtd) {
            int esq = 2 * i;
            int dir = 2 * i + 1;
            int menor = esq;
            if (dir <= qtd && heap[dir] < heap[esq]) {
                menor = dir;
            }
            if (heap[i] <= heap[menor]) {
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
    Heap() : qtd(0) {
        heap.resize(2000);  // Começa com capacidade 2000 (index 0 não é usado)
    }
    // Insere um novo elemento no heap
    void insert(int key) {
        if (qtd + 1 == heap.size()) {
            redimensionar();
        }
        qtd++;
        heap[qtd] = key;
        indiceMapa[key] = qtd;
        subir(qtd);
    }
    // Remove o elemento do topo (mínimo, pois é MinHeap)
    int extrairMin() {
        if (qtd == 0) throw runtime_error("Heap esta vazia");
        int minVal = heap[1];
        trocar(1, qtd);
        qtd--;
        descer(1);
        indiceMapa.erase(minVal);
        return minVal;
    }
    // Atualiza a chave de um elemento no heap
    void atualizarChave(int velhaKey, int novaKey) {
        if (indiceMapa.find(velhaKey) == indiceMapa.end()) {
            throw runtime_error("Chave nao encontrada");
        }
        int index = indiceMapa[velhaKey];
        heap[index] = novaKey;
        indiceMapa.erase(velhaKey);
        indiceMapa[novaKey] = index;
        // Ajusta o heap
        if (novaKey < velhaKey) {
            subir(index);
        } else {
            descer(index);
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
int main(){
    return 0 ;
}