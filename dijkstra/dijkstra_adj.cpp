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

void writeSolution(int dist[], int n, char nome_o[], int final)
{
    std::ofstream arquivo;
    arquivo.open(nome_o);
    
    if (final != -1)
    {
        for (int i = 0; i < n; i++)
            arquivo << i << ": " << dist[i] << endl;
    }
    else
        arquivo << dist[final] << endl;

    arquivo.close();
    cout << "Solução escrita no arquivo '" << nome_o << "'\n";
}

void printSolution(int dist[], int n, int final)
{
    if (final != -1) 
        cout << dist[final] << "\n";
    else {
        cout << "Vertex distance from Source\n";
        for (int i = 0; i < n; i++)
            cout << i << "\t\t" << dist[i] << "\n";
    }
}

void dijkstra(Graph graph, int src, int final, int dist[])
{
    int V = graph.V;

    // int dist[V]; //array solução

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
    int vertices, arestas, fonte = 0, final = -1;
    bool o = false, f = false, s = false;
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

    if (!f) {
        cout << "Nenhum arquivo de entrada foi informado.\nEncerrando programa...\n";
        return 0;
    }
    else {
        std::ifstream arquivo;
        arquivo.open(entrada);

        arquivo >> vertices >> entrada;

        Graph G(vertices);
        
        int cnt = 2, src, dest, peso;
        while (!arquivo.eof()) {
            arquivo >> src >> dest >> peso;
            G.addEdge(src, dest, peso);
            cnt += 3;
        }

        arquivo.close();
        int dist[vertices]; //resposta

        dijkstra(G, fonte, final, dist);
        
        if (o)
            writeSolution(dist, vertices, saida, final);
        if (s)
            printSolution(dist, vertices, final);

    }

    return 0;
}

//passar pelos vértices usando BFS 
//Min Heap para guardar os vértices ainda não procesados
//Min Heap -> priority queue

/*
1) let Min_Heap[V] = {numero, distancia}
2) new Min_Heap with src as root
3) while !min_heap.empty()
{
    a) u = o vértice de distância minima da minheap
    b) for v adjacentes de u {
        if (min_heap.contains(v) && distancia(v) > peso(u-v)+ distancia(u))
            update(v)
    }
}
*/