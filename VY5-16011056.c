#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


#define SIZE 50
#define INT_MAX = 9999999

typedef struct{

    int st[SIZE];
    int top;

}Stack;

bool var[26] = {false}; // Variable checker. For example if string: a = b + 4 then var[0] = true
int alphabet[26] = {0}; //It keeps the values of variables from a to z
int opCount = 0; // How many operations are there it the file?

//It returns precedence of an operator
int precOfOperator(char x){
    
    if(x == '(' || x == ')')
        return 0;
    else if(x == '+' || x == '-')
        return 1;
    else if(x == '*' || x == '/')
        return 2;
    else return -1;
}

//This function returns the last element in the stack by not popping
char peek(Stack *stack){
    return stack->st[stack->top -1];
}

//Initialize the stack
Stack *initStack(){
    Stack *stack = malloc(sizeof(Stack));
    stack->top = 0;
    return stack;
}

//Checks whether the stack is empty or not
bool isEmpty(Stack *stack){
    return (stack->top == 0);
}

//Checks whether the stack is full or not
bool isFull(Stack *stack){
    return (stack->top == SIZE);
}

//It pushes x to the stack
void push(Stack *stack, char x){
    if(!isFull(stack)){
        stack->st[stack->top] = x;
        stack->top++;
    }else{
        printf("Stack is Full\n");
        exit(1);
    }
}

//It pops the stack
char pop(Stack *stack){
    if(!isEmpty(stack)){
        stack->top--;
        return stack->st[stack->top];
    }else{
        printf("Stack is empty!\n");
        exit(1);
    }
}

//Checks if x is +-*/
bool isOperator(char x){
    return precOfOperator(x) > 0;
}

//It returns a char * that shows postfix array
char *currentPostfix(char *postfix){
    int i = 4, j = 0;
    char *tmp = malloc(50*sizeof(char));
    while(i < strlen(postfix))
        tmp[j++] = postfix[i++];
    return tmp;
}

//It returns a char * that shows the eleents in the stack
char *whatIsInTheStack(Stack *stack){
    char *tmp = malloc(50*sizeof(char));
    int i = 0;
    while(i < stack->top)
        tmp[i] = stack->st[i++];
    tmp[i] = '\0';
    return tmp;
}

//Converts an infix expression to postfix
char *infixToPostfix(char *infix){
    printf("%s",infix);
    int i = 4, j = 4;
    char *postfix = malloc(50*sizeof(char));
    Stack *stack = initStack();
    char popped;
    postfix[0] = infix[0];
    postfix[1] = infix[1];
    postfix[2] = infix[2];
    postfix[3] = infix[3];
    
    while(infix[i] != '\0'){
    
        //Alphanumeric
        if(isalnum(infix[i]))
            postfix[j++] = infix[i];
            
        //If infix[i] is blank and last element of postfix arr is not blank, add a blank
        else if(isblank(infix[i]) && postfix[j-1] != ' '){
            postfix[j++] = ' ';
        }
        
        else if(infix[i] == '(')
            push(stack, infix[i]);
            
        else if(infix[i] == ')'){
            popped = pop(stack);
            while(!isEmpty(stack) && popped != '('){
                postfix[j++] = popped;
                postfix[j++] = ' ';
                popped = pop(stack);
            }
        }
        // if infix[i] is an operator except '(' and ')'
        else if(isOperator(infix[i])){
            while(!isEmpty(stack) && precOfOperator(infix[i]) <= precOfOperator(peek(stack)))
                postfix[j++] = pop(stack);
            push(stack, infix[i]);
        }
        if(isOperator(infix[i]) || isalnum(infix[i]))
            printf("Postfix: %s    Stack: %s\n", currentPostfix(postfix), whatIsInTheStack(stack));
        i++;
    }
    while(!isEmpty(stack)){
        popped = pop(stack);
        postfix[j++] = popped;
        printf("Postfix: %s    Stack: %s\n", currentPostfix(postfix), whatIsInTheStack(stack));
        postfix[j++] = ' ';
    }
    printf("Temporary Output: %s\n\n", currentPostfix(postfix));
    postfix[j] = '\0';
    return postfix;
}

//Reads a file
char **readFile(char *fileName){
    char buff[100];
    int i = 0;
    char **infix;
    infix = malloc(50*sizeof(char*));
    for(i = 0; i < 50; i++)
        infix[i] = malloc(50*sizeof(char));
   
    FILE *f = fopen(fileName, "r");
    
    if(!f){
        printf("Error opening file!\n");
        exit(1);
    }
    
    i = -1;
    
    while(fgets(infix[++i], 100, f));
    opCount = i;
    return infix;
}

//printing the stack which keeps the integer values in it
void printResultStack(Stack *stack){
    int i = 0;
    int tmp[50];
    printf("Stack: ");
    while(i < stack->top){
        tmp[i] = stack->st[i++];
        printf("%d ",tmp[i]);
    }
    printf("\n");
}

//It calculates the result for a postfix expression
void result(char *postfix){
    int i = 4;
    int tmp = 0;
    printf("For %s and ",currentPostfix(postfix));
    for(i = 0; i < 26; i++){
        if(var[i])
            printf("%c <- %d    ",i + 'a', alphabet[i]);
    }
    printf("\n");
    int popped1, popped2;
    Stack *stack = initStack();
    
    //Postfix keeps whole the string except ; For example a = b c 2 - * So we need token to begin with b. 
    char *token = strtok(postfix, " ");//This makes token[0] = 'a'
    token = strtok(NULL, " ");//This makes token[0] = '='
    token = strtok(NULL, " ");//This makes token[0] = 'b'
    printResultStack(stack);
    while(token){
        if(isalpha(token[0])){
            push(stack, alphabet[token[0] - 'a']);
            printResultStack(stack);
        }else if(isOperator(token[0])){
            popped1 = pop(stack);
            popped2 = pop(stack);
            printResultStack(stack);
            switch(token[0]){
                case '+': push(stack, popped1 + popped2); printResultStack(stack); break;
                case '-': push(stack, popped2 - popped1); printResultStack(stack); break;
                case '*': push(stack, popped1 * popped2); printResultStack(stack); break;
                case '/': push(stack, popped2 / popped1); printResultStack(stack); break;
                default: break;
            }
        }else{
            push(stack, atoi(token));
            printResultStack(stack);
        }
        token = strtok(NULL, " ");
    }
    alphabet[postfix[0]-'a'] = pop(stack);
    var[postfix[0]-'a'] = true;
    printf("\n");
}

int main(){

    char **infix, **postfix;
    int i;
    postfix = malloc(opCount*sizeof(char*));
    for(i = 0; i < opCount; i++)
        postfix[i] = malloc(50*sizeof(char));
        
    infix = readFile("input.txt");
    for(i = 0; i < opCount; i++){
        postfix[i] = infixToPostfix(infix[i]);
    }
    
    for(i = 0; i < opCount; i++)
        result(postfix[i]);
    printf("\nResults\n");
    for(i = 0; i < 26; i++)
        if(var[i])
            printf("%c = %d\n",('a'+i), alphabet[i]);
    
    return 0;
}
