// Kruskal-2010-2037
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct node
{
    int vertex;
    int weight;
    struct node *next;
};

struct node *createNode(int v, int weight);

struct Edge
{
    int src;
    int dest;
    int weight;
    struct Edge *edge;
};

struct Graph
{
    int NumberOfVertices;
    struct node **AdjacencyList;
    int *visited;
    struct Edge *edges;
    int edgeCounter;
};

struct Graph *createGraph(int vertices);

void addEdge(struct Graph *graph, int src, int dest, int weight);
void ConstructSet(int parent[], int rank[], int n);
int FindSet(int parent[], int i);
void Connection(int parent[], int rank[], int x, int y);
int compareEdges(const void *a, const void *b);
void generateRandomGraph(struct Graph *graph, int NumberOfVertices, int NumberOfEdges, unsigned int seed, int maxWeight);
void freeGraph(struct Graph *graph);

void Kruskal(struct Graph *graph)
{
    int *parent = malloc(graph->NumberOfVertices * sizeof(int));
    int *rank = malloc(graph->NumberOfVertices * sizeof(int));
    ConstructSet(parent, rank, graph->NumberOfVertices);

    qsort(graph->edges, graph->edgeCounter, sizeof(struct Edge), compareEdges);

    printf("Edge \tWeight\n");
    int NumberOfEdges = 0;
    int totalWeight = 0;
    for (int i = 0; NumberOfEdges < graph->NumberOfVertices - 1 && i < graph->edgeCounter; i++)
    {
        int u = FindSet(parent, graph->edges[i].src);
        int v = FindSet(parent, graph->edges[i].dest);
        if (u != v)
        {
            printf("%d - %d \t%d\n", graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
            totalWeight += graph->edges[i].weight;
            Connection(parent, rank, u, v);
            NumberOfEdges++;
        }
    }
    printf("Total weight of MST: %d\n", totalWeight);

    free(parent);
    free(rank);
}

int main()
{
    clock_t start, end;
    double cpu_time_used;

    int NumberOfVertices = 10000;          
    int NumberOfEdges = 20000;            
    unsigned int seed = 20;  
    int maxWeight = 20;
    srand(seed);

    struct Graph *graph1 = createGraph(NumberOfVertices);
    generateRandomGraph(graph1, NumberOfVertices, NumberOfEdges, seed, maxWeight);

    start = clock();
    Kruskal(graph1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("EXECUTION TIME FOR KRUSKAL'S ALGORITHM IS: %.6lf SECONDS\n", cpu_time_used);
    freeGraph(graph1);
    return 0;
}

// Create node
struct node *createNode(int v, int weight)
{
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// CreateGraph
struct Graph *createGraph(int vertices)
{
    int i;
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->NumberOfVertices = vertices;
    graph->AdjacencyList = malloc(vertices * sizeof(struct node *));
    graph->visited = malloc(vertices * sizeof(int));
    graph->edges = malloc(vertices * vertices * sizeof(struct Edge));
    graph->edgeCounter = 0;
    for (i = 0; i < vertices; i++)
    {
        graph->AdjacencyList[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// AddEdge
void addEdge(struct Graph *graph, int src, int dest, int weight)
{
    struct node *newNode = createNode(dest, weight);
    newNode->next = graph->AdjacencyList[src];
    graph->AdjacencyList[src] = newNode;

    newNode = createNode(src, weight);
    newNode->next = graph->AdjacencyList[dest];
    graph->AdjacencyList[dest] = newNode;

    // Edge list
    // Always store with src < dest to avoid duplication
    if (src > dest)
    {
        int temp = src;
        src = dest;
        dest = temp;
    }

    graph->edges[graph->edgeCounter].src = src;
    graph->edges[graph->edgeCounter].dest = dest;
    graph->edges[graph->edgeCounter].weight = weight;
    graph->edgeCounter++;
}

// ConstructSet
void ConstructSet(int parent[], int rank[], int n)
{
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
        rank[i] = 0;
    }
}

// FindSet
int FindSet(int parent[], int i)
{
    if (parent[i] != i)
    {
        parent[i] = FindSet(parent, parent[i]);
    }
    return parent[i];
}

// Connection
void Connection(int parent[], int rank[], int x, int y)
{
    int xRank = FindSet(parent, x);
    int yRank = FindSet(parent, y);

    if (xRank == yRank)
        return;

    if (rank[xRank] > rank[yRank])
    {
        parent[yRank] = xRank;
    }
    else
    {
        parent[xRank] = yRank;
        if (rank[xRank] == rank[yRank])
        {
            rank[yRank] = rank[yRank] + 1;
        }
    }
}

int compareEdges(const void *a, const void *b)
{
    const struct Edge *e1 = (const struct Edge *)a;
    const struct Edge *e2 = (const struct Edge *)b;
    return e1->weight - e2->weight;
}

// GenerateRandomGraph
void generateRandomGraph(struct Graph *graph, int NumberOfVertices, int NumberOfEdges, unsigned int seed, int maxWeight)
{
    srand(seed);

    // Step 1: Ensure connectivity with a spanning tree
    for (int i = 1; i < NumberOfVertices; i++)
    {
        int j = rand() % i; // connect to an existing node
        int weight = rand() % maxWeight + 1;
        addEdge(graph, i, j, weight);
    }

    // Step 2: Add remaining edges randomly (use a simple 1D edge lookup)
    int extraEdges = NumberOfEdges - (NumberOfVertices - 1);
    int added = 0;

    while (added < extraEdges)
    {
        int u = rand() % NumberOfVertices;
        int v = rand() % NumberOfVertices;

        if (u == v) continue; // no self-loops

        // Check if edge already exists
        struct node *temp = graph->AdjacencyList[u];
        bool exists = false;
        while (temp)
        {
            if (temp->vertex == v)
            {
                exists = true;
                break;
            }
            temp = temp->next;
        }

        if (!exists)
        {
            int weight = rand() % maxWeight + 1;
            addEdge(graph, u, v, weight);
            added++;
        }
    }
}



// FreeGraph
void freeGraph(struct Graph *graph)
{
    for (int i = 0; i < graph->NumberOfVertices; i++)
    {
        struct node *temp = graph->AdjacencyList[i];
        while (temp)
        {
            struct node *next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->AdjacencyList);
    free(graph->visited);
    free(graph->edges);
    free(graph);
}
