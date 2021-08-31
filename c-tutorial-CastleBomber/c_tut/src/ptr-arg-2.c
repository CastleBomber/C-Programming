#include <stdio.h>

void swapIntegers(int *, int *);
void swapStrings(char **str1, char **str2);

int main(void) {
    char *string1 = "Hello";
    char *string2 = "Goodbye";

    swapStrings(&string1, &string2);
    printf("After the swap: string1 = %s and string2 = %s\n", string1, string2);

    return 0;
}

void swapStrings(char **str1_ptr, char **str2_ptr){
    char *tmp;
    tmp = *str1_ptr;
    *str1_ptr = *str2_ptr;
    *str2_ptr = tmp;
}

void swapIntegers(int *n1, int *n2) { /* passed and returned by using values of pointers */
    int temp;

    temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}
