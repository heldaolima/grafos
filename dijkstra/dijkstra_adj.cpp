/*
Implementação do algoritmo de Dijkstra para encontrar o caminho menos custoso de um 
vértice fonte aos demais

Essa implementação a representação de grafos com lista de listas de adjacências
e a estrutra de MinHeap para maior eficiçência
*/

#include <iostream>
#include <list>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <limits.h>

#include "grafo.h" //traz a classe de grafo com pesos nas arestas
#include "min_heap.h" //traz a estrutura minHeap

using std::list; 
using std::cout;
using std::cin;
using std::endl;

void writeSolution(int dist[], int n, char nome_o[], bool l, int final)
{
    std::ofstream arquivo;
    arquivo.open(nome_o);
    
    if (l)
    {
        for (int i = 0; i < n; i++)
            arquivo << i << ": " << dist[i] << " " << endl;
    }
    else
        arquivo << dist[final] << endl;

    arquivo.close();
    cout << "Solução inscrita no arquivo '" << nome_o << "'\n";
}

void printSolution(int dist[], int n, bool l, int final)
{
    if (l) 
        cout << dist[final] << "\n";
    else {
        for (int i = 0; i < n; i++)
            cout << i << ":" << dist[i] << " ";
        cout << "\n";
    }
}

void dijkstra(Graph graph, int src, int dist[])
{
    int V = graph.V;

    MinHeap minHeap(V);

    //fonteizar minHeap com os vértices
    //e a dist dos vértices no inicio
    for (int v = 0; v < V; v++) {
       dist[v] = INT_MAX;
       minHeap.array[v] = newMinHeapNode(v, dist[v]);
       minHeap.pos[v] = v;
    }

    //primeiro vértice com 0
    minHeap.array[src] = newMinHeapNode(src, dist[src]);
    
    minHeap.pos[src] = src;
    dist[src] = 0;
    minHeap.decreaseKey(src, dist[src]);
    
    minHeap.size = V;
    
    while(!minHeap.isEmpty())
    {
        //extrair o vértice com menor distância
        MinHeapNode *heapnode = minHeap.extractMin();
        
        int u = heapnode->v;
        // cout << u << " " << heapnode->peso << "\n";

        //passar pelos adjacentes de u e atualizar suas distâncias

        list<AdjListNode>::iterator i;
        for (i = graph.adj[u].begin(); i != graph.adj[u].end(); i++) {
            int v = i->dest;
            
            //se a menor distância a v não foi finalizada, e a distância
            //de u até v é menor do que antes
            // printf("%d + %d < %d\n", i->peso, dist[u], dist[v]);
            if (minHeap.isInMinHeap(v) && dist[u] != INT_MAX && i->peso + dist[u] < dist[v])
            {
                //   cout << "entrei\n";
                dist[v] = dist[u] + i->peso;
                
                //atualizar a distância na minHeap
                minHeap.decreaseKey(v, dist[v]);
                // cout << "dist[v] = " << dist[v] << "\n";
            }
        }
    }
}

void help_dijkstra()
{
    cout << "~~ HELP - ALGORITMO DE DIJKSTRA ~~\n\n";
    cout << "* O algoritmo mostra a distância mínima entre um vértice fonte e um vértice saída, em grafos com pesos nas arestas\n";
    cout << "* Se um vértice fonte não for informado pelo comando '-i', o padrão é 0\n";
    cout << "* O grafo de entrada tem 9 vértices, enumerado de 0 a 8. Nos comandos '-i' e '-l', não insira vértices fora desse intervalo\n";
    cout << "* O comando '-s' mostra a distância do vértice fonte a todos os outros\n";
}


int main(int argc, char const *argv[])
{
    int vertices, arestas, final = 0, fonte = 0;
    bool o = false, f = false, s = false, l = false;
    char entrada[100], saida[100];

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-h") == 0) {
                help_dijkstra();
                return 0;
            } 
            else if (strcmp(argv[i], "-i") == 0) {
                fonte = std::stoi(argv[i+1]);
                i++;
            }
            else if (strcmp(argv[i], "-l") == 0) {
                l = true;
                final = std::stoi(argv[i+1]);
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

    if (o) cout << saida << "\n";

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

        arquivo >> vertices >> entrada;

        Graph G(vertices);
        
        int cnt = 2, src, dest, peso;
        while (!arquivo.eof()) {
            arquivo >> src >> dest >> peso;
            G.addEdge(src, dest, peso);
            cnt += 3;
        }

        arquivo.close();
        
        if (fonte < 0 || fonte >= G.V) {
            cout << "Vértice fonte fora do intervalo aceitável\n";
            return 0;
        }
        else if (l && (final < 0 || final >= G.V)) {
            cout << "Vértice final fora do intervalo aceitável\n";
            return 0;
        }

        int dist[vertices]; //resposta

        dijkstra(G, fonte, dist);
        
        if (o)
            writeSolution(dist, vertices, saida, l, final);
        
        printSolution(dist, vertices, l, final);
    }

    return 0;
}

//O(E*logV)