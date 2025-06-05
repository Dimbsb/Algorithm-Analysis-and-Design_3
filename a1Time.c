// Prim-2010-2037
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define SIZE INT_MAX // Or dynamically allocate

// Structs
struct node
{
    int vertex;
    int weight;
    struct node *next;
};

struct node *createNode(int v, int weight);

struct Graph
{
    int NumberOfVertices;
    struct node **AdjacencyList;
    int *visited;
};

struct Graph *createGraph(int vertices);

struct queue
{
    int items[SIZE];
    int front;
    int rear;
};

void addEdge(struct Graph *graph, int src, int dest, int weight);
struct queue *createQueue();
void enqueue(struct queue *q, int value);
int dequeue(struct queue *q, int key[]);
void generateRandomGraph(struct Graph *graph, int NumberOfVertices, int NumberOfEdges, unsigned int seed, int maxWeight); 
void printGraph(struct Graph *graph);
void freeGraph(struct Graph *graph);
void freeQueue(struct queue *q);
// void printQueue(struct queue *q);

// Prim algorithm
void Prim(struct Graph *graph)
{
    int *parent = malloc(graph->NumberOfVertices * sizeof(int));
    int *key = malloc(graph->NumberOfVertices * sizeof(int));
    int startVertex = 0;

    for (int i = 0; i < graph->NumberOfVertices; i++)
    {
        key[i] = INT_MAX;
        parent[i] = -1;
    }

    key[startVertex] = 0;

    struct queue *q = createQueue();

    enqueue(q, startVertex);
    while (q->front != -1)
    {
        int u = dequeue(q, key);
        graph->visited[u] = 1;

        struct node *temp = graph->AdjacencyList[u];
        while (temp)
        {
            int v = temp->vertex;

            if (!graph->visited[v] && temp->weight < key[v])
            {
                key[v] = temp->weight;
                parent[v] = u;
                enqueue(q, v);
            }
            temp = temp->next;
        }
    }
    printf("EDGE BETWEEN NODES: \tWEIGHT\n");
    int totalWeight = 0;
    for (int i = 1; i < graph->NumberOfVertices; i++)
    {
        if (parent[i] != -1)
        {
            printf("%d - %d\t\t\t%d\n", parent[i], i, key[i]);
            totalWeight += key[i];
        }
    }
    printf("TOTAL WEIGHT IS: %d\n", totalWeight);
    freeQueue(q);
    free(parent);
    free(key);
}

// Main function
int main()
{
    clock_t start, end;
    double cpu_time_used,TotalTime;
    int tests = 10;

    unsigned int seed = 20;
    int maxWeight = 20;

    // GRAPH 1
    int NumberOfVertices1 = 5;
    int NumberOfEdges1 = 10;
    TotalTime = 0;
    for(int i=0; i<tests; i++){
    srand(seed);
    struct Graph *graph1 = createGraph(NumberOfVertices1);
    generateRandomGraph(graph1, NumberOfVertices1, NumberOfEdges1, seed, maxWeight);
    printf("\nGRAPH 1: %d vertices, %d edges\n", NumberOfVertices1, NumberOfEdges1);
    //printGraph(graph1);
    start = clock();
    Prim(graph1);
    end = clock();
    TotalTime += ((double)(end - start)) / CLOCKS_PER_SEC;
    freeGraph(graph1);
    }
    printf("Average execution time for GRAPH 1 over %d tests: %.6lf seconds\n", tests, TotalTime / tests);


    // GRAPH 2
    int NumberOfVertices2 = 10;
    int NumberOfEdges2 = 15;
    TotalTime = 0;
    for(int i=0; i<tests; i++){
    srand(seed);
    struct Graph *graph2 = createGraph(NumberOfVertices2);
    generateRandomGraph(graph2, NumberOfVertices2, NumberOfEdges2, seed, maxWeight);
    printf("\nGRAPH 2: %d vertices, %d edges\n", NumberOfVertices2, NumberOfEdges2);
    //printGraph(graph2);
    start = clock();
    Prim(graph2);
    end = clock();
    TotalTime += ((double)(end - start)) / CLOCKS_PER_SEC;
    freeGraph(graph2);
    }
    printf("Average execution time for GRAPH 2 over %d tests: %.6lf seconds\n", tests, TotalTime / tests);

    // GRAPH 3
    int NumberOfVertices3 = 15;
    int NumberOfEdges3 = 20;
    TotalTime = 0;
    for(int i=0; i<tests; i++){
    srand(seed);
    struct Graph *graph3 = createGraph(NumberOfVertices3);
    generateRandomGraph(graph3, NumberOfVertices3, NumberOfEdges3, seed, maxWeight);
    printf("\nGRAPH 3: %d vertices, %d edges\n", NumberOfVertices3, NumberOfEdges3);
    //printGraph(graph3);
    start = clock();
    Prim(graph3);
    end = clock();
    TotalTime += ((double)(end - start)) / CLOCKS_PER_SEC;
    freeGraph(graph3);
    }
    printf("Average execution time for GRAPH 3 over %d tests: %.6lf seconds\n", tests, TotalTime / tests);

    // GRAPH 4
    int NumberOfVertices4 = 20;
    int NumberOfEdges4 = 25;
    TotalTime = 0;
    for(int i=0; i<tests; i++){
    srand(seed);
    struct Graph *graph4 = createGraph(NumberOfVertices4);
    generateRandomGraph(graph4, NumberOfVertices4, NumberOfEdges4, seed, maxWeight);
    printf("\nGRAPH 4: %d vertices, %d edges\n", NumberOfVertices4, NumberOfEdges4);
    //printGraph(graph4);
    start = clock();
    Prim(graph4);
    end = clock();
    TotalTime += ((double)(end - start)) / CLOCKS_PER_SEC;
    freeGraph(graph4);
    }
    printf("Average execution time for GRAPH 4 over %d tests: %.6lf seconds\n", tests, TotalTime / tests);

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
}

// Create queue
struct queue *createQueue()
{
    struct queue *q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

// Enqueue
void enqueue(struct queue *q, int value)
{
    if (q->rear == SIZE - 1)
        printf("\nQueue is Full!!");
    else
    {
        if (q->front == -1)
        {
            q->front = 0;
        }
        q->rear++;
        q->items[q->rear] = value;
    }
}

// Dequeue
int dequeue(struct queue *q, int key[])
{
    if (q->front == -1)
    {
        printf("Queue is empty\n");
        return -1;
    }

    int minimumIndex = q->front;
    for (int i = q->front + 1; i <= q->rear; i++)
    {
        if (key[q->items[i]] < key[q->items[minimumIndex]])
        {
            minimumIndex = i;
        }
    }

    int minimumVertex = q->items[minimumIndex];

    for (int i = minimumIndex; i < q->rear; i++)
    {
        q->items[i] = q->items[i + 1];
    }
    q->rear--;
    if (q->rear < q->front)
    {
        q->front = q->rear = -1;
    }

    return minimumVertex;
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


// PrintGraph
void printGraph(struct Graph *graph)
{
    printf("Graph adjacency list:\n");
    for (int i = 0; i < graph->NumberOfVertices; i++)
    {
        printf("%d:", i);
        struct node *temp = graph->AdjacencyList[i];
        while (temp)
        {
            printf(" -> (%d, w=%d)", temp->vertex, temp->weight);
            temp = temp->next;
        }
        printf("\n");
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
    free(graph);
}

// FreeQueue
void freeQueue(struct queue *q)
{
    free(q);
}


/*
void printQueue(struct queue *q)
{
    printf("Queue: ");
    if (q->front == -1)
    {
        printf("empty");
    }
    else
    {
        for (int i = q->front; i <= q->rear; i++)
            printf("%d ", q->items[i]);
    }
    printf("\n");
}*/