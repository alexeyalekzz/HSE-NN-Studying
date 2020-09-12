#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
void Insertion (int a[], int N);
int main() {
    int a[10000] = {0};
    int i = 0, N = 0;

    scanf("%d\n", &N);
    for (i = 0; i < N; i++)
        scanf("%d", &a[i]);

    printf("\nInitial array:\n");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);

    printf("\n");

    if (N > 1)
        Insertion(a, N);

    printf("\n");
    return 0;
}
void Insertion (int a[], int N){
    int i = 0, j = 0,c =0, d =0;
    for (i=1; i < N; i++) {
        c = a[i];

        for (j = i-1; j>=0 && a[j] > c; j--)
            a[j+1] = a[j];

        a[j+1] = c;

        printf("Insertion - Element #%d\n", ++d);
        for (int k = 0; k < N; k++)
            printf("%d ", a[k]);
        printf("\n");
    }
}

