#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define point(x) ((int)((float) x / 2+ 0.5))

void MergeSort(int *a, int left, int right);
void Merge (int *a, int left, int mid, int right);

int main() {
    int  i = 0, left = 0, N = 0, a[101];

    scanf("%d", &N);
    for (i = 0; i < N; i++)
        scanf("%d", &a[i]);
    printf("\n");
    printf("Initial array:\n");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n");
    printf("\n");

    if (N > 1)
        MergeSort(a, left, N-1);

    return 0;
}
void MergeSort(int *a, int left, int right){   //Функция для разделения массива пополам
    int mid = 0;
    if (right > left + 1){
        mid = point((left + right));
        MergeSort(a, left, mid-1);
        MergeSort(a, mid, right);
        Merge(a, left, mid, right);
    }
    else if (right == left + 1){
        mid = point((left + right));
        Merge(a, left, mid, right);
    }
}
void Merge (int *a, int left, int mid, int right){  //функция для слияния двух частей массива
    int pos1 = left, pos2 = mid, pos3 = pos1, i = 0; //pos1 - индекс 1-ого элемента 1-ого массива; pos2 - индекс 1-ого элемента 2-ого массива; pos3 - начальная позиция для записи в массив "b"
    int b[101] = {0};

    printf("Left part: ");
    for(i = pos1; i < pos2; i++)
        printf("%d ", a[i]);
    printf("\n");
    printf("Right part: ");
    for(i = pos2; i <= right; i++)
        printf("%d ", a[i]);
    printf("\n");

    while ((pos1 < mid) && (pos2 <= right)){     //Пока в одной из частей не закончатся элементы заполняем массив 'b'
        if (a[pos1] < a[pos2]){
            b[pos3] = a[pos1];
            pos1++;
            pos3++;
        }
        else {
            b[pos3] = a[pos2];
            pos2++;
            pos3++;
        }
    }
    while (pos1 < mid){                          //Заканчиваем заполнять массив 'b'
        b[pos3] = a[pos1];
        pos1++;
        pos3++;
    }
    while (pos2 <= right){
        b[pos3] = a[pos2];
        pos2++;
        pos3++;
    }

    printf("Merged parts: ");
    for (i = left; i<=right; i++)
        printf("%d ",b[i]);
    printf("\n");
    printf("\n");

    for(pos1 = left; pos1 <= right; pos1++){      //Изменение исходного массива 'a'
        a[pos1] = b[pos1];
    }
}