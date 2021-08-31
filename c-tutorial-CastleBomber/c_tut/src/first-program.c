#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <first-program.h>

#define MAX_NUMBER 2
#define BUFFER_SIZE 128


MEMBER *createMember(){
    char buffer[BUFFER_SIZE];
    MEMBER *member = calloc(1, sizeof(MEMBER));
    printf("Enter information for the member.\n");

    printf("Name : ");
    scanf(" %[^\n]c", buffer); // why ^\n here? possibly string input
    member->name = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(member->name, buffer);

    printf("Age :");
    scanf(" %d", &member->age);

    return member;
}

void displayPerson(MEMBER *member){
    printf("\n%s :", member->name);
    printf("\n\tAge : %d", member->age);
}

void displayMembers(MEMBER *member[], int numMembers){
    printf("\nDisplaying Employees...\n");

    for(int e = 0; e < numMembers; e++){
        displayPerson(member[e]);
    }

    for(int r = 0; r < numMembers; r++){
        freePerson(member[r]);
    }
}

void freeMember(MEMBER** member){
    free((*member)->name );
    free( *member);
    *member = NULL;
}

int main(void) {
    int numMembers = 2;
    MEMBER *members[MAX_NUMBER];

    for(int index = 0; index < numMembers; index++){
        members[index] = createMember();
    }

    return 0;
}
