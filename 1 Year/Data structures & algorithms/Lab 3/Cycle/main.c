#include <stdio.h>

void DFS  (int start, int from, int N, int m2[]);
int a[100][100] = {0};
int p[100] = {0};
int g = 0;
int main() {
    int N = 0, e = 0, i = 0, a1 = 0, a2 = 0 , j  =0;
    int m[100] = {0};

    scanf("%d%d",&N,&e);
    for (i = 0; i < e; i++){
        scanf("%d%d", &a1,&a2);
        a[a1][a2] = 1;
    }
    for (i = 0; i < N; i++){
        p[i] = -1;
    }
    i = 0;
    if (e > 1){
        for (i = 0; (i < N) && (g == 0); i++){
            DFS(i, i, N, m);
        }
        if (g == 0)
            printf("No cycles");
    }
    else
        printf("No cycles");

    return 0;
}
void DFS (int start, int from, int N, int m2[]) {
    int i = 0, j = 0, k = 0, v = 0;
    int b[100] = {0}, m1[100] = {0};
    for (i = 0; i < N; i++)
        m1[i] = m2[i];
    m1[from] = 1;

    for (v = 0; v < N; v++){

        if (a[from][v] == 1){
            if (v == start){
                p[v] = from;
                for (j = p[v]; j != start; j = p[j]){
                    b[k] = j;
                    k++;
                }
                b[k] = j;
                if (g == 0){
                    printf("Cycle:\n");
                    for (int y = k; y >=0; y--)
                        printf("%d ", b[y]);
                    printf("\n");
                }
                g = 1;
                return;
            }
            if ((m1[v] != 1) && (g == 0)){
                p[v] = from;
                m1[v] = 1;
                DFS(start, v, N, m1);
            }
        }
    }
}