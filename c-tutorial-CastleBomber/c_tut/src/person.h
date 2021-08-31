/**
 * [COMP 232] Task 9: Employee Struct
 * Author: CastleBomber
 * Date: February 7, 2021
 */

#ifndef C_TUT_PERSON_H
#define C_TUT_PERSON_H

typedef struct birthday{
    int month;
    int day;
    int year;
} BIRTHDAY;

typedef struct person {
    char *name;
    int age;
    float height;
    BIRTHDAY bday;
} PERSON;

/**
 * previously addEmployee()
 * read data from stdin to populate a PERSON struct and return it
 * @return
 */
PERSON *createPerson();

/**
 * previously displayEmployee
 * print to stdout the data from the ref PERSON in a readable format
 *
 * @param person
 */
void displayPerson(PERSON *person);

/**
 *  prev displayAllEmployees
 *  displayPerson on numberOfPeople PERSON* in people array
 *
 * @param people
 * @param numberOfPeople
 */
void displayPeople(PERSON *people[], int numberOfPeople);

void freePerson(PERSON**);

#endif //C_TUT_PERSON_H
