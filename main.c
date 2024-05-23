#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#define MAX_SIZE 1000
#define MAX_STACK_SIZE 1000

typedef struct {
    char value;
    double dvalue;
    int weight;
} StackValue;

typedef struct {
    StackValue data[MAX_STACK_SIZE];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

void push(Stack *s, StackValue *value) {
    if (s->top >= MAX_STACK_SIZE - 1) {
        printf("Error: Stack overflow\n");
        exit(1);
    }
    s->data[++s->top].value = value->value;
    s->data[s->top].dvalue = value->dvalue;
    s->data[s->top].weight = value->weight;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

StackValue pop(Stack *s) {
    if (s->top == -1) {
        printf("Error: Stack underflow\n");
        exit(1);
    }
    return s->data[s->top--];
}

StackValue get(Stack *s) {
    if (s->top == -1) {
        printf("Error: Stack underflow\n");
        exit(1);
    }
    return s->data[s->top];
}

double calculate(char operation, double a, double b) {
    switch (operation) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b == 0){
                printf("Division by zero!");
                exit(1);
            }
            return a / b;
        case '^':
            return pow(a, b);
        default:
            printf("Incorrect operation!");
            exit(1);
    }
}

double evaluateExpression(char *input) {
    char *pointer = input;
    char postfix[MAX_SIZE] = {};
    int index = 0;
    bool isNumberExists = false;
    bool isAfterOperator = false;
    Stack* stack1 = (Stack *) malloc(sizeof(Stack));
    if(stack1 == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    initStack(stack1);
    while (*pointer != '\0') {
        if (isdigit(*pointer)) {
            if (isAfterOperator && postfix[index - 1] != ' ') {
                postfix[index++] = ' ';
                isAfterOperator = false;
            }
            postfix[index] = *pointer;
            index++;
            isNumberExists = true;
            isAfterOperator = false;
        } else if (*pointer == '+' || *pointer == '-' || *pointer == '*' || *pointer == '/' || *pointer == '^' ||
                   *pointer == '(' || *pointer == ')') {
            if (*pointer == '+' || *pointer == '-') {
                StackValue value;
                value.weight = 1;
                value.value = *pointer;
                if (!isEmpty(stack1)) {
                    //Проверяем можно ли положить оператор в стек
                    if (get(stack1).weight >= value.weight) {
                        //пока в стеке есть элемент больше либо равный по весу текущему, достаем и кладем в постфиксную запись
                        while (!isEmpty(stack1) && get(stack1).weight >= value.weight && get(stack1).value != '(') {
                            StackValue operator = pop(stack1);
                            postfix[index++] = operator.value;
                            postfix[index++] = ' ';

                        }
                    }
                }
                push(stack1, &value);
            } else if (*pointer == '*' || *pointer == '/') {
                StackValue value;
                value.weight = 2;
                value.value = *pointer;
                if (!isEmpty(stack1)) {
                    //Проверяем можно ли положить оператор в стек
                    if (get(stack1).weight >= value.weight) {
                        //пока в стеке есть элемент больше либо равный по весу текущему, достаем и кладем в постфиксную запись
                        while (!isEmpty(stack1) && get(stack1).weight >= value.weight && get(stack1).value != '(') {
                            StackValue operator = pop(stack1);
                            postfix[index++] = operator.value;
                            postfix[index++] = ' ';

                        }
                    }
                }
                push(stack1, &value);
            } else if (*pointer == '^') {
                StackValue value;
                value.weight = 3;
                value.value = *pointer;
                push(stack1, &value);
            } else if (*pointer == '(') {
                StackValue value;
                value.value = *pointer;
                push(stack1, &value);
            } else {
                if (!isEmpty(stack1)) {
                    while (get(stack1).value != '(') {
                        StackValue operator = pop(stack1);
                        postfix[index++] = ' ';
                        postfix[index++] = operator.value;
                    }
                    pop(stack1);
                } else {
                    printf("Extension error!");
                    exit(1);
                }
            }
            isAfterOperator = true;
            isNumberExists = false;
        } else if (isspace(*pointer)) {
            if (isNumberExists && postfix[index - 1] != ' ') {
                postfix[index] = *pointer;
                index++;
            }
            isNumberExists = false;
            isAfterOperator = false;
        } else {
            printf("Unknown symbol!");
            exit(1);
        }
        *pointer++;
    }
    if (postfix[index - 1] != ' ') {
        postfix[index++] = ' ';
    }
    while (!isEmpty(stack1)) {
        postfix[index++] = pop(stack1).value;
        postfix[index++] = ' ';
    }
    postfix[index - 1] = '\0';
    printf("Postfix: %s\n", postfix);


    initStack(stack1);

    char *ptr = postfix;
    double temp = 0;
    bool isStarted = false;

    while (*ptr != '\0') {
        if (isdigit(*ptr)) {
            temp = temp == 0 ? temp : temp * 10;
            temp += *ptr - '0';
            isStarted = true;
        } else if (*ptr == '+' || *ptr == '-' || *ptr == '*' || *ptr == '/' || *ptr == '^') {
            double b = pop(stack1).dvalue;
            double a = pop(stack1).dvalue;
            temp = calculate(*ptr, a, b);
        } else if (isspace(*ptr) && isStarted) {
            StackValue value;
            value.dvalue = (double) temp;
            push(stack1, &value);
            temp = 0;
        } else {
            if(isStarted){
                printf("Unknown symbol!");
                exit(1);
            }
        }
        ptr++;
    }
    if (!isEmpty(stack1)) {
        printf("Stack is not empty!");
        exit(1);
    }
    free(stack1);
    return temp;
}

int main() {
    char input[] = "1/0";
//    char input[] = "7 + 33 + 56";
//    char input[] = "7^5 + (123  12)";
//    char input[] = "7^5 + (123 -    12)";
//    char input[] = "(2^3)^2";

    double result = evaluateExpression(input);
    printf("Result: %f\n", result);
    return 0;
}


