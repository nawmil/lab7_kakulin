#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int** generate_adjacency_matrix(int size, int edge_percent) {
    int **G = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        G[i] = (int *)malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        G[i][i] = 0;
        for (int j = i + 1; j < size; j++) {
            int val = (rand() % 100 < edge_percent) ? 1 : 0;
            G[i][j] = val;
            G[j][i] = val;
        }
    }
    return G;
}

void free_matrix(int **G, int size) {
    for (int i = 0; i < size; i++) {
        free(G[i]);
    }
    free(G);
}

void print_matrix(int **G, int size) {
    printf("   ");
    for (int j = 0; j < size; j++) {
        printf("v%-3d", j);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("v%-2d ", i);
        for (int j = 0; j < size; j++) {
            printf("%-4d", G[i][j]);
        }
        printf("\n");
    }
}


void dfs_recursive(int v, int size, int **G, bool *NUM) {
    NUM[v] = true;
    printf("%d ", v);

    for (int i = 0; i < size; i++) {
        if (G[v][i] == 1 && !NUM[i]) {
            dfs_recursive(i, size, G, NUM);
        }
    }
}

void run_task_1(int size, int **G, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));

    printf("\nЗАДАНИЕ 1: Рекурсивный обход в глубину (Матрица смежности)\n");
    printf("Стартовая вершина: %d\n", start);
    printf("Порядок обхода вершин: ");

    if (!NUM[start]) {
        dfs_recursive(start, size, G, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            dfs_recursive(i, size, G, NUM);
        }
    }

    printf("\n");
    free(NUM);
}

void run_task_2(int size, int **G, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));
    int  *stack_v = (int  *)malloc(size * sizeof(int));
    int  *stack_i = (int  *)malloc(size * sizeof(int));

    printf("\nЗАДАНИЕ 2: Не рекурсивный обход в глубину (Матрица смежности)\n");
    printf("Стартовая вершина: %d\n", start);
    printf("Порядок обхода вершин: ");

    for (int k = 0; k < size; k++) {
        int s = (k == 0) ? start : ((start + k) % size);

        if (NUM[s]) continue;

        int top = -1;

        NUM[s] = true;
        printf("%d ", s);
        stack_v[++top] = s;
        stack_i[top] = 0;

        while (top >= 0) {
            int v = stack_v[top];
            int i = stack_i[top];

            while (i < size && !(G[v][i] == 1 && !NUM[i])) {
                i++;
            }

            if (i < size) {
                stack_i[top] = i + 1;  
                NUM[i] = true;  
                printf("%d ", i);
                stack_v[++top] = i;
                stack_i[top] = 0;
            } else {
                top--;       
            }
        }
    }

    printf("\n");
    free(NUM);
    free(stack_v);
    free(stack_i);
}

struct Node {
    int vertex;
    struct Node* next;
};

struct Node* createNode(int v) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

void addEdge(struct Node** adjLists, int src, int dest) {
    struct Node* newNode = createNode(dest);
    if (adjLists[src] == NULL) {
        adjLists[src] = newNode;
    } else {
        struct Node* temp = adjLists[src];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    struct Node* newNodeRev = createNode(src);
    if (adjLists[dest] == NULL) {
        adjLists[dest] = newNodeRev;
    } else {
        struct Node* temp = adjLists[dest];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNodeRev;
    }
}

void dfs_list_recursive(int v, struct Node** adjLists, bool *NUM) {
    NUM[v] = true;
    printf("%d ", v);

    struct Node* temp = adjLists[v];
    while (temp != NULL) {
        int adjVertex = temp->vertex;
        if (!NUM[adjVertex]) {
            dfs_list_recursive(adjVertex, adjLists, NUM);
        }
        temp = temp->next;
    }
}

void run_task_3(int size, int **G, int start) {
    struct Node** adjLists = (struct Node**)malloc(size * sizeof(struct Node*));
    for (int i = 0; i < size; i++) {
        adjLists[i] = NULL;
    }

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (G[i][j] == 1) {
                addEdge(adjLists, i, j);
            }
        }
    }

    bool *NUM = (bool *)calloc(size, sizeof(bool));

    printf("\nЗАДАНИЕ 3: Рекурсивный обход в глубину (Списки смежности)\n");
    printf("Стартовая вершина: %d\n", start);
    printf("Порядок обхода вершин: ");

    if (!NUM[start]) {
        dfs_list_recursive(start, adjLists, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            dfs_list_recursive(i, adjLists, NUM);
        }
    }

    printf("\n");

    for (int i = 0; i < size; i++) {
        struct Node* temp = adjLists[i];
        while (temp) {
            struct Node* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }

    free(adjLists);
    free(NUM);
}


int main() {
    int size;
    int edge_percent;
    int start;

    printf("Введите количество вершин графа: ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Ошибка: некорректное количество вершин.\n");
        return 1;
    }

    printf("Введите вероятность появления ребра в процентах (от 0 до 100): ");
    if (scanf("%d", &edge_percent) != 1 || edge_percent < 0 || edge_percent > 100) {
        printf("Ошибка: некорректное значение вероятности.\n");
        return 1;
    }



    srand(time(NULL));

    int **G = generate_adjacency_matrix(size, edge_percent);

    printf("\nСгенерированная матрица смежности G (%d вершин, вероятность ребра %d%%):\n",
           size, edge_percent);
    print_matrix(G, size);

    printf("Введите начальную вершину (от 0 до %d): ", size - 1);
    if (scanf("%d", &start) != 1 || start < 0 || start >= size) {
        printf("Ошибка: некорректная начальная вершина.\n");
        return 1;
    }
    clock_t t1, t2;
    double elapsed;

    t1 = clock();
    run_task_1(size, G, start);
    t2 = clock();
    elapsed = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Время выполнения задания 1: %.6f сек.\n", elapsed);

    t1 = clock();
    run_task_2(size, G, start);
    t2 = clock();
    elapsed = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Время выполнения задания 2: %.6f сек.\n", elapsed);

    t1 = clock();
    run_task_3(size, G, start);
    t2 = clock();
    elapsed = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Время выполнения задания 3: %.6f сек.\n", elapsed);

    free_matrix(G, size);

    return 0;
}