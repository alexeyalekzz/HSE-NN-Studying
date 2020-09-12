#include <stdio.h>
#define s 20

int n = 0;
int a[s][s] = {0};
int cost[s] = {0};
void GreedyCost(int v, int start, int *b1, int c);
void GreedyPath(int v, int start, int *b1, int c);
int main() {
    int b[s] = {0}, g = 0, min = 1000;
    scanf("%d", &n);
    for (int i = 0; i < n ; i++){
        for (int j = 0; j < n; j++)
            scanf("%d", &a[i][j]);
    }
    for (int i = 0; (i < n); i++){
        GreedyCost(i, i, b, 0);
    }
    for (int i = 0; (i < n); i++){
        if ((cost[i] < min) && (cost[i] != -1)){
            min = cost[i];
            g = i;
        }
    }
    if (min == 1000)
        printf("Lost\n");
    else{
        printf("Path:\n");
        GreedyPath(g, g, b, 0);
        printf("Cost: %d", cost[g]);
    }

    for (int i = 0; (i < n); i++)
        printf("%d ", cost[i]);

    return 0;
}
void GreedyCost(int v, int start, int *b1, int c){
   // printf("Step: %d, top: %d, ",c + 1,  v);
    int next = -1, min = 99;
    int b[s] = {0};
    for (int j = 0; j < n; j++)
        b[j] = b1[j];
    b[v] = 1;
    c++;
    if (c == n)
        b[start] = 0;

    for (int j = 0; j < n; j++){
        if ((a[v][j] > 0) && (a[v][j] < min) && (b[j] == 0)){
            next = j;
            min = a[v][j];
        }
    }
   /* printf("next: %d\t", next);
    for (int j = 0; j < n; j++)
        printf("%d ", b[j]);
    printf("\n");*/
    if ((next != start) && (next != -1)){
        cost[start] += a[v][next];
        GreedyCost(next, start, b, c);
    }
    else if (next == -1){
        cost[start] = -1;
        return;
    }
    else{
        cost[start] += a[v][next];
    }
}

void GreedyPath(int v, int start, int *b1, int c){
    printf("%d ", v);
    int next = -1, min = 99;
    int b[s] = {0};
    for (int j = 0; j < n; j++)
        b[j] = b1[j];
    b[v] = 1;
    c++;
    if (c == n)
        b[start] = 0;
    for (int j = 0; j < n; j++){
        if ((a[v][j] > 0) && (a[v][j] < min) && (b[j] == 0)){
            next = j;
            min = a[v][j];
        }
    }
    if (c == n)
        printf("%d \n", next);
    if ((next != start) && (next != -1)){
        GreedyPath(next, start, b, c);
    }
}