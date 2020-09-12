#include <stdio.h>

/*
 * Массив отсортирован, есть некое pos = 1, мы будем сравнивать a[pos] и target (a[pos] == target ?) если нет, то pos*=2;
 * мы гарантируем, что target лежит в промежутке a[pos/2] < targ < a[pos];
 * запускаем binary search для границ (pos/2 , pos);
*/

int main() {
    int a[101] = {0};
    int i = 0, pos = 1, N = 0, targ = 0, mid = 0, right = 0, left = 0;
    scanf("%d%d", &N, &targ);
    for (i = 0; i < N; i++)
        scanf("%d", &a[i]);

    printf("Doubling search:\n");
    if (N > 1) {
        while ((a[pos] < targ) && (pos < N - 1))
            pos *= 2;
        if (pos > N - 1)
            right = N - 1;
        else
            right = pos - 1;
        left = pos / 2 + 1;
        if (a[pos] != targ) {
            do {
                mid = (right + left) / 2;
                if (a[mid] > targ) {
                    right = mid - 1;
                } else if (a[mid] < targ) {
                    left = mid + 1;
                }
                printf("%d ", a[mid]);
            }while ((a[mid] != targ) && !((left > right) || (right < 0)));
        }
        else
            printf("%d", a[pos]);
    }
    else
        printf("%d ", a[0]);
    return 0;
}