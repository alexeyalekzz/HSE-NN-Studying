#include <stdio.h>
#include <stdlib.h>

/*
 * есть алфавит из трех видов строк " { [ ( " , на входе последовательность скобок, если все скобки закрыты, вывести "CORRECT" или "INCORRECT"
 * Решается через стек (стек на основе массива?)
 * Открывающиеся скобки в стек, закрывающиеся на сравнение со стеком (?)
*/

typedef struct {
    char value;
    struct Node* pnext;
}Node;

Node *push(char v, Node * phead){
    Node *p = (Node*) malloc(sizeof(Node));
    p->value = v;
    p->pnext = (struct Node *) phead;
    return p;
}

Node *pop(Node* phead){
    Node *p = (Node *) phead->pnext;
    free(phead);
    return p;
}

int IsEmpty(Node *phead)
{
    if(phead)
        return 0;
    return 1;
}

int main() {
    Node *head = 0;
    char b = 0;
    int N = 0;
    scanf("%d ", &N);

    if ((N != 0) && (N % 2 != 1)){
        for (int i = 0; i < N; i++) {
            scanf("%c", &b);
            if ((b == '(') || (b == '{') || (b == '['))
                head = push(b, head);
            else if ((!IsEmpty(head)) && ((head->value + 1 == b) || (head->value + 2 == b)))
                head = pop(head);
            else{
                printf("INCORRECT ");
                return 0;
            }
        }
    }
    else{
        printf("INCORRECT");
        return 0;
    }

    if (IsEmpty(head))
        printf("CORRECT ");
    else
        printf("INCORRECT");

    return 0;
}




