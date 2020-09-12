#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
void quickSortR(int *a, int a1, int aN, int N);

int main() {
    int a[101] = {0};
    int i = 0, N = 0;
    scanf("%d", &N);
    for(i = 0; i < N; i++)
        scanf("%d", &a[i]);
    printf("Initial array:\n");
    for(i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n");
    printf("\n");
    if (N > 1)
        quickSortR(a, 0, N - 1, N);

    return 0;
}

void quickSortR(int *a, int a1, int aN, int N) {  // На входе индексы. a[a1] - первый, a[aN] - последний элемент массива.

    int i = a1, j = aN, d = 0, piv = 0, k = 0, g = aN - a1;

    piv = a[(((aN+a1))/2)];
    printf("Pivot index: %d , Pivot element: %d\n",(((aN+a1))/2), piv );

    for (k= a1; k <= aN; k++)
        if (a[k] > piv)
            g--;         // 'g' - количество элементов, меньших либо равных pivot'a

    d = a[g+a1];
    a[g+a1] = piv;
    a[((aN+a1))/2] = d;   //Меняем местами 'pivot' и 'a[g]'

    while ( i<j ) {
        while ( a[i] < piv ) i++;
        while ( a[j] > piv ) j--;

            d = a[i];
            a[i] = a[j];
            a[j] = d;
            i++; j--;
    }

    printf("Array after partition: ");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n");
    printf("\n");

    if ( g > 1 ) quickSortR(a,a1, g+a1-1, N);  //Рекурсивный вызов для левой и правой части, если есть что вызывать
    if ( aN > i ) quickSortR(a,a1+g, aN,N);
}