#include <iostream>
#include <climits>

using std::cout;

typedef struct MinHeapNode {
    int v;
    int peso;
} MinHeapNode;

MinHeapNode* newMinHeapNode(int v, int peso)
{
    MinHeapNode* node = (MinHeapNode*) malloc(sizeof(MinHeapNode));
    node->peso = peso;
    node->v = v;
    return node;
}

class MinHeap
{
public:
    MinHeapNode **array;
    int *pos;
    int capacity;
    int size; //tamanho atual
    MinHeap(int capacity);

    void MinHeapify(int ); //
    void printHeap();
    int parent(int i) { return (i-1)/2; }
    int left(int i) { return (2*i + 1); }
    int right(int i) { return (2*i + 2); }
    MinHeapNode* extractMin();
    void decreaseKey(int, int);
    void deleteKey(int i);
    bool isInMinHeap(int);
    bool isEmpty() {return size == 0;}
};

MinHeap::MinHeap(int cap)
{
    size = 0;
    capacity = cap;
    array = (MinHeapNode**) malloc(sizeof(MinHeapNode)*cap);
    pos = new int[cap];
}

void swap(MinHeapNode **x, MinHeapNode **y)
{
    MinHeapNode* temp = *x;
    *x = *y;
    *y = temp;
}

void MinHeap::printHeap()
{
    int i;
    for (i = 0; i < size; i++)
        cout << array[i]->v << "(" << array[i]->peso << ") ";
    cout << "\n";
}

bool MinHeap::isInMinHeap(int v)
{
    if (pos[v] < size) 
        return true; 
    return false;
}

void MinHeap::decreaseKey(int v, int dist)
{
    int i = pos[v];

    array[i]->peso = dist;

    while (i && array[i]->peso < array[parent(i)]->peso)
    {
        pos[array[i]->v] = parent(i);
        pos[array[parent(i)]->v] = i;

        swap(&array[i], &array[parent(i)]);
        i = parent(i);
    }
}

//heapify a subtree with the root at given index
void MinHeap::MinHeapify(int i)
{
    int l = left(i), r = right(i), smallest = i;

    if (l < size && array[l]->peso < array[smallest]->peso)
        smallest = l;
    if (r < size && array[r]->peso < array[smallest]->peso)
        smallest = r;

    if (smallest != i)
    {
        MinHeapNode* smallestNode = array[smallest];
        MinHeapNode* iNode = array[i];

        pos[smallestNode->v] = i;
        pos[iNode->v] = smallest;

        swap(&array[smallest], &array[i]);

        MinHeapify(smallest);
    }
}

MinHeapNode* MinHeap::extractMin()
{
    if (size <= 0)
        return NULL;
    
    MinHeapNode* root = array[0];
    
    //
    MinHeapNode* last = array[size-1];
    array[0] = last;
    
    pos[root->v] = size-1;
    pos[last->v] = 0;

    size--;

    MinHeapify(0);

    return root;
}

//deleta o elemento no índice i
void MinHeap::deleteKey(int i)
{
    decreaseKey(i, INT_MIN);
    extractMin();
}
