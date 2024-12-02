#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct _stack;
typedef struct _stack* Position;
typedef struct _stack {
    int El;
    Position Next;
}stack;

int Push(Position head, int element);
int Pop(Position head);
int Evaluate(Position head);
int Sum(Position head);
int Difference(Position head);
int Product(Position head);
int Quotient(Position head);

int main() {

    stack Head = { .El = 0, .Next = NULL };

    Evaluate(&Head);

    if (Head.Next != NULL) {                                 
        printf("The result is %d\n", Head.Next->El);
    }
    else {
        printf("Stack is empty.\n");
    }

    return 0;
}

int Evaluate(Position head) {
    FILE* fp = NULL;
    fp = fopen("postfix.txt", "r");  
    char operation[15];

    if (fp == NULL) { 
        printf("Failed to open the file!\n");
        return 1;
    }

    while (fscanf(fp, "%s", operation) != EOF) {
        int num = 0;
        int read = sscanf(operation, "%d", &num);

        if (read == 1) { 
            Push(head, num);
        }
        else {
            if (strcmp(operation, "+") == 0)
                Sum(head);
            else if (strcmp(operation, "-") == 0)
                Difference(head);
            else if (strcmp(operation, "*") == 0)
                Product(head);
            else if (strcmp(operation, "/") == 0)
                Quotient(head);
        }
    }

    fclose(fp);
    return 0;
}

int Push(Position head, int element) {

    Position newElement = NULL;
    newElement = (Position)malloc(sizeof(stack));

    if (newElement == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    newElement->El = element;
    newElement->Next = head->Next;
    head->Next = newElement;

    return 0;
}

int Pop(Position head) {

    Position temp = NULL;
    int element;

    if (head->Next == NULL) {
        printf("The stack is empty!\n"); 
        return -1;
    }

    temp = head->Next;
    element = temp->El;
    head->Next = temp->Next;
    free(temp);

    return element;
}

int Sum(Position head) {
    int firstNum = Pop(head);
    int secondNum = Pop(head);

    Push(head, secondNum + firstNum);
    return 0;
}

int Difference(Position head) {
    int firstNum = Pop(head);
    int secondNum = Pop(head);

    Push(head, secondNum - firstNum);
    return 0;
}

int Product(Position head) {
    int firstNum = Pop(head);
    int secondNum = Pop(head);

    Push(head, secondNum * firstNum);
    return 0;
}

int Quotient(Position head) {
    int firstNum = Pop(head);
    int secondNum = Pop(head);

    if (firstNum == 0) { 
        printf("Cannot divide by zero!\n");
        return 1;
    }

    Push(head, secondNum / firstNum);
    return 0;
}
