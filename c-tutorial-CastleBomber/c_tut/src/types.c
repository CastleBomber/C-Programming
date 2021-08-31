#include <stdio.h>
#include <math.h>

int main(void) {
    int nStudents = 0; /* Initialization, required */
    int nFaculty = 0;
    double ratio = 0.0;

    printf("How many students does CSUCI have ?: ");
    scanf("%d", &nStudents);  /* Read input */
    printf("How many faculty does CSUCI have ?: ");
    scanf("%d", &nFaculty);
    ratio = (double)nStudents/nFaculty;
    printf("The ratio of students to faculty is: %.1f\n", ratio);

    return 0;
}
