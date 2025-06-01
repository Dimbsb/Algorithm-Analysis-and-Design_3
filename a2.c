// Kruskal-2010-2037
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define V 100

struct node
{
    int vertex;
    int weight;
    struct node *next;
};

struct Edge
{
    int src;
    int dest;
    int weight;
    struct Edge *edge;
};

struct node *createNode(int v, int weight);

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
void freeGraph(struct Graph *graph);

void Kruskal(struct Graph *graph)
{

    int parent[V], rank[V];
    ConstructSet(parent, rank, graph->NumberOfVertices);

    for (int i = 0; i < graph->edgeCounter - 1; i++) {
        int MinimumIndex = i;
        for (int j = i + 1; j < graph->edgeCounter; j++) {
            if (graph->edges[j].weight < graph->edges[MinimumIndex].weight)
                MinimumIndex = j;
        }
        if (MinimumIndex != i) {
            struct Edge temp = graph->edges[i];
            graph->edges[i] = graph->edges[MinimumIndex];
            graph->edges[MinimumIndex] = temp;
        }
    }

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
}


int main()
{

    clock_t start, end;
    double cpu_time_used;

    // Example 1...8 Nodes
    struct Graph *graph1 = createGraph(V);
    addEdge(graph1, 0, 1, 4);
    addEdge(graph1, 0, 7, 8);
    addEdge(graph1, 1, 2, 8);
    addEdge(graph1, 1, 7, 11);
    addEdge(graph1, 2, 3, 7);
    addEdge(graph1, 2, 8, 2);
    addEdge(graph1, 2, 5, 4);
    addEdge(graph1, 3, 4, 9);
    addEdge(graph1, 3, 5, 14);
    addEdge(graph1, 4, 5, 10);
    addEdge(graph1, 5, 6, 2);
    addEdge(graph1, 6, 7, 1);
    addEdge(graph1, 6, 8, 6);
    addEdge(graph1, 7, 8, 7);
 

    start = clock();
    Kruskal(graph1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("EXECUTION TIME FOR GRAPH - 1 WITH KRUSKAL IS: %.6lf SECONDS\n\n", cpu_time_used);
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
void addEdge(struct Graph *graph, int src, int dest, int weight) {
    struct node *newNode = createNode(dest, weight);
    newNode->next = graph->AdjacencyList[src];
    graph->AdjacencyList[src] = newNode;

    newNode = createNode(src, weight);
    newNode->next = graph->AdjacencyList[dest];
    graph->AdjacencyList[dest] = newNode;

    // Edge list 
    if (src < dest) {
        graph->edges[graph->edgeCounter].src = src;
        graph->edges[graph->edgeCounter].dest = dest;
        graph->edges[graph->edgeCounter].weight = weight;
        graph->edgeCounter++;
    }
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

void freeGraph(struct Graph *graph) {
    for (int i = 0; i < graph->NumberOfVertices; i++) {
        struct node *temp = graph->AdjacencyList[i];
        while (temp) {
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