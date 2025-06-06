#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_EDGES 10000 

//Structs
struct node {
    int vertex;
    int weight;
    struct node *next;
};

struct Edge {
    int u, v, weight;
};

struct Graph {
    int NumberOfVertices;
    struct node **AdjacencyList;
    int *visited;
    struct Edge *edges;
    int edgeCount;
};

//Function declarations 
struct node *createNode(int v, int weight);
void removeAdjEdge(struct Graph *graph, int src, int dest);
struct Graph *createGraph(int vertices);
void addEdge(struct Graph *graph, int src, int dest, int weight);
void dfs(struct Graph *graph, int v);
bool isConnected(struct Graph *graph);
int compareEdges(const void *a, const void *b);
void freeGraph(struct Graph *graph);
void generateRandomGraph(struct Graph *graph, int NumberOfVertices, int NumberOfEdges, unsigned int seed, int maxWeight);

// Reverse‐delete algorithm  
void reverseDeleteMST(struct Graph *graph) {
    int idx = 0;
     
    for (int u = 0; u < graph->NumberOfVertices; u++) {
        for (struct node *p = graph->AdjacencyList[u]; p; p = p->next) {
            int v = p->vertex;
            if (u < v) {
                graph->edges[idx].u = u;
                graph->edges[idx].v = v;
                graph->edges[idx].weight = p->weight;
                idx++;
                if (idx >= MAX_EDGES) break;
            }
        }
        if (idx >= MAX_EDGES) break;
    }
    graph->edgeCount = idx;

    qsort(graph->edges, graph->edgeCount, sizeof(struct Edge), compareEdges);

    int totalWeight = 0;
    printf("Edge   Weight\n");
    for (int i = 0; i < graph->edgeCount; i++) {
        int u = graph->edges[i].u;
        int v = graph->edges[i].v;
        int w = graph->edges[i].weight;

        
        removeAdjEdge(graph, u, v);
        removeAdjEdge(graph, v, u);

        if (!isConnected(graph)) {
             
            struct node *n1 = createNode(v, w);
            n1->next = graph->AdjacencyList[u];
            graph->AdjacencyList[u] = n1;
            struct node *n2 = createNode(u, w);
            n2->next = graph->AdjacencyList[v];
            graph->AdjacencyList[v] = n2;

            totalWeight += w;
            printf("%2d - %2d    %d\n", u, v, w);
        }
         
    }
    printf("Total weight of MST = %d\n", totalWeight);
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
    reverseDeleteMST(graph1);
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
    reverseDeleteMST(graph2);
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
    reverseDeleteMST(graph3);
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
    reverseDeleteMST(graph4);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %.6lf seconds\n", cpu_time_used);
    freeGraph(graph4);

    return 0;
}


// Create a new adjacency‐list node 
struct node *createNode(int v, int weight) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

//Delete the edge from src to dest in the adjacency list if it exists.
void removeAdjEdge(struct Graph *graph, int src, int dest) {
    struct node *p = graph->AdjacencyList[src];
    struct node *prev = NULL;

    while (p != NULL) {
        if (p->vertex == dest) {
            if (prev == NULL) {
                graph->AdjacencyList[src] = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
}

// Create an empty graph with the given vertices 
struct Graph *createGraph(int vertices) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->NumberOfVertices = vertices;
    graph->AdjacencyList = malloc(vertices * sizeof(struct node *));
    graph->visited = malloc(vertices * sizeof(int));
    graph->edges = malloc(MAX_EDGES * sizeof(struct Edge));
    graph->edgeCount = 0;
    for (int i = 0; i < vertices; i++) {
        graph->AdjacencyList[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Add undirected edge by inserting nodes in both lists 
void addEdge(struct Graph *graph, int src, int dest, int weight) {
    struct node *newNode = createNode(dest, weight);
    newNode->next = graph->AdjacencyList[src];
    graph->AdjacencyList[src] = newNode;

    newNode = createNode(src, weight);
    newNode->next = graph->AdjacencyList[dest];
    graph->AdjacencyList[dest] = newNode;
}

// DFS to mark reachable vertices from v 
void dfs(struct Graph *graph, int v) {
    graph->visited[v] = 1;
    for (struct node *p = graph->AdjacencyList[v]; p; p = p->next) {
        if (!graph->visited[p->vertex]) {
            dfs(graph, p->vertex);
        }
    }
}

// Check if graph is connected via DFS from vertex 0 
bool isConnected(struct Graph *graph) {
    for (int i = 0; i < graph->NumberOfVertices; i++)
        graph->visited[i] = 0;
    dfs(graph, 0);
    for (int i = 0; i < graph->NumberOfVertices; i++)
        if (!graph->visited[i])
            return false;
    return true;
}

// Compare edges by descending weight for qsort 
int compareEdges(const void *a, const void *b) {
    struct Edge *e1 = (struct Edge*)a;
    struct Edge *e2 = (struct Edge*)b;
    return e2->weight - e1->weight;
}

//Free Graph
void freeGraph(struct Graph *graph) {
    for (int i = 0; i < graph->NumberOfVertices; i++) {
        struct node *p = graph->AdjacencyList[i];
        while (p) {
            struct node *nx = p->next;
            free(p);
            p = nx;
        }
    }
    free(graph->AdjacencyList);
    free(graph->visited);
    free(graph->edges);
    free(graph);
}


// Generate random connected graph
void generateRandomGraph(struct Graph *graph, int NumberOfVertices, int NumberOfEdges, unsigned int seed, int maxWeight) {
    srand(seed);

    for (int i = 1; i < NumberOfVertices; i++) {
        int j = rand() % i;
        int weight = rand() % maxWeight + 1;
        addEdge(graph, i, j, weight);
    }

    int edgesSoFar = NumberOfVertices - 1;
    int remaining = NumberOfEdges - edgesSoFar;
    if (remaining < 0) remaining = 0;
    if (NumberOfEdges > MAX_EDGES) remaining = MAX_EDGES - edgesSoFar;
    int added = 0;

    while (added < remaining) {
        int u = rand() % NumberOfVertices;
        int v = rand() % NumberOfVertices;
        if (u == v) continue;

        struct node *temp = graph->AdjacencyList[u];
        bool EdgeInGraph = false;
        while (temp) {
            if (temp->vertex == v) {
                EdgeInGraph = true;
                break;
            }
            temp = temp->next;
        }

        if (!EdgeInGraph) {
            int weight = rand() % maxWeight + 1;
            addEdge(graph, u, v, weight);
            added++;
        }
    }
}

 