#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_EDGES 1000

struct node {
    int vertex;
    int weight;
    struct node *next;
};

struct Edge {
    int u, v, weight;
};

struct Graph {
    int numVertices;
    struct node **adjLists;
    int *visited;
    struct Edge *edges;
    int edgeCount;
};

/* --- adjacency‐list helper functions --- */

struct node *createNode(int v, int weight) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

/* add one directed adjacency (for reverse-delete restore) */
void addAdjEdge(struct Graph *g, int src, int dest, int weight) {
    struct node *n = createNode(dest, weight);
    n->next = g->adjLists[src];
    g->adjLists[src] = n;
}

/* remove the first adjacency from src to dest */
void removeAdjEdge(struct Graph *g, int src, int dest) {
    struct node **pp = &g->adjLists[src];
    while (*pp) {
        if ((*pp)->vertex == dest) {
            struct node *toFree = *pp;
            *pp = toFree->next;
            free(toFree);
            return;
        }
        pp = &(*pp)->next;
    }
}

/* --- graph construction & utilities --- */

struct Graph *createGraph(int vertices) {
    struct Graph *g = malloc(sizeof(struct Graph));
    g->numVertices = vertices;
    g->adjLists = calloc(vertices, sizeof(struct node *));
    g->visited  = calloc(vertices, sizeof(int));
    g->edges     = malloc(MAX_EDGES * sizeof(struct Edge));
    g->edgeCount = 0;
    return g;
}

/* adds an undirected edge:
   - records it once in g->edges[]
   - adds to both adjacency lists */
void addEdge(struct Graph *g, int src, int dest, int weight) {
    if (g->edgeCount < MAX_EDGES) {
        g->edges[g->edgeCount].u = src;
        g->edges[g->edgeCount].v = dest;
        g->edges[g->edgeCount].weight = weight;
        g->edgeCount++;
    }
    addAdjEdge(g, src, dest, weight);
    addAdjEdge(g, dest, src, weight);
}

/* reset visited[] and do a DFS from v */
void dfsConn(struct Graph *g, int v) {
    g->visited[v] = 1;
    for (struct node *p = g->adjLists[v]; p; p = p->next) {
        if (!g->visited[p->vertex])
            dfsConn(g, p->vertex);
    }
}

/* check connectivity from vertex 0 */
bool isConnected(struct Graph *g) {
    for (int i = 0; i < g->numVertices; i++)
        g->visited[i] = 0;
    dfsConn(g, 0);
    for (int i = 0; i < g->numVertices; i++)
        if (!g->visited[i])
            return false;
    return true;
}

/* comparison for qsort: descending weight */
int compareEdges(const void *a, const void *b) {
    struct Edge *e1 = (struct Edge*)a;
    struct Edge *e2 = (struct Edge*)b;
    return e2->weight - e1->weight;
}

/* --- reverse‐delete MST --- */

void reverseDeleteMST(struct Graph *g) {
    /* sort edges by descending weight */
    qsort(g->edges, g->edgeCount, sizeof(struct Edge), compareEdges);

    int totalWeight = 0;
    printf("Edge   Weight\n");
    for (int i = 0; i < g->edgeCount; i++) {
        int u = g->edges[i].u;
        int v = g->edges[i].v;
        int w = g->edges[i].weight;

        /* try removing it */
        removeAdjEdge(g, u, v);
        removeAdjEdge(g, v, u);

        if (!isConnected(g)) {
            /* if disconnected, restore and keep in MST */
            addAdjEdge(g, u, v, w);
            addAdjEdge(g, v, u, w);
            totalWeight += w;
            printf("%2d - %2d    %d\n", u, v, w);
        }
        /* otherwise the removal is permanent (edge not in MST) */
    }
    printf("Total weight of MST = %d\n", totalWeight);
}

/* --- cleanup --- */

void freeGraph(struct Graph *g) {
    for (int i = 0; i < g->numVertices; i++) {
        struct node *p = g->adjLists[i];
        while (p) {
            struct node *nx = p->next;
            free(p);
            p = nx;
        }
    }
    free(g->adjLists);
    free(g->visited);
    free(g->edges);
    free(g);
}

/* --- example usage --- */

int main(void) {
    /* Example graph (same as your Prim example, 9 vertices 0–8) */
    struct Graph *g = createGraph(9);

    addEdge(g, 0, 1, 4);
    addEdge(g, 0, 7, 8);
    addEdge(g, 1, 2, 8);
    addEdge(g, 1, 7, 11);
    addEdge(g, 2, 3, 7);
    addEdge(g, 2, 8, 2);
    addEdge(g, 2, 5, 4);
    addEdge(g, 3, 4, 9);
    addEdge(g, 3, 5, 14);
    addEdge(g, 4, 5, 10);
    addEdge(g, 5, 6, 2);
    addEdge(g, 6, 7, 1);
    addEdge(g, 6, 8, 6);
    addEdge(g, 7, 8, 7);

    reverseDeleteMST(g);

    freeGraph(g);
    return 0;
}
