#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_OF_CELLS 128

int main(void) {
    int nDoubles = 0;
    int sum = 0;

    puts("Provide the desired number of doubles: ");
    scanf("%d", &nDoubles);

    if(nDoubles < 1){
        printf("\nCannot accept negative cardinality; set to 1 instead.\n");
    } else if (nDoubles > MAX_NUM_OF_CELLS){
        printf("\nResponse too large; set to %d instead. \n", MAX_NUM_OF_CELLS);
    }

    double doubles[nDoubles];

    puts("Provide the doubles?");
    for(int index = 0; index < nDoubles; index++){
        scanf(" %lf", &doubles[index]);
    }

    for(int index = 0; index < nDoubles; index++){
        sum += doubles[index];
        printf("%d ", doubles[index]);
    }

    printf("\nSum: %lf\n", sum);
    printf("\nAverage: %.1lf", sum/nDoubles);


    return 0;
}
