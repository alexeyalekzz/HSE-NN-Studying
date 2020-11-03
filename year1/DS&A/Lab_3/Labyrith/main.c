#include <stdio.h>
#include <stdlib.h>
/*
10 9
F........
.........
.#######.
.#.....#.
.#.###.#.
.#.#S#.#.
.#.#...#.
.#.#####.
.#.......
.########
 */
void BFS (int a[][300], int start, int x, int y, int finish);
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
    int x = 0, y = 0, i = 0, j = 0, p = 0;
    char b[100][100] = {0};
    int a[300][300] = {0};
    int start = 0, finish = 0;
    scanf("%d%d", &x, &y);
    for (i = 0; i < x; i++){
        for (j = 0; j < y; j++){
            scanf("%c", &b[i][j]);
            if (b[i][j] == '\n')
                scanf("%c", &b[i][j]);
        }
    }
    printf("Initial labyrinth:\n");
    for (i = 0; i < x; i++){
        for (j = 0; j < y; j++){
            printf("%c", b[i][j]);
        }
        printf("\n");
    }
    for (i = 0; i < x; i++){
        for (j = 0; j < y; j++){
            if ((b[i][j] == 'S') || (b[i][j] == '.') || (b[i][j] == 'F')){
                if ((i - 1 >= 0) && ((b[i - 1][j] == 'S') || (b[i- 1][j] == '.') || (b[i - 1][j] == 'F')))
                    a[i + (y-1) * i + j][(i - 1)*(y) + j] = 1;
                if ((j - 1 >= 0) && ((b[i][j - 1] == 'S') || (b[i][j - 1] == '.') || (b[i][j - 1] == 'F')))
                    a[i + (y-1) * i + j][i + (y-1) * i + j - 1] = 1;
                if ((j + 1 < y) && ((b[i][j + 1] == 'S') || (b[i][j + 1] == '.') || (b[i][j + 1] == 'F')))
                    a[i + (y-1) * i + j][i + (y-1) * i + j + 1] = 1;
                if ((i + 1 < x) && ((b[i + 1][j] == 'S') || (b[i + 1][j] == '.') || (b[i + 1][j] == 'F')))
                    a[i + (y-1) * i + j][(i + 1)*(y) + j] = 1;
                if (b[i][j] == 'S')
                    start = (i)*(y)+j;
                else if (b[i][j] == 'F')
                    finish = (i)*(y)+j;
            }
        }
    }
    printf("Graph:\n");
    for (i = 0; i < x * y; i++){
        printf("%d - ", i);
        for (j = 0; j < x * y; j++){
            if (a[i][j] != 0){
                printf("%d ", j);
                p++;
            }
        }
        if (p == 0)
            printf("None");
        p = 0;
        printf("\n");
    }
    BFS(a, start, x, y, finish);
    return 0;
}
void BFS (int a[][300], int start, int x, int y, int finish){
    Queue *head = NULL;
    int mark[300] = {0}, b = 0, i =0, j = 0;
    int dest[300] = {0};
    head = add2Queue(start, head);
    mark[start] = 1;
    b = head->n;
    while (QEmpty(head)){
        b = head->n;
        if (b == finish)
            break;
        head = deleteFromQueue(head);
        for (i = 0; i < x*y; i++){
            if ((mark[i] == 0) && (a[b][i] != 0)){
                dest[i] = dest[b]+ 1;
                mark[i] = 1;
                head = add2Queue(i, head);
            }
        }
    }
    for(i = 0; i < x * y; i++){
        if ((dest[i] == 0) && (i != start))
            dest[i] = -1;
    }
    printf("BFS result is :\n");
    for (i = 0; i < x * y; i+=y){
        for (j = i; j < i + y; j++){
            printf("%d ", dest[j]);
        }
        printf("\n");
    }
}