/**
 * [COMP 232] Task 5
 * Author: CastleBomber
 * Date: February 6, 2021
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 128
#define STR_IMPL_(x) #x
#define STR(x) STR_IMPL_(x)
#define MAX_NUMBER_OF_INPUTS 128
#define SENTINTEL "END"

int main(void) {
    char buffer[BUFFER_SIZE+1];
    //char **words = malloc(MAX_NUMBER_OF_INPUTS * sizeof(char *));
    char *words[MAX_NUMBER_OF_INPUTS];
    int numberOfWords = 0;

    printf("Enter words (enter\"END\" to stop):\n");
    scanf("%"STR(BUFFER_SIZE)"s", buffer); // scanf puts in the '\0'

    while(strcmp(buffer,SENTINTEL) != 0 && numberOfWords < MAX_NUMBER_OF_INPUTS){
        words[numberOfWords] = malloc((strlen(buffer)+1) *sizeof(char)); // make room for buffer
        strcpy(words[numberOfWords], buffer);
        scanf("%"STR(BUFFER_SIZE)"s", buffer);
        numberOfWords++;
    }

    printf("\nThe following %d words  have been read:", numberOfWords);
    for(int i = 0; i < numberOfWords; ++i){
        printf("\n%s", words[i]);
        free(words[i]);
    }

    return 0;
}