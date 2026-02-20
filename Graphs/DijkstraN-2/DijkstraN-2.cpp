#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Структура для представления ребра графа
typedef struct Edge {
    int to;             // вершина назначения
    long long weight;   // вес ребра (стоимость)
    struct Edge* next;  // указатель на следующее ребро
} Edge;

// Структура для представления графа
typedef struct Graph {
    int numVertices;    // количество вершин
    Edge** adjLists;    // массив списков смежности
} Graph;

// Структура для элемента очереди с приоритетом (для Дейкстры)
typedef struct {
    int vertex;         // вершина
    long long dist;     // расстояние до вершины
} QueueItem;

// Структура для очереди с приоритетом (мин-куча)
typedef struct {
    QueueItem* items;   // массив элементов
    int size;           // текущий размер
    int capacity;       // вместимость
} PriorityQueue;

// Создание графа
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = (Edge**)calloc(vertices + 1, sizeof(Edge*)); // индексация с 1
    return graph;
}

// Добавление ориентированного ребра в граф
void addEdge(Graph* graph, int from, int to, long long weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->to = to;
    newEdge->weight = weight;
    newEdge->next = graph->adjLists[from];
    graph->adjLists[from] = newEdge;
}

// Освобождение памяти графа
void freeGraph(Graph* graph) {
    for (int i = 1; i <= graph->numVertices; i++) {
        Edge* current = graph->adjLists[i];
        while (current != NULL) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjLists);
    free(graph);
}

// Создание очереди с приоритетом
PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->items = (QueueItem*)malloc((capacity + 1) * sizeof(QueueItem));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

// Обмен двух элементов в очереди
void swap(QueueItem* a, QueueItem* b) {
    QueueItem temp = *a;
    *a = *b;
    *b = temp;
}

// Восстановление свойства кучи вверх (при вставке)
void heapifyUp(PriorityQueue* pq, int index) {
    while (index > 1 && pq->items[index / 2].dist > pq->items[index].dist) {
        swap(&pq->items[index], &pq->items[index / 2]);
        index = index / 2;
    }
}

// Восстановление свойства кучи вниз (при извлечении минимума)
void heapifyDown(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index;
    int right = 2 * index + 1;

    if (left <= pq->size && pq->items[left].dist < pq->items[smallest].dist)
        smallest = left;
    if (right <= pq->size && pq->items[right].dist < pq->items[smallest].dist)
        smallest = right;

    if (smallest != index) {
        swap(&pq->items[index], &pq->items[smallest]);
        heapifyDown(pq, smallest);
    }
}

// Добавление элемента в очередь с приоритетом
void push(PriorityQueue* pq, int vertex, long long dist) {
    if (pq->size >= pq->capacity) return;
    pq->size++;
    pq->items[pq->size].vertex = vertex;
    pq->items[pq->size].dist = dist;
    heapifyUp(pq, pq->size);
}

// Извлечение элемента с минимальным расстоянием из очереди
QueueItem pop(PriorityQueue* pq) {
    QueueItem minItem = pq->items[1];
    pq->items[1] = pq->items[pq->size];
    pq->size--;
    heapifyDown(pq, 1);
    return minItem;
}

// Проверка, пуста ли очередь
int isEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

// Освобождение памяти очереди
void freePriorityQueue(PriorityQueue* pq) {
    free(pq->items);
    free(pq);
}

// Алгоритм Дейкстры для поиска кратчайших расстояний от start до всех вершин
long long* dijkstra(Graph* graph, int start) {
    int n = graph->numVertices;
    long long* dist = (long long*)malloc((n + 1) * sizeof(long long));
    int* visited = (int*)calloc(n + 1, sizeof(int));

    // Инициализация расстояний
    for (int i = 1; i <= n; i++) {
        dist[i] = LLONG_MAX;
    }
    dist[start] = 0;

    // Создание очереди с приоритетом
    PriorityQueue* pq = createPriorityQueue(n * n);
    push(pq, start, 0);

    while (!isEmpty(pq)) {
        QueueItem current = pop(pq);
        int u = current.vertex;

        if (visited[u]) continue;
        visited[u] = 1;

        // Обход всех соседей вершины u
        Edge* edge = graph->adjLists[u];
        while (edge != NULL) {
            int v = edge->to;
            long long weight = edge->weight;

            if (!visited[v] && dist[u] != LLONG_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                push(pq, v, dist[v]);
            }
            edge = edge->next;
        }
    }

    free(visited);
    freePriorityQueue(pq);
    return dist;
}

int main() {
    system("chcp 1251 > nul"); // Установка кодировки для русского языка

    int N, M;
    printf("Vvedite kolichestvo gorodov (N) i dorog (M): ");
    scanf("%d %d", &N, &M);

    if (N < 1 || N > 2000 || M < 0 || M > 20000) {
        printf("Oshibka: nepravilnye razmery (1 LE N LE 2000, 0 LE M LE 20000)\n");
        return 1;
    }

    // Создание графа
    Graph* graph = createGraph(N);

    printf("Vvedite %d dorog v formate 'u v w' (gorod otpravleniya, gorod naznacheniya, stoimost):\n", M);
    for (int i = 0; i < M; i++) {
        int u, v;
        long long w;
        scanf("%d %d %lld", &u, &v, &w);

        if (u < 1 || u > N || v < 1 || v > N || w < 0 || w > 1000000000) {
            printf("Oshibka: nepravilnye dannye rebra (1 LE u,v LE %d, 0 LE w LE 10^9)\n", N);
            freeGraph(graph);
            return 1;
        }

        addEdge(graph, u, v, w);
    }

    // Если нет дорог, но нужно попасть из 1 в N
    if (M == 0 && N > 1) {
        printf("-1\n");
        freeGraph(graph);
        return 0;
    }

    // Если начальная и конечная вершины совпадают
    if (N == 1) {
        printf("0\n");
        freeGraph(graph);
        return 0;
    }

    // 1. Находим кратчайшие расстояния от вершины 1 до всех вершин
    long long* distFromStart = dijkstra(graph, 1);

    // Проверяем, достижима ли конечная вершина без использования купона
    if (distFromStart[N] == LLONG_MAX) {
        printf("-1\n");
        free(distFromStart);
        freeGraph(graph);
        return 0;
    }

    // 2. Строим обратный граф для поиска расстояний до конечной вершины
    Graph* reverseGraph = createGraph(N);
    for (int u = 1; u <= N; u++) {
        Edge* edge = graph->adjLists[u];
        while (edge != NULL) {
            addEdge(reverseGraph, edge->to, u, edge->weight);
            edge = edge->next;
        }
    }

    // 3. Находим кратчайшие расстояния от вершины N до всех вершин в обратном графе
    long long* distToEnd = dijkstra(reverseGraph, N);

    // 4. Перебираем все ребра и пробуем применить купон
    long long minCost = LLONG_MAX;

    for (int u = 1; u <= N; u++) {
        Edge* edge = graph->adjLists[u];
        while (edge != NULL) {
            int v = edge->to;
            long long weight = edge->weight;

            // Проверяем, можно ли использовать купон на этом ребре
            // Путь: 1 -> ... -> u (бесплатно по ребру u->v) -> ... -> N
            if (distFromStart[u] != LLONG_MAX && distToEnd[v] != LLONG_MAX) {
                long long cost = distFromStart[u] + distToEnd[v];
                if (cost < minCost) {
                    minCost = cost;
                }
            }

            edge = edge->next;
        }
    }

    // 5. Вывод результата
    if (minCost == LLONG_MAX) {
        printf("-1\n");
    }
    else {
        printf("Minimalnaya stoimost: %lld\n", minCost);
    }

    // Освобождение памяти
    free(distFromStart);
    free(distToEnd);
    freeGraph(graph);
    freeGraph(reverseGraph);

    return 0;
}