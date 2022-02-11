#include <iostream>
#include <list>

using std::cout;
using std::list;

/*
Makefile da classe Graph

Trata-se de um grafo não direcionado com pesos nas arestas

*/

typedef struct AdjListNode {
    int dest; //vértice destino
    int peso;
    
    AdjListNode(int d, int p) {
        dest = d;
        peso = p;
    }

} AdjListNode;

class Graph {

public:
    int V; //n de vértices
    int E;
    list<AdjListNode> *adj;

    Graph(int v, int e);
    void addEdge(int src, int dest, int wgt);
    void PrintGraph();
};

Graph::Graph(int V, int E)
{
    this->V = V;
    this->E = E;
    adj = new list<AdjListNode>[V];
}

void Graph::addEdge(int src, int dest, int wgt)
{
    AdjListNode novo_src = AdjListNode(dest, wgt);
    AdjListNode novo_dest = AdjListNode(src, wgt);

    adj[src].push_back(novo_src);
    adj[dest].push_back(novo_dest);
}

void Graph::PrintGraph()
{
    list<AdjListNode>::iterator i;
    for (int j = 0; j < V; j++) {
        cout << "No vértice " << j << ": ";

        for (i = adj[j].begin(); i != adj[j].end(); i++)
            cout << i->dest << "(" << i->peso << ")\t";
        
        cout << "\n";
    }
}