#include <stdio.h>
/*
 * Отсортированный массив, делим пополам сравниваем средний элемент с target, рекурсивно делим нужную часть, пока target != a[mid]
 * Выполнить итерационно
*/

int main() {
    int a[100] = {0}, N = 0, i = 0, targ = 0, mid = 0, left = 0, right = 0;
    scanf("%d%d", &N, &targ);
    for (i = 0; i < N; i++)
        scanf("%d", &a[i]);

    printf("Binary search:\n");
    right = N - 1;
    if (N > 1){
        do{
            mid = (right + left) / 2;
            if (a[mid] > targ){
                right = mid - 1;
            }
            else if (a[mid] < targ){
                left = mid + 1;
            }
            printf("%d ", a[mid]);
        }while((a[mid] != targ) && !((left > right) || (right < 0)));
    }
    else
        printf("%d ", a[0]);
    printf("\n");

   return 0;
}