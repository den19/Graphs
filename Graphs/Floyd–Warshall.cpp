#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

#define INF 1000000000  // достаточно большое число, больше любого возможного пути

int main() {
    // Настройка кодировки для правильного отображения русского текста в Windows
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Также устанавливаем локаль для поддержки русского языка
    setlocale(LC_ALL, "Russian");

    int N;

    printf("========================================\n");
    printf("   POISK MAKSIMALNOGO KRATCHAISHEGO RASSTOYANIYA\n");
    printf("         V ORIENTIROVANNOM GRAFE\n");
    printf("========================================\n\n");

    printf("Vvedite kolichestvo vershin grafa N [1 .. 100]: ");
    scanf_s("%d", &N);

    // Очистка буфера ввода
    while (getchar() != '\n');

    // Проверка корректности ввода
    if (N < 1 || N > 100) {
        printf("\nOshibka! N dolzhno byt' ot 1 do 100.\n");
        printf("Nazhmite Enter dlya vyhoda...");
        getchar();
        return 1;
    }

    printf("\nVvedite matricu smezhnosti %d x %d:\n", N, N);
    printf("(celye chisla, gde -1 oznachaet otsutstvie rebra)\n");
    printf("(na glavnoi diagonali vsegda nuli)\n\n");

    // Создаём матрицу расстояний
    int** dist = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        dist[i] = (int*)malloc(N * sizeof(int));
        printf("Stroka %d: ", i + 1);
        for (int j = 0; j < N; j++) {
            int val;
            scanf_s("%d", &val);
            if (val == -1) {
                dist[i][j] = INF;  // нет ребра
            }
            else {
                dist[i][j] = val;  // есть ребро
            }
        }
        // Очистка буфера после каждой строки
        while (getchar() != '\n');
    }

    printf("\nObrabotka dannyh...\n");

    // Алгоритм Флойда–Уоршелла
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    // Поиск максимального кратчайшего расстояния
    int max_dist = -1;
    int start_vertex = -1;
    int end_vertex = -1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j && dist[i][j] < INF) {
                if (dist[i][j] > max_dist) {
                    max_dist = dist[i][j];
                    start_vertex = i + 1;  // +1 для понятной нумерации вершин
                    end_vertex = j + 1;
                }
            }
        }
    }

    printf("\n========================================\n");
    printf("                 REZULTAT\n");
    printf("========================================\n");

    if (max_dist == -1) {
        printf("\nV grafe net dostizhimyh par vershin!\n");
    }
    else {
        printf("\nMaksimal'noe kratchaishee rasstoyanie: %d\n", max_dist);
        printf("Dostigaetsya mezhdu vershinami: %d -> %d\n", start_vertex, end_vertex);
        printf("\n(rasstoyanie %d - eto dlina kratchaishego puti iz vershiny %d v vershiny %d)\n",
            max_dist, start_vertex, end_vertex);
    }

    printf("\n========================================\n");

    // Освобождение памяти
    for (int i = 0; i < N; i++) {
        free(dist[i]);
    }
    free(dist);

    printf("\nProgramma zavershena. Nazhmite Any Key dlya vyhoda...");
    getchar();

    return 0;
}