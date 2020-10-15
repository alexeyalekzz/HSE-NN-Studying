#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int priority(char a, char b);
typedef struct {
    int value;
    struct Node* pnext;
}Node;

Node *push(int v, Node * phead){
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

int IsEmpty(Node *phead){
    if(phead)
        return 0;
    return 1;
}

int main() {
    int  output[100] = {0}, i = 0, k = 0, t1 = 0, l = 0, c= 0, a = 0;
    char b[100] = {0};
    Node *head = NULL;
    fgets(b, 100, stdin);
    printf("Expression:\n");
    printf("%s\n", b);
    while((b[i] != '\n') && (b[i] != '\0')){
        if ((b[i] >= '0') && (b[i] <= '9')){
            while((b[i] >= '0') && (b[i] <= '9')){
                i++;
                l++;
            }
            while(l > 0){
                output[t1] += ((int)b[i - l] - 48) * (int)pow(10, l - 1);
                l--;
            }
            t1++;
        }
        if (b[i] == '(')
            head = push(b[i], head);
        else if (b[i] == ')'){
            while (head->value != '('){
                output[t1] = head->value;
                head = pop(head);
                t1++;
            }
            if (head->value == '(')
                head = pop(head);
        }
        else if ((b[i] == '*') || (b[i] == '+') || (b[i] == '-') || (b[i] == '/')){
            while ( !(IsEmpty(head)) && priority((char)(head->value), b[i])){
                output[t1] = head->value;
                head = pop(head);
                t1++;
            }
            head = push(b[i], head);
        }
        i++;
    }
    while (!IsEmpty(head)){
        output[t1] = head->value;
        head = pop(head);
        t1++;
    }
    printf("Reverse Polish Notation:\n");
    for (k = 0; k < t1; k++){
        if ((output[k] != '+') && (output[k] != '-') && (output[k] != '*') && (output[k] != '/'))
            printf("%d ", output[k]);
        else
            printf("%c ", output[k]);
    }
    for (k = 0; k < t1; k++){
        if ((output[k] != '+') && (output[k] != '-') && (output[k] != '*') && (output[k] != '/')){
            head = push(output[k], head);
        }
        else{
            a = head->value;
            head = pop(head);
            c = head->value;
            head = pop(head);
            if (output[k] == '+')
                head = push(a + c, head);
            else if (output[k] == '-')
                head = push(c - a, head);
            else if (output[k] == '*')
                head = push(a * c, head);
            else
                head = push(c / a, head);
        }
    }
    printf("\nResult:\n%d", head->value);
    return 0;
}
int priority(char a, char b){
    if (a == '(')
        return 0;
    if (((a == '+') || (a == '-')) && ((b == '*') || (b == '/')))
        return 0;
    else
        return 1;
}