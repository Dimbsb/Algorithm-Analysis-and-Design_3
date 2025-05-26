#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define SIZE 40
#define V 30

struct queue
{
    int items[SIZE];
    int front;
    int rear;
};

struct queue *createQueue();
void enqueue(struct queue *q, int value);
int dequeue(struct queue *q, int key[]);
//void printQueue(struct queue *q);

struct node
{
    int vertex;
    int weight;
    struct node *next;
};

struct node *createNode(int v, int weight);
struct Graph
{
    int numVertices;
    struct node **adjLists;
    int *visited;
};

struct Graph *createGraph(int vertices);
void addEdge(struct Graph *graph, int src, int dest, int weight);

// ΑΠΟ ΒΙΒΛΙΟ ΣΕΛ 630
void Prim(struct Graph *graph, int startVertex)
{
    int parent[V];
    int key[V];
    
    for (int i = 0; i < V; i++)
    {
        key[i] = INT_MAX;
        parent[i] = -1;
    }

    key[startVertex] = 0;
 
    struct queue *q = createQueue();

    enqueue(q, startVertex);
    //printQueue(q);
    while (q->front != -1)
    {
        int u = dequeue(q, key);
        //printQueue(q);
        graph->visited[u] = 1;

        struct node *temp = graph->adjLists[u];
        while (temp)
        {
            int v = temp->vertex;

            if (!graph->visited[v] && temp->weight < key[v])
            {
                key[v] = temp->weight;
                parent[v] = u;
                enqueue(q, v);
                //printQueue(q);
            }
            temp = temp->next;
        }
    }
    printf("Edge \tWeight\n");
    for (int i = 1; i < V; i++)
    {
        struct node *temp = graph->adjLists[i];
        while (temp)
        {
            if (temp->vertex == parent[i])
            {
                printf("%d - %d \t%d\n", parent[i], i, temp->weight);
                break;
            }
            temp = temp->next;
        }
    }
}

int main()
{

//https://www.geeksforgeeks.org/prims-mst-for-adjacency-list-representation-greedy-algo-6/ 
    struct Graph *graph = createGraph(V);

    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 3, 5, 14);
    addEdge(graph, 4, 5, 10);
    addEdge(graph, 5, 6, 2);
    addEdge(graph, 6, 7, 1);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 7, 8, 7);

    Prim(graph, 0);

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
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(struct node *));
    graph->visited = malloc(vertices * sizeof(int));
    for (i = 0; i < vertices; i++)
    {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// AddEdge
void addEdge(struct Graph *graph, int src, int dest, int weight)
{
    struct node *newNode = createNode(dest, weight);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    newNode = createNode(src, weight);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
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
            q->front = 0;
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
            minimumIndex = i;
    }

    int minimumVertex = q->items[minimumIndex];

    for (int i = minimumIndex; i < q->rear; i++)
        q->items[i] = q->items[i + 1];
    q->rear--;
    if (q->rear < q->front)
        q->front = q->rear = -1;

    return minimumVertex;
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