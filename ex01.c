/*
Dado um conjunto de compostos químicos fornecer uma lista dos compostos químicos aos quais esse composto tem relação de imcopatibilidade

Entrada: N(número de compostos - 1 a 600) e M(relações de imcompatibilidade - 1 a 10_000)
uma relação de imcompatibilidade ocorre entre dois compostos distintos e é valida nas duas direções, ou seja, se A é incompatível a B -> B é incompatível a A

Saída: determinar se é possível manter os compostos em duas linhas de produção ou não

Representação das relações com grafos não direcionados, considerando que é um grafo não direcionado representação por matriz de adjacências
*/

#include <stdio.h>
#include <stdlib.h>


int** initializeGraph (int **graph, int n){

    graph = malloc (n * sizeof(int*));
    for (int k = 0; k < n; k++){
        graph[k] = malloc (n * sizeof(int));
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            graph[i][j] = -3;
        }
    }
    return graph;
}

void insertGraph (int **graph, int compostA, int compostB){
    graph[compostA][compostB] = 1;
    graph[compostB][compostA] = 1;
}

void printGraph (int **graph, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    } 
}

int paintCompost (int **graph, int *bicolor, int compost, int n, int color){
    /* pinta o composto*/
    bicolor[compost] = color;
    /* checa se os vizinhos do composto tem cor igual a dele*/
    for (int i = 0; i < n; i++){
    /* se o composto tem a mesma cor que algum dos seus vizinhos, não tem como ter duas linhas de produção*/
        if (graph[compost][i] == 1 && bicolor[i] == bicolor[compost]){
            return -1;
        }
    }

    //colocar o i pra iniciar no primeiro composto com relaçao igual a 1
    /* para cada composto ainda não colorido do composto atual chamar a função recursivamente para colorir o composto vizinho com uma cor diferente do atual*/
    for (int i = 0; i < n; i++){
        if (graph[compost][i] == 1 && bicolor[i] == -1){
            if(bicolor[compost] == 0){
                int equalColors = paintCompost(graph, bicolor, i, n, 1);
                if (equalColors == -1){
                    return -1;
                }
            } else {
                int equalColors =  paintCompost(graph, bicolor, i, n, 0);
                if (equalColors == -1){
                    return -1;
                }
            }
        }
    }
    return 0;
}

void paintGraph (int **graph, int n, int *bicolor){
    /*
    São três casos:
    1) Composto ainda não foi pintado
    2) Composto pintado tem a mesma cor do composto vizinho (não consegue separar em duas linhas de produção)
    3) Composto pintado em cor diferente diferente da cor do composto vizinho 
    */

   int paint = 0;
   int equalColors = 0;
   int compost = 0;

   while (equalColors == 0 && paint < n){
    /* ainda não foi pintado*/
    if (bicolor[compost] == -1){
        equalColors = paintCompost(graph, bicolor, compost, n, 1);
    }
    // substituir o paint so por compost
    paint++;
    compost = paint;
   }

   if (equalColors == 0){
    printf("Eh possivel produzir com duas linhas de producao.\n");
    return;
   }
   printf("Nao eh possivel produzir com duas linhas de producao.\n");
}


int main (){
    int n, m, compostA, compostB, **graph, *bicolor;

    scanf("%d %d", &n, &m);

    /* bicolor 0 = preto, 1 = branco, -1 = sem cor*/
    bicolor = malloc (n * sizeof(int));
    for(int i = 0; i < n; i++){ 
        bicolor[i] = -1;
    }

    graph = initializeGraph(graph, n);
    for (int i = 0; i < m; i++){
        scanf("%d %d", &compostA, &compostB);
        insertGraph(graph, compostA, compostB);
    }
    paintGraph(graph, n, bicolor);
    return 0;
}