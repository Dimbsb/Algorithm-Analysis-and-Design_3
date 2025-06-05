#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_EDGES 1000

/* Δομές */

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

/* --- adjacency‐list helper functions --- */

/**
 * Δημιουργεί έναν κόμβο λίστας γειτόνων με κορυφή v και βάρος weight.
 */
struct node *createNode(int v, int weight) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

/**
 * Προσθέτει κατευθυνόμενη ακμή src→dest με βάρος weight στην adjacency‐list.
 */
void addAdjEdge(struct Graph *graph, int src, int dest, int weight) {
    struct node *n = createNode(dest, weight);
    n->next = graph->AdjacencyList[src];
    graph->AdjacencyList[src] = n;
}

/**
 * Αφαιρεί την πρώτη εμφάνιση ακμής src→dest από την adjacency‐list.
 */
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

/* --- graph construction & utilities --- */

/**
 * Δημιουργεί κενό γράφο με δοθείσες κορυφές, χωρίς ακμές.
 */
struct Graph *createGraph(int vertices) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->NumberOfVertices = vertices;
    graph->AdjacencyList = calloc(vertices, sizeof(struct node *));
    graph->visited  = calloc(vertices, sizeof(int));
    graph->edges     = malloc(MAX_EDGES * sizeof(struct Edge));
    graph->edgeCount = 0;
    return graph;
}

/**
 * Προσθέτει ΜΗ‐ΠΡΟΣΑΝΑΤΟΛΙΣΜΕΝΗ ακμή src–dest με βάρος weight:
 * 1) Καταγράφει την (src,dest,weight) στον πίνακα edges[] (μία φορά).
 * 2) Καλεί addAdjEdge δύο φορές για src→dest και dest→src.
 */
void addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->edgeCount < MAX_EDGES) {
        graph->edges[graph->edgeCount].u = src;
        graph->edges[graph->edgeCount].v = dest;
        graph->edges[graph->edgeCount].weight = weight;
        graph->edgeCount++;
    }
    addAdjEdge(graph, src, dest, weight);
    addAdjEdge(graph, dest, src, weight);
}

/**
 * DFS για να σηματοδοτήσει (visited=1) όλες τις κορυφές προσβάσιμες από v.
 */
void dfsConn(struct Graph *graph, int v) {
    graph->visited[v] = 1;
    for (struct node *p = graph->AdjacencyList[v]; p; p = p->next) {
        if (!graph->visited[p->vertex]) {
            dfsConn(graph, p->vertex);
        }
    }
}

/**
 * Ελέγχει αν ο γράφος είναι συνδεδεμένος (κάνοντας DFS από κορυφή 0).
 */
bool isConnected(struct Graph *graph) {
    for (int i = 0; i < graph->NumberOfVertices; i++)
        graph->visited[i] = 0;
    dfsConn(graph, 0);
    for (int i = 0; i < graph->NumberOfVertices; i++)
        if (!graph->visited[i])
            return false;
    return true;
}

/**
 * Συνάρτηση σύγκρισης για qsort: ταξινόμηση κατά φθίνουσα βαρών.
 */
int compareEdges(const void *a, const void *b) {
    struct Edge *e1 = (struct Edge*)a;
    struct Edge *e2 = (struct Edge*)b;
    return e2->weight - e1->weight;
}

/* --- reverse‐delete MST --- */

/**
 * Υλοποιεί τον αλγόριθμο Reverse‐Delete για MST:
 * Ταξινομεί τις ακμές κατά φθίνουσα βαρών. Στη συνέχεια, βρόχος:
 *   α) Αφαιρεί προσωρινά την ακμή (u,v) από τις adjacency‐lists.
 *   β) Καλεί isConnected.  Αν επιστρέψει false, επαναφέρει την ακμή
 *      και την κρατά στο MST. Διαφορετικά, την αφήνει κομμένη.
 */
void reverseDeleteMST(struct Graph *graph) {
    qsort(graph->edges, graph->edgeCount, sizeof(struct Edge), compareEdges);

    int totalWeight = 0;
    printf("Edge   Weight\n");
    for (int i = 0; i < graph->edgeCount; i++) {
        int u = graph->edges[i].u;
        int v = graph->edges[i].v;
        int w = graph->edges[i].weight;

        /* Προσωρινή αφαίρεση της ακμής από τις δύο λίστες */
        removeAdjEdge(graph, u, v);
        removeAdjEdge(graph, v, u);

        if (!isConnected(graph)) {
            /* Αν αποσυνδεθεί, επαναφέρουμε στο MST */
            addAdjEdge(graph, u, v, w);
            addAdjEdge(graph, v, u, w);
            totalWeight += w;
            printf("%2d - %2d    %d\n", u, v, w);
        }
        /* Διαφορετικά την αφήνουμε τελείως κομμένη (remote) */
    }
    printf("Total weight of MST = %d\n", totalWeight);
}

/**
 * Απελευθερώνει όλη τη δυναμική μνήμη του γράφου.
 */
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

/* --- generateRandomGraph + printGraph από τον Prim κώδικα --- */

/**
 * Γεννάει τυχαίο γράφο με NumberOfVertices κορυφές και NumberOfEdges ακμές.
 * Διασφαλίζει αρχικά spanning‐tree + επιπλέον ακμές, χρησιμοποιώντας rand() με seed.
 * Καλεί addEdge για να ενημερώσει και τις adjacency‐lists και τον πίνακα edges[].
 */
void generateRandomGraph(struct Graph *graph, int NumberOfVertices, int NumberOfEdges, unsigned int seed, int maxWeight) {
    srand(seed);

    /* 1. Φτιάχνουμε ένα τυχαίο spanning tree ώστε να είναι συνδεδεμένος ο γράφος */
    for (int i = 1; i < NumberOfVertices; i++) {
        int j = rand() % i;                // j ∈ [0, i−1]
        int weight = rand() % maxWeight + 1;
        addEdge(graph, i, j, weight);
    }

    /* 2. Προσθέτουμε τις υπόλοιπες ακμές τυχαία, χωρίς διπλοεγγραφές */
    int edgesSoFar = NumberOfVertices - 1;
    int remaining = NumberOfEdges - edgesSoFar;
    if (remaining < 0) remaining = 0;
    if (NumberOfEdges > MAX_EDGES) remaining = MAX_EDGES - edgesSoFar;
    int added = 0;

    while (added < remaining) {
        int u = rand() % NumberOfVertices;
        int v = rand() % NumberOfVertices;
        if (u == v) continue;

        /* Έλεγχος αν ήδη υπάρχει ακμή u–v */
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

/**
 * Εκτυπώνει τον γράφο ως adjacency‐list: σε κάθε γραμμή i, όλοι οι γείτονες.
 */
void printGraph(struct Graph *graph) {
    printf("Graph adjacency list:\n");
    for (int i = 0; i < graph->NumberOfVertices; i++) {
        printf("%d:", i);
        struct node *temp = graph->AdjacencyList[i];
        while (temp) {
            printf(" -> (%d, w=%d)", temp->vertex, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

/* --- κύριο πρόγραμμα που συγκρίνει μετράει και reverse‐delete --- */

int main(void) {
    /* Παράμετροι τυχαίου γράφου */
    int NumberOfVertices = 10;   // Αριθμός κορυφών
    int NumberOfEdges    = 15;   // Σύνολο ακμών
    unsigned int seed    = 20;      // Seed για rand()
    int maxWeight        = 20;      // Μέγιστο βάρος ακμής

    /* Δημιουργία κενού γράφου */
    struct Graph *graph = createGraph(NumberOfVertices);

    /* Γέμισμα με τυχαίες ακμές και εκτύπωση */
    generateRandomGraph(graph, NumberOfVertices, NumberOfEdges, seed, maxWeight);
    printGraph(graph);

    /* Εκτέλεση Reverse‐Delete MST και μέτρηση χρόνου */
    clock_t start = clock();
    reverseDeleteMST(graph);
    clock_t end = clock();

    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("EXECUTION TIME FOR REVERSE-DELETE MST IS: %.6lf SECONDS\n", cpu_time_used);

    freeGraph(graph);
    return 0;
}
