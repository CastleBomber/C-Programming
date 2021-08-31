#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <first-program.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSTRING 50

int main(void) {
    FILE *fileReadPtr=NULL;
    FILE *fileWritePtr;
    char fileName[MAXSTRING];
    char c; // stores input to be used for output

    printf("Name of new file: \n");
    scanf(" %49[^ \t.\n]%*c", fileName);
    //strcpy(fileName,"tmp1.txt");
    fileWritePtr = fopen(fileName, "w+");

    fileReadPtr = fopen("data.txt", "r");
    if(fileReadPtr == NULL){
        perror("Error opening file");
        return (-1);
    }

    do{
        c = fgetc(fileReadPtr);

        if(feof(fileReadPtr)){
            break;
        }
        fputc(c, fileWritePtr);

    } while(1);

    fclose(fileReadPtr);
    fclose(fileWritePtr);

    return 0;
}
