/* 
Algoritmos em Grafos
Nome: Cínthia Souza Costa   RA:792173
*/

/* 
    Dado um projeto de cruzamento de ruas, que podem ser de mão única ou mão dupla verificar se o projeto se adequa a seguinte condição a partir de um cruzamento chegar a qualquer outro cruzamento do projeto.
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct Vertex{
    int id;
    struct Vertex *next;
} Vertex;

typedef struct 
{
    Vertex *adjList;
} AdjacencyList;

typedef struct {
    int n; 
    int *color;
    int *d;
    int *f;
    int time;
    AdjacencyList *adjacencies;

} Graph;

typedef struct {
    int vertex;
    int finishTime;
} Order;

Graph *initializeGraph (int n){

    Graph *G = (Graph*) malloc (sizeof(Graph));

    int *color = (int*) malloc (n * sizeof(int));
    int *d = (int*) malloc (n * sizeof(int));
    int *f = (int*) malloc (n * sizeof(int));
    AdjacencyList *adjacencies = (AdjacencyList*) malloc (n * sizeof(AdjacencyList));
    for (int i = 0; i < n; i++){ 
        color[i] = 0;
        d[i] = 0;
        f[i] = 0;
        adjacencies[i].adjList = NULL;
    }

    G->n = n;
    G->time = 0;
    G->color = color;
    G->d = d;
    G->f = f;
    G->adjacencies = adjacencies;
    return G;
}

void insertEdge (int A, int B, Graph *G){

    /* criação do novo vértice */
    Vertex *newVertex = (Vertex*) malloc (sizeof(Vertex));
    newVertex->id = B;
    newVertex->next = NULL; //pode ser eliminado depois
    /* inserção ordenada na lista de adjacências */

    // caso a lista ainda esteja vazia e seja primeira inserção na lista OU o novo valor de B seja o menor na lista
    if (G->adjacencies[A].adjList == NULL || G->adjacencies[A].adjList->id > B){
        newVertex->next = G->adjacencies[A].adjList;
        G->adjacencies[A].adjList = newVertex;
    } else { 
        // se já existem itens na lista procurar pelo item imediatamente maior que B
        Vertex *aux = G->adjacencies[A].adjList;
        /* enquanto não atinge o fim da lista ou não encontra um elemento maior segue buscando local pra inserir o novo vértice*/
        while (aux->next != NULL && aux->next->id < B ){
            aux = aux->next;
        }

        
        /* colocar o novo vértice no fim da lista */
        if (aux->next == NULL){
            aux->next = newVertex;
        } else if (aux->next->id < B ){
            newVertex-> next = aux->next;
            aux->next = newVertex;
        }
    }
}

void dfsVisit (Graph *G, int v) {

    //marcar grafo como visitando = cinza
    G->color[v] = 1;
    G->d[v] = G->time + 1 ;
    G->time++;
    Vertex *son = G->adjacencies[v].adjList;
    while (son != NULL){
        if (G->color[son->id] == 0){
            dfsVisit(G, son->id);
        }
        son = son->next;
    }
    G->color[v] = 2;
    G->f[v] = G->time + 1;
    G->time++;
}

void dfsGraph (Graph *G){
    for (int i = 0; i < G->n; i++){
        if (G->color[i] == 0){
            dfsVisit (G, i);
        }    
    }
}

int dfsGT (Graph *G, Order *finish){
    int components = 0;
    for (int i = 0; i < G->n; i++){
        if (G->color[finish[i].vertex] == 0){
            dfsVisit (G, finish[i].vertex);
            components++;
        }    
    }
    return components;
}

int compare(const void *a, const void *b) {
    Order *oa = (Order *)a;
    Order *ob = (Order *)b;
    return (ob->finishTime - oa->finishTime);
}

int main () {
    int n, m, A, B, D, components;
    Graph *graph, *graphT;
    Order *finish;

    scanf("%d %d", &n, &m);

    graph = initializeGraph (n);
    graphT = initializeGraph (n);
    for (int i = 0; i < m; i++){
       scanf("%d %d %d", &A, &B, &D);
       insertEdge(A, B, graph);
       insertEdge(B, A, graphT);

       /* checar se deve ser inserida a aresta de volda, quando D = 2*/
       if (D == 2) {
        insertEdge(B, A, graph);
        insertEdge(A, B, graphT);
       } 
    }
    dfsGraph (graph);

    finish = (Order*) malloc (n * sizeof(Order));
    for (int i = 0; i < n; i++){
        finish[i].vertex = i;
        finish[i].finishTime = graph->f[i]; 
    }
    qsort(finish, n, sizeof(Order), compare);
    components = dfsGT (graphT, finish);

    if (components > 1){
        printf("Inadequado\n");
    } else {
        printf("Adequado\n");
    }
    
    return 0;
}