#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define s 9
void BFS (int a[][s], int start);
typedef struct Queue {
    int n;
    struct Queue* next;
}Queue;
Queue* add2Queue(int n, Queue* phead){
    Queue* pnew = (Queue*)malloc(sizeof(Queue));
    Queue *p = phead;
    while(p && p->next){
        p=p->next;
    }
    pnew->next = NULL;
    pnew->n = n;

    if(p)
        p->next = pnew;
    if(phead)
        return phead;
    return pnew;
}
Queue* deleteFromQueue(Queue *phead){
    Queue *p = NULL;
    if(phead)
        p=phead->next;
    free(phead);
    return p;
}
int QEmpty (Queue* head){
    if (head)
        return 1;
    else
        return 0;
}

int main() {
    int a[s][s] = {0};
    int i = 0, j = 0, N = s, start = 0;
    srand(time(0));
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            //a[i][j] = rand() % 2;
            //a[j][i] = a[i][j];
            scanf("%d", &a[i][j]);
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d  ", a[i][j]);
        }
        printf("\n");
    }
    scanf("%d", &start);

    BFS(a, start);
    return 0;
}
void BFS (int a[][s], int start ){
    Queue *head = NULL;
    int mark[s] = {0}, b = 0, i =0;
    int dest[s] = {0};
    head = add2Queue(start, head);
    mark[start] = 1;

    while (QEmpty(head)){
        printf("mark:\n");
        for (i = 0; i < s; i++)
            printf("%d ", mark[i]);
        printf("\ndest:\n");
        for (i = 0; i < s; i++)
            printf("%d ", dest[i]);
        printf("\n");
        b = head->n; printf("b:%d\n", b);
        head = deleteFromQueue(head);
        for (i = 0; i < s; i++){
            if ((mark[i] == 0) && (a[b][i] != 0)){
                dest[i] = dest[b] + 1;
                mark[i] = 1;
                head = add2Queue(i, head);
            }
        }
    }

    for (i = 0; i < s; i++)
        printf("%d ", dest[i]);
}