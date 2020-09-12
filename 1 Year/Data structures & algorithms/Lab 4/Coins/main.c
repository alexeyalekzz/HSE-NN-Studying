#include <stdio.h>
#include <stdlib.h>

void func(int i, int j, int s);
void Path(int i, int j, int n, int m);
int c[100][100] = {0};
int b[100][100] = {0};
int si = 0;
int sj = 0;
int main() {
    char a[100][1000] = {0};
    char *end =  0;
    int j = 0, n = 0, m = 0, f = 0;
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; i++){
        fgets(a[i], 1000, stdin);
        if (a[i][j] == '\n'){
            fgets(a[i], 1000, stdin);
        }
    }
    for (int i = 0; i < n; i++){
        end = a[i];
        do{
            if ((*(end + 1) == 'S') || (*end == 'S')){
                b[i][j] = -1;
                j++;
                end += 2;
                si = i;
                sj = j-1;
                f++;
            }
            else{
                b[i][j] = (int) strtol(end, &end, 10);
                j++;
                f++;
            }
        }while ( f != m);
        j = 0;
        f = 0;
    }
    func(n - 1, m - 1, 0);
    printf("Path:\n");
    Path(si, sj, n , m);
    printf("\n");
    printf("Coins: %d",c[si][sj]);

    return 0;
}
void func(int i, int j, int s){
    if (b[i][j] != -1)
        s += b[i][j];
    if (c[i][j] < s)
        c[i][j] = s;
    if ((i == si) && (j == sj)){
        return;
    }
    else if (c[i][j] <= s){
        if (i - 1 >= 0)
            func(i - 1, j, s);
        if (j - 1 >= 0)
            func(i, j - 1, s);
    }
}
void Path(int i, int j, int n, int m){
    printf("(%d,%d) ", i,j);
    if ((i != n - 1) || (j != m - 1)){
        if ((j + 1 < m) && (c[i][j + 1] >= c[i + 1][j]))
            Path(i, j + 1, n, m);
        else if (i + 1 < n)
            Path(i + 1, j, n, m);
    }
}
