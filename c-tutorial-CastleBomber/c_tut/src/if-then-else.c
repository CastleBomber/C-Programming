#include <stdio.h>
#include <ctype.h>

int main(void) {
    char level = '\0';
    printf("How full is the tank (e = empty, h = half, f = full): ");
    level = tolower(getchar());
    //scanf("%c", &level);

    switch(level){
        case 'f':
            puts("Good driver, full tank!\n");
            break;
        case 'h':
            puts("Tank half full!\n");
            break;
        case 'e':
            puts("Low on gas, almost empty!\n");
            break;
        default:
            puts("Not valid\n");
    }

    return 0;
}
