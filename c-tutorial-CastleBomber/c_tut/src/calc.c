/**
 * [COMP 232] Task 8: Calculator
 * Author: CastleBomber
 * Date: February 4, 2021
 */

#include <stdlib.h>
#include <stdio.h>

double calc(double (*)(double, double), double, double);
double add(double, double);
double subtract(double, double);
double multiply(double, double);
double divide(double, double);

int main(void) {
    double num1;
    char operand;
    double num2;
    char buffer[256];
    double answer;

    do{
        printf("calc > ");
        fgets(buffer, 256, stdin);
        if(sscanf(buffer, "%lf %1[-+/*] %lf", &num1, &operand, &num2) != 3){
            printf("Error: Invalid input.\n");
        }

        switch (operand) {
            case '+':
                answer = calc(add, num1, num2);
                printf("answer is: %.2f\n", answer);
                break;
            case '-':
                answer = calc(subtract, num1, num2);
                printf("answer is: %.2f\n", answer);
                break;
            case '*':
                answer = calc(multiply, num1, num2);
                printf("answer is: %.2f\n", answer);
                break;
            case '/':
                 answer = calc(divide, num1, num2);
                 printf("answer is: %.2f\n", answer);
                break;
            default:
                printf("Error: Incorrect Usage\n");
                break;
        }
    } while(1);

    return 0;
}

/* call function *f with a, b */
double calc(double (*f)(double, double), double a, double b){
    double myAnswer = 0.0;
    myAnswer = (*f)(a, b);
    return myAnswer;
}

double add(double a, double b){
    double sum = 0.0;
    sum = a + b;
    return sum;
}

double subtract(double a, double b){
    double difference = 0.0;
    difference = a - b;
    return difference;
}

double multiply(double a, double b){
    double product = 0.0;
    product = a*b;
    return product;
}

double divide(double a, double b){
    double quotient = 0.0;
    quotient = a/b;
    return quotient;
}