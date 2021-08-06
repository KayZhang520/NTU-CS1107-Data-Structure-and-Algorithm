#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 200 //The number digit limitation

typedef struct _btnode{
	int item;
	struct _btnode *left;
	struct _btnode *right;
} BTNode;   // You should not change the definition of BTNode

typedef struct _node{
     BTNode* item;
     struct _node *next;
}StackNode;

typedef struct _stack{
   int size;
   StackNode *head;
}Stack;

void deleteTree(BTNode **root);

void createExpTree(BTNode** root,char* prefix);
void printTree(BTNode *node);
void printTreePostfix(BTNode *node);
double computeTree(BTNode *node);

void push(Stack *sPtr,  BTNode* item);
int pop(Stack *sPtr);
BTNode *peek(Stack s);
int isEmptyStack(Stack s);

int main()
{
    char prefix[SIZE];
    BTNode* root=NULL;

    printf("Enter an prefix expression:\n");
    gets(prefix);

    createExpTree(&root, prefix);

    printf("The Infix Expression:\n");
    printTree(root);
    printf("\n");
    printf("The Postfix (Reverse Polish) Expression:\n");
    printTreePostfix(root);
    printf("\n");
    printf("Answer ");
    printf("%.2f\n", computeTree(root));
    deleteTree(&root);
    return 0;
}

void createExpTree(BTNode** root,char* prefix)
{
    if (prefix[0] == '\0') return;
    Stack s;
    s.head = NULL;
    s.size = 0;

    int index = strlen(prefix), digit = 0, multiplier = 1;

    while(index!=-1){
        if (prefix[index]!=' '&&prefix[index]!='\0'){
            BTNode *newnode = malloc(sizeof(BTNode));
            newnode->left = NULL;
            newnode->right = NULL;

            if (isdigit(prefix[index])){
                while (isdigit(prefix[index])){
                    digit = digit + ((prefix[index]-'0')*multiplier);
                    multiplier*=10;
                    index--;
                }
                newnode->item = digit;
                digit = 0; multiplier = 1;
                push(&s, newnode); //pushing digits into a stack
                continue;
            }
            else{//opt
                newnode->item = prefix[index];
                newnode->left = peek(s);
                pop(&s);
                newnode->right = peek(s);
                pop(&s);
                push(&s, newnode);
            }
        }
        index--;
    }
    *root = peek(s);
}

void printTree(BTNode *node){
    if (node == NULL){
        return;
    }

    printTree(node->left);
    if(node->left==NULL && node->right==NULL){
        printf("%d ", node->item);
    }
    else{
        printf("%c ", node->item);
    }
    printTree(node->right);
}


void printTreePostfix(BTNode *node){
    if (node == NULL){ //If initial parameter was NULL aka empty tree, return and end func
        return;
    }

    printTreePostfix(node->left); //Traverse down left first, then print node
    printTreePostfix(node->right);


    if (node->left!=NULL || node->right!=NULL){
        printf("%c ", node->item);
    }
    else{
        printf("%d ", node->item);
    }
}

double computeTree(BTNode *node){
    if (node->left==NULL && node->right==NULL){
        return node->item;
    }

    double left = computeTree(node->left);
    double right = computeTree(node->right);

    if (node->left!=NULL && node->right!=NULL){
        char operator = node->item;
        if (operator == '+'){
            return left+right;
        }
        else if (operator == '-'){
            return left-right;
        }
        else if (operator == '*'){
            return left*right;
        }
        else if (operator == '/'){
            return left/right;
        }
    }
}

void push(Stack *sPtr, BTNode *item){
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

BTNode *peek(Stack s){
    return s.head->item;
}

int isEmptyStack(Stack s){
     if(s.size == 0) return 1;
     else return 0;
}

void deleteTree(BTNode **root){
    BTNode* temp;
    if(*root !=NULL)
    {
        temp = (*root)->right;
        deleteTree(&((*root)->left));
        free(*root);
        *root = NULL;
        deleteTree(&temp);
    }
}
