#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define BUFFER_SIZE 128

bool isInteger(double val)
{
    int truncated = (int)val;
    return (val == truncated);
}

int main(void) {
    double val = 5; // 5.5
    printf("%d\n", isInteger(val));

    return 0;
}

