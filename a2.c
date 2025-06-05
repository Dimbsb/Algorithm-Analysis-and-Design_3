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

// Kruskal's algorithm
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


// Main function
int main()
{
    clock_t start, end;
    double cpu_time_used;
    int tests = 10;

    unsigned int seed = 20;
    int maxWeight = 20;

    // GRAPH 1
    int NumberOfVertices1 = 5;
    int NumberOfEdges1 = 10;
    srand(seed);
    struct Graph *graph1 = createGraph(NumberOfVertices1);
    generateRandomGraph(graph1, NumberOfVertices1, NumberOfEdges1, seed, maxWeight);
    printf("\nGRAPH 1: %d vertices, %d edges\n", NumberOfVertices1, NumberOfEdges1);
    start = clock();
    Kruskal(graph1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %.6lf seconds\n", cpu_time_used);
    freeGraph(graph1);

    // GRAPH 2
    int NumberOfVertices2 = 10;
    int NumberOfEdges2 = 15;
    srand(seed);
    struct Graph *graph2 = createGraph(NumberOfVertices2);
    generateRandomGraph(graph2, NumberOfVertices2, NumberOfEdges2, seed, maxWeight);
    printf("\nGRAPH 2: %d vertices, %d edges\n", NumberOfVertices2, NumberOfEdges2);
    start = clock();
    Kruskal(graph2);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %.6lf seconds\n", cpu_time_used);
    freeGraph(graph2);

    // GRAPH 3
    int NumberOfVertices3 = 15;
    int NumberOfEdges3 = 20;
    srand(seed);
    struct Graph *graph3 = createGraph(NumberOfVertices3);
    generateRandomGraph(graph3, NumberOfVertices3, NumberOfEdges3, seed, maxWeight);
    printf("\nGRAPH 3: %d vertices, %d edges\n", NumberOfVertices3, NumberOfEdges3);
    start = clock();
    Kruskal(graph3);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %.6lf seconds\n", cpu_time_used);
    freeGraph(graph3);

    // GRAPH 4
    int NumberOfVertices4 = 20;
    int NumberOfEdges4 = 25;
    srand(seed);
    struct Graph *graph4 = createGraph(NumberOfVertices4);
    generateRandomGraph(graph4, NumberOfVertices4, NumberOfEdges4, seed, maxWeight);
    printf("\nGRAPH 4: %d vertices, %d edges\n", NumberOfVertices4, NumberOfEdges4);
    start = clock();
    Kruskal(graph4);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %.6lf seconds\n", cpu_time_used);
    freeGraph(graph4);

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

// Compare edges for qsort
int compareEdges(const void *a, const void *b)
{
    const struct Edge *e1 = (const struct Edge *)a;
    const struct Edge *e2 = (const struct Edge *)b;
    return e1->weight - e2->weight;
}

// Generate random graph
void generateRandomGraph(struct Graph *graph, int NumberOfVertices, int NumberOfEdges, unsigned int seed, int maxWeight)
{
 
    for (int i = 1; i < NumberOfVertices; i++)
    {
        int j = rand() % i;  
        int weight = rand() % maxWeight + 1;
        addEdge(graph, i, j, weight);
    }

    int RemainingEdges = NumberOfEdges - (NumberOfVertices - 1);
    int AddEdgeToGraph = 0;

    while (AddEdgeToGraph < RemainingEdges)
    {
        int u = rand() % NumberOfVertices;
        int v = rand() % NumberOfVertices;

        if (u == v) {
            continue;
        }

        struct node *temp = graph->AdjacencyList[u];
        bool EdgeInGraph = false;
        while (temp)
        {
            if (temp->vertex == v)
            {
                EdgeInGraph = true;
                break;
            }
            temp = temp->next;
        }

        if (!EdgeInGraph)
        {
            int weight = rand() % maxWeight + 1;
            addEdge(graph, u, v, weight);
            AddEdgeToGraph++;
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