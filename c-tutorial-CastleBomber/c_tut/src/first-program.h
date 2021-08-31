#ifndef C_TUT_FIRST_PROGRAM_H
#define C_TUT_FIRST_PROGRAM_H

typedef struct member {
    char *name;
    int age;
} MEMBER;

MEMBER *createMember();

void displayPerson(MEMBER *member);

void displayPeople(MEMBER *members[], int numMember);

void freePerson(MEMBER**);



#endif //C_TUT_FIRST_PROGRAM_H
