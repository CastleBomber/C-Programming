/**
 * [COMP 232] Task 9: Employee Struct
 * Author: CastleBomber
 * Date: February 7, 2021
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "person.h"
#define BUFFER_SIZE 128

/*
    Enter information for the next employee.
    Name : Bob Ross
    Age : 54
    Height : 1.2
    Birthday ( MM / DD / YYYY ) : 11/22/3333

    read data from stdin to populate a PERSON struct and return it
 */
PERSON *createPerson(){
    char buffer[BUFFER_SIZE];
    PERSON *person = calloc(1, sizeof(PERSON));
    printf("Enter information for the next employee.\n");

    printf("Name : ");
    scanf(" %[^\n]c", buffer); // why ^\n here? possibly string input
    person->name = malloc(sizeof(char) * (strlen(buffer)+1));
    strcpy(person->name, buffer);

    printf("Age :");
    scanf(" %d", &person->age);
    printf("Height :");
    scanf(" %f", &person->height);
    printf("Birthday (MM / DD / YYYY): ");
    scanf(" %d / %d / %d",
          &person->bday.month,
          &person->bday.day,
          &person->bday.year);

    return person;
}

/*
 * print to stdout the data from the ref PERSON in a readable format
 */
void displayPerson(PERSON *person){
    printf("\n%s :", person->name);
    printf("\n\tAge : %d", person->age);
    printf("\n\tHeight : %1.1f", person->height);
    printf("\n\tBirthday : %d/%d/%d\n", person->bday.month,person->bday.day, person->bday.year);
}

/*
 * displayPerson on numberOfPeople PERSON* in people array
 */
void displayPeople(PERSON *people[], int numberOfPeople){
    printf("\nDisplaying Employees...\n");

    for(int e = 0; e < numberOfPeople; e++){
        displayPerson(people[e]);
    }

    for(int r = 0; r < numberOfPeople; r++){
        freePerson(people[r]);
    }
}


void freePerson(PERSON** person){
    free((*person)->name );
    free( *person);
    *person = NULL;
}
