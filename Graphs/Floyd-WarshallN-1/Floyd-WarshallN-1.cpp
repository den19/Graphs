#include <stdio.h>
#include <locale.h>

#define MAX_N 100
#define INF 1000000000

int main() {
    // Устанавливаем русскую локаль для корректного отображения текста
    setlocale(LC_ALL, "Russian");

    int N;
    int dist[MAX_N][MAX_N];

    printf("===========================================\n");
    printf("  PROGRAMMA RASCHYOTA KRATCHAISHIH PUTEY\n");
    printf("     (Algoritm Floyda-Uorshella)\n");
    printf("===========================================\n\n");

    // Чтение количества вершин
    printf("Vvedite kolichestvo vershin (N): ");
    scanf_s("%d", &N);

    printf("\nVvedite matritsu smezhnosti %d x %d:\n", N, N);
    printf("(Element na glavnoy diagonali vsegda 0)\n");
    printf("Esli puti net, mozhno ukazat bolshoe chislo, naprimer 100\n\n");

    // Чтение матрицы смежности
    for (int i = 0; i < N; i++) {
        printf("Stroka %d: ", i + 1);
        for (int j = 0; j < N; j++) {
            scanf_s("%d", &dist[i][j]);
        }
    }

    printf("\nObrabatyvayu dannye...\n");

    // Алгоритм Флойда-Уоршелла
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Вывод результатов
    printf("\n===========================================\n");
    printf("     MATRITSA KRATCHAISHIH RASSTOYANIY\n");
    printf("===========================================\n\n");

    printf("     ");
    for (int j = 0; j < N; j++) {
        printf("  v%d  ", j + 1);
    }
    printf("\n");

    printf("     ");
    for (int j = 0; j < N; j++) {
        printf("-----");
    }
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("iz v%d |", i + 1);
        for (int j = 0; j < N; j++) {
            printf(" %3d ", dist[i][j]);
        }
        printf("\n");
    }

    printf("\n===========================================\n");
    printf("Raschet uspeshno zavershen! Spasibo za ispolzovanie programmy.\n");
    getchar();
    return 0;
}