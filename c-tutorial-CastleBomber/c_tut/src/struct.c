/**
 * [COMP 232] Task 9: Employee Struct
 * Author: CastleBomber
 * Date: February 7, 2021
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <person.h>

#define MAX_NUMBER_OF_EMPLOYEES 128

int main(void) {
    int numEmployees = 0;
    PERSON *employees[MAX_NUMBER_OF_EMPLOYEES];

    printf("How many Employees?\n");
    scanf(" %d", &numEmployees);

    for(int index = 0; index < numEmployees; index++){
        employees[index] = createPerson();
    }

    displayPeople(employees, numEmployees);

    return 0;
}
