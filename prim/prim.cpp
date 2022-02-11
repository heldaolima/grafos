#include <iostream>
#include <climits>
#include <cstring>
#include <fstream>

#include "grafo.h"
#include "min_heap.h"

using std::cout;
using std::cin;
using std::endl;

void writeSolution(int pai[], int n, int ini, char nome_o[])
{
    std::ofstream arquivo;
    arquivo.open(nome_o);
    
    for (int i = 0; i < n; i++) {
        if (i != ini)
            arquivo << "(" << pai[i] << "," << i << ") ";
    }
    arquivo << endl;
    
    arquivo.close();
    cout << "Solução inscrita no arquivo '" << nome_o << "'\n";
}


void printSolution(int pai[], int n, int ini)
{
    for (int i = 0; i < n; i++)
        if (i != ini)
            cout << "(" << pai[i] << "," << i << ") ";

    cout << "\n";
}

void PrimAGM(Graph graph, int src, int parent[])
{
    int V = graph.V;
    int key[V]; //chaves usadas para pegar o peso mínimo

    MinHeap minHeap(V);

    for (int v = 0; v < V; v++) {
        parent[v] = -1;
        key[v] = INT_MAX;
        minHeap.array[v] = newMinHeapNode(v, key[v]);
        minHeap.pos[v] = v;
    }
    key[src] = 0;
    minHeap.array[src] = newMinHeapNode(src, key[src]);
    minHeap.pos[src] = 0;

    minHeap.size = V;

    //todos os nós ainda não incluídos em parent estão na minHeap
    while (!minHeap.isEmpty()) {
        MinHeapNode* node = minHeap.extractMin();
        int u = node->v;

        list<AdjListNode>::iterator i;
        for (i = graph.adj[u].begin(); i != graph.adj[u].end(); i++) {
            int v = i->dest;

            //se v não está incluído na MST e o peso de u-v é menor que key[v]
            //atualize key[v] e o pai de v
            if (minHeap.isInMinHeap(v) && i->peso < key[v]) {
                key[v] = i->peso;
                parent[v] = u;
                minHeap.decreaseKey(v, key[v]);
            }
        }
    }
}

void help_prim()
{
    cout << "~~ HELP - ALGORITMO DE PRIM ~~\n\n";
    cout << "* O algoritmo mostra o custo da ACM do grafo, a partir de um vértice fonte\n";
    cout << "* -h\tmostra esse help\n";
    cout << "* -o <arquivo>\tredireciona a saída para 'arquivo'\n";
    cout << "* -f <arquivo>\tindica o 'arquivo' que contém o grafo de entrada\n";
    cout << "* -s\tmostra a solução (em ordem crescente)\n";
    cout << "* -i\tvértice inicial (se não for informado, o padrão é 0)\n";
}

int main(int argc, char const *argv[])
{
    int vertices, arestas, fonte = 0;
    bool o = false, f = false, s = false;
    char entrada[100], saida[100];

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-h") == 0) {
                help_prim();
                return 0;
            } 
            else if (strcmp(argv[i], "-i") == 0) {
                fonte = std::stoi(argv[i+1]);
                i++;
            }
            else if (strcmp(argv[i], "-o") == 0){
                o = true;
                strcpy(saida, argv[i+1]);
                i++;
            }
            else if (strcmp(argv[i], "-f") == 0) {
                f = true;
                strcpy(entrada, argv[i+1]);
                i++;
            }
            else if (strcmp(argv[i], "-s") == 0)
                s = true;
        }
    }
    
    if (!f) {
        cout << "Nenhum arquivo de entrada foi informado.\nEncerrando programa...\n";
        return 0;
    }
    else {
        std::ifstream arquivo;
        arquivo.open(entrada);

        if (arquivo.fail()) {
            cout << "O arquivo '" << entrada << "' não foi encontrado.\nEncerrando programa...\n";
            return 0;
        }

        arquivo >> vertices >> arestas;

        Graph G(vertices, arestas);
        int src, dest, peso;
        while (!arquivo.eof()) {
            arquivo >> src >> dest >> peso;
            G.addEdge(src, dest, peso);
        }

        arquivo.close();
        
        if (fonte < 0 || fonte >= vertices) {
            cout << "Vértice fonte fora do intervalo aceitável\n";
            return 0;
        }
        int pai[vertices]; //resposta

        PrimAGM(G, fonte, pai);
        
        if (o)
            writeSolution(pai, vertices, fonte, saida);
        
        printSolution(pai, vertices, fonte);   
    }
    return 0;
}