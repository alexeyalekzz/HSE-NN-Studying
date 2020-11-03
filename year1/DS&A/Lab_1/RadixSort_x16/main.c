#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int ddd(long long a);
void RadixSort(long long int a[], int N);
int digits(long long a[], int N);
int main()
{
    long long a[100] = {0};
    int N =0;
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
        scanf("%llX", &a[i]);
    RadixSort(a, N);
    return 0;
}
void RadixSort(long long int a[], int N){
    int c[16] = {0};
    int d[100] = {0};
    long long int b[101] = {0};
    int i = 0, p = 0, g = 0, t = 1, k =0;


    k = digits(a, N);

    for ( g = 1; g <= k; g++){

        for (i = 0; i < N; i++)
            d[i] = ddd(a[i] / t);

        printf("Sorting by %d digit:\n", k - g);


        for (i = 0; i <= N - 1; i++)            //Подсчет цифр в элементах массива
                c[(a[i] / t) % 16] += 1;

        for (i = 0; i < 16; i++){       //Подсчет кол-ва чисел исходного массива, меньших i (цифры числа).
            if (c[i] != 0){
                p += c[i];
                c[i] = p - c[i];
            }
        }

        for (i = 0; i <= N - 1; i++) {
            if (d[i] != 1 && a[i] != 0) {
                b[c[(a[i] / t) % 16]] = a[i];   //Заполнение массива 'b'
                c[(a[i] / t) % 16]++;
            }
        }

        t *= 16;                         //Переход к след. разряду
        p = 0;

        for (i = 0; i <= N - 1; i++){  //Изменение исходного массива
            a[i] = b[i];
        }

        for (i = 0; i < 16; i++){  //Обнуление массива 'c'
            c[i] = 0;
        }

        for (i = 0; i < 16; i++){  //Обнуление массива 'c'
            d[i] = 0;
        }

        for(i = 0; i < N; i++)
            printf("%llX\n", a[i]);
        printf("\n");
    }
}
int digits(long long a[], int N){
    int c = 0, maxc=0;
    for (int i = 0; i < N; i++){
        long long d = a[i];
        while (d > 0){
            c++;
            d /= 16;
        }
        if (c > maxc)
            maxc = c;
        c = 0;
    }
    return maxc;
}
int ddd(long long a){
    int c=0;
    while (a > 0){
        c++;
        a/=16;
    }

        return c;
}