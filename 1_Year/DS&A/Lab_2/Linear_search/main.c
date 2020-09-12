#include <stdio.h>

int Lsearch (int array[], int Size, int target);


int main() {
    int a[100] = {6, 2, 0, 3, 7}, i = 0, targ = 4, N = 5;

    /*scanf("%d%d", &N, &targ);
    for (i = 0; i < N; i++)
        scanf("%d", &a[i]);
    printf("Initial array:\n");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n"); i = 0;
    printf("Linear search:\n");
    while ((a[i] != targ) && (i < N-1))
        printf("%d ", a[i++]);
    printf("%d \n", a[i]);*/

    printf("%d", Lsearch(a, N, targ));
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