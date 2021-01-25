#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define s 5

void Dijkstra (int a[][s], int start, int N);
int Lsearch (int array[], int N, int target);
int main() {
    int a[s][s] = {0};
    int i = 0, j = 0, N = s, start = 0;
    srand(time(0));
    for (i = 0; i < N; i++){
        for (j = 0; j < i; j++){
            a[i][j] = rand() % 3;
            a[j][i] = a[i][j];
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d  ", a[i][j]);
        }
        printf("\n");
    }
    scanf("%d", &start);

    Dijkstra(a,start,N);
    return 0;
}
int Lsearch (int a[], int N, int targ) {
    int i = 0;
    while ((a[i] != targ) && (i < N))
        i++;
    if (i == N)
        return -1;
    else
        return i;
}

void Dijkstra (int a[][s], int start, int N){
    int used[s] = {0};
    int dest[s] = {0};
    int prev[s] = {0};
    int i = 0, min = INT_MAX, j = 0;
    for (i = 0; i < N; i++)
        dest[i] = INT_MAX;
    dest[start] = 0;
    while (Lsearch(used, N,0) != -1){

        for (i = 0; i < N; i++){
            if ((dest[i] < min) && (used[i] == 0))
                min = i;
        }
        start = min;
        printf("min: %d\n", min);

        for (j = 0; j < N; j++){
            if (a[start][j] != 0){
                if (a[start][j] + dest[start] < dest[j]){
                    dest[j] = a[start][j] + dest[start];
                    prev[j] = start;
                }
            }
        }
        used[start] = 1;
        printf("Used:\n");
        for (i = 0; i < N; i++)
            printf("%d ", used[i]);
        printf("\ndest:\n");
        for (i = 0; i < N; i++)
            printf("%d ", dest[i]);
        printf("\nprev:\n");
        for (i = 0; i < N; i++)
            printf("%d ", prev[i]);
        printf("\n");
        min = INT_MAX;
    }
    printf("\n");

    for (i = 0; i < N; i++)
        printf("%d ", dest[i]);
}
