#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 80 //The size of the array

enum ExpType {OPT,OPERAND};

typedef struct _stackNode{
    char item;
    struct _stackNode *next;
}StackNode;

typedef struct _stack{
   int size;
   StackNode *head;
}Stack;

void push(Stack *sPtr, char item);
int pop(Stack *sPtr);
char peek(Stack s);
int isEmptyStack(Stack s);

typedef struct _listnode
{
    int  item;
    enum ExpType type;
	struct _listnode *next;
} ListNode;

typedef struct _linkedlist{
   int size;
   ListNode *head;
} LinkedList;

void insertNode(LinkedList *llPtr, int item,enum ExpType type);
int deleteNode(LinkedList *llPtr);
void removeAllNodes(LinkedList *llPtr);
int isEmptyLinkedList (LinkedList ll);


void in2PreLL(char* infix, LinkedList *inExpLL);

void printExpLL(LinkedList inExp);

int main()
{
    char infix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    LinkedList inExpLL;
    inExpLL.head = NULL;
    inExpLL.size = 0;

    in2PreLL(infix, &inExpLL);

    printExpLL(inExpLL);

    removeAllNodes(&inExpLL);
    return 0;
}

void in2PreLL(char* infix, LinkedList *inExpLL)
{
    if (infix[0]=='\0') return;

    //reverse string
    int length = strlen(infix), j = 0, i;
    char reverse[SIZE];
    strcpy(reverse, infix);
    char c;
    i = length;
    while (j < length){
        c = infix[i-1];
        reverse[j++] = c;
        i--;
    }

    Stack s;
    s.head = NULL;
    s.size = 0;
    int digit = 0, multiplier = 1;

    for (int i = 0; i < length; i++){
        if (reverse[i] == ' '){
            continue;
        }
        if (isdigit(reverse[i])){//this line work on numbers and increase multiplier
            digit = digit + ((reverse[i] - '0')*multiplier);
            multiplier*=10;
        }
        else{//if the current loop's char is not a digit do these...
            if (digit!=0){
                insertNode(inExpLL, digit, OPERAND);
                digit = 0, multiplier = 1;
            }
            if (reverse[i] == ')'){
                push(&s, reverse[i]);
            }
            else if (reverse[i] == '('){
                while (!isEmptyStack(s) && peek(s)!=')'){
                        insertNode(inExpLL, peek(s), OPT);
                        pop(&s); //Pop all operator until start of bracket
                }
                pop(&s);
            }
            else{
                if (reverse[i]=='+'||reverse[i]=='-'){
                    while(!isEmptyStack(s)&&(peek(s)=='/'||peek(s)=='*')){
                        insertNode(inExpLL, peek(s), OPT);
                        pop(&s);
                    }
                }
                push(&s, reverse[i]);
            }
        }
    }
    if(digit!=0){
		insertNode(inExpLL, digit, OPERAND);
    }
    while(!isEmptyStack(s)){ //Check if there is still OPT
        insertNode(inExpLL, peek(s), OPT); //If there is, pop and append everything
        pop(&s);
    }
}

void printExpLL(LinkedList inExpLL)
{
    ListNode* temp = NULL;
    temp = inExpLL.head;
    while(temp!= NULL){
        if(temp->type == OPERAND)
            printf(" %d ",temp->item);
        else
            printf(" %c ",(char)(temp->item));
        temp = temp->next;
    }
    printf("\n");
}

void insertNode(LinkedList *LLPtr, int item, enum ExpType type) {
    ListNode *newNode;
    newNode = malloc(sizeof(ListNode));
    if(newNode==NULL) exit(0);

    newNode->item = item;
    newNode->type = type;
    newNode->next = LLPtr->head;
    LLPtr->head=newNode;
    LLPtr->size++;
}

int deleteNode(LinkedList *LLPtr) {
    if(LLPtr==NULL || LLPtr->size==0){
        return 0;
    }
    else{
       ListNode *temp = LLPtr->head;
       LLPtr->head = LLPtr->head->next;

       free(temp);
       LLPtr->size--;
       return 1;
    }
}

int isEmptyLinkedList (LinkedList ll) {
    if(ll.size==0) return 1;
    else return 0;
}

void removeAllNodes(LinkedList *LLPtr)
{
	while(deleteNode(LLPtr));
}

void push(Stack *sPtr, char item){
    StackNode *newNode;
    newNode = malloc(sizeof(StackNode));
    newNode->item = item;
    newNode->next = sPtr->head;
    sPtr->head = newNode;
    sPtr->size++;
}

int pop(Stack *sPtr){
    if(sPtr == NULL || sPtr->head == NULL){
        return 0;
    }
    else{
       StackNode *temp = sPtr->head;
       sPtr->head = sPtr->head->next;
       free(temp);
       sPtr->size--;
       return 1;
    }
}

char peek(Stack s){
    return s.head->item;
}

int isEmptyStack(Stack s){
     if(s.size == 0) return 1;
     else return 0;
}

