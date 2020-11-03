#include <stdio.h>
#include <math.h>
int m[65] = {0};
int p[65] = {0};
int a[65][65] = {0};
int g = 0;
void DFS(int from, int v, int N, int m2[], int p2[], int k);
int main() {
    int N = 0, i = 0, j = 0, x = 0, y = 0, k =0 ;
    scanf("%d%d%d", &N, &x, &y);
    for (i = 0; i < N*N; i ++)
        p[i]= -1;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if (i - 2 >= 0){
                if (j - 1 >= 0)
                    a[i * N + j][(i-2)*N + j - 1] = 1;
                if (j + 1 < N)
                    a[i * N + j][(i-2)*N + j + 1] = 1;
            }
            if (j - 2 >= 0){
                if (i - 1 >= 0)
                    a[i * N + j][(i-1)*N + j - 2] = 1;
                if (i + 1 < N)
                    a[i * N + j][(i+1)*N + j - 2] = 1;
            }
            if (j + 2 < N){
                if (i - 1 >= 0)
                    a[i * N + j][(i-1)*N + j + 2] = 1;
                if (i + 1 < N)
                    a[i * N + j][(i+1)*N + j + 2] = 1;
            }
            if (i + 2 < N){
                if (j - 1 >= 0)
                    a[i * N + j][(i+2)*N + j - 1] = 1;
                if (j + 1 < N)
                    a[i * N + j][(i+2)*N + j + 1] = 1;
            }
        }
    }
    printf("Graph:\n");
    for (i = 0; i < N * N; i++){
        printf("%d - ", i);
        for (j = 0; j < N * N; j++){
            if (a[i][j] == 1)
                printf("%d ", j);
        }
        printf("\n");
    }
    m[x * N + y] = 1;
    p[k] = x * N + y;
    k++;
    for (i = 0 ; i < N * N; i++){
        if (a[x * N + y][i] == 1)
            DFS(x * N + y, i, N * N, m, p, k);
    }
    if (g == 0)
        printf("Hamiltonian path:\nNo way");
    return 0;
}

void DFS(int from, int v, int N, int m2[], int p2[], int k){
    int m1[65] = {0}, p1[65] = {0} , c= 0;
    int b[65] = {0};
    for (int i = 0; i < N; i++){
        m1[i] = m2[i];
        p1[i] = p2[i];
    }
    if (m1[v] == 1)
        return;
    else{
        m1[v] = 1;
        p1[k] = v;
        k++;
        for (int i = 0; i < N; i++){
            if ((a[v][i] == 1) && (g == 0))
                DFS(v, i, N, m1, p1, k);
        }
        for (c = 0; (c < N) && (m1[c] == 1); c++);
        if ((c == N) && (g == 0)){
            g = 1;
            for (int i = 0; i < N; i++){
                b[p1[i]] = i;
            }
            printf("Hamiltonian path:\n");
            for (int i = 0; i < N; i++){
                printf("%d ", b[i]);
                if ((i + 1) % (int)sqrt((double) N) == 0)
                    printf("\n");
            }
            printf("\n");
        }
    }


}
