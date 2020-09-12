#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

double rangeof(double a[], int n);
void BucketSort(double *a, int n);
void InsSort(double a[], int n);

int main() {
    double a[101] = {0};
    int i = 0, N = 0;
    scanf("%d", &N);
    for (i = 0; i <= N - 1; i++)
        scanf("%lf", &a[i]);
    printf("\n");
    printf("Initial array:\n");

    for (i = 0; i <= N - 1; i++)
        printf("%.2lf ", a[i]);
    printf("\n");
    printf("\n");

    if (N > 1)
        BucketSort(a, N);

    if (N>1){
        printf("Final array:\n");
        for (i = 0; i <= N - 1; i++)
            printf("%.2lf ", a[i]);
    }

    return 0;
}

void BucketSort(double *a, int n){
    int  i = 0, j = 0, k = 0;
    double range = rangeof(a, n);             //range - интервал чисел в массиве
    double b[101 * 2][101] = {0};             //'b[i][j]' - двумерный массив в котором: 'i' - кол-во карманов, 'j' - кол-во элементов
    int c[101 * 2] = {0};                    //'c' - массив, элементы которого указывают на кол-во элементов в соответствующем кармане
    int ind = 0;

    for (i = 0 ; i < n; i++){         //Заполнение карманов
        ind = (int)((a[i] * n * 2) / (range));
        if ((int)((a[i] * n * 2) / (range)) == n * 2)
            ind = n * 2 - 1;
        b[ind][c[ind]] = a[i];
        c[ind]++;
    }

    for (i= 0; i <= n*2-1; i++){
        if (c[i] > 0){
            printf("Bucket:\n");
            for (j = 0; j <= c[i] - 1; j++)
                printf("%.2lf ", b[i][j]);
            printf("\n");
            InsSort(b[i], c[i]);                     //Сортировка каждого кармана
            printf("Sorted bucket:\n");
            for (j = 0; j <= c[i] - 1; j++)
                printf("%.2lf ", b[i][j]);

            printf("\n");
            printf("\n");
        }
    }


    for (i = 0; i<= n*2 - 1; i++){                 //Изменение исходого массива 'a'
        for (j = 0; j<=c[i] - 1; j++){
            a[k] = b[i][j];
            k++;
        }
    }
}

double rangeof(double a[], int n){
    double max = -1, min = 10000;
    int i = 0;
    for (i = 0; i <= n-1; i++){
        if (a[i] < min)
            min = a[i];
        if (a[i] >= max)
            max = a[i];
    }
    return max + min;
}

void InsSort(double a[], int n) {
    int i = 0, j = 0;
    double c =0;
    for (i=0; i < n; i++) {
        c = a[i];

        for (j = i-1; j>=0 && a[j] > c; j--)
            a[j+1] = a[j];

        a[j+1] = c;
    }
}