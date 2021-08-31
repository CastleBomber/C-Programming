#include "processor.h"

MESSAGE *messageCache[CACHE_SIZE]; // ptr so as not to pass by value

int cacheIndex = 0;
int messageStats[] = { 0, 0, 0, 0, 0};
int nBatchesProcessed = 0;

void addMessageToCache(char *inputLine)
{
    if (cacheIndex == CACHE_SIZE){
        messageDispatcher();
    }

    messageCache[cacheIndex] = malloc(sizeof(MESSAGE));
    messageCache[cacheIndex]->type = strtol(inputLine, &inputLine, 10); // chops off beg #
    inputLine++;

    switch(messageCache[cacheIndex]->type){
        case MSG_TYPE_1:
            //messageCache[cacheIndex]->content.string = malloc((strlen(inputLine)+1) * sizeof(char));
            //strcpy(messageCache[cacheIndex]->content.string, inputLine);
            messageCache[cacheIndex]->content.string = strdup(inputLine);
            break;
        case MSG_TYPE_2:
            for (int i = 0; i < NUM_OF_INTEGERS; i++){
                messageCache[cacheIndex]->content.integers[i]
                    = strtol(inputLine, &inputLine, 10);
                inputLine++;
            }
            inputLine = NULL;
            break;
        case MSG_TYPE_3:
            for (int i = 0; i < NUM_OF_DOUBLES; i++) {
                messageCache[cacheIndex]->content.doubles[i]
                        = strtod(inputLine, &inputLine);
                inputLine++;
            }
                inputLine = NULL;
            break;
        case MSG_TYPE_4:
            {
                char sep[] = {' '};
                strcpy(
                    messageCache[cacheIndex]->content.words[0],
                    strtok(inputLine, sep));
                for (int i = 1; i < NUM_OF_STRINGS; i++){
                    strcpy(
                        messageCache[cacheIndex]->content.words[i],
                        strtok(NULL, sep));
                }
                inputLine = NULL;
            }
            break;
        default:
            printf("NOT VALID");
    }
    cacheIndex++;
}

void messageDispatcher(void)
{
    if(cacheIndex != 0) {
        printf("\n\nRunning Message Dispatcher...");
        nBatchesProcessed++;

        for (int i = 0; i < cacheIndex; i++) {
            processMessage(messageCache[i]);
        }
        cacheIndex = 0;
    }
}

void displayMessage(MESSAGE *message){
    printf("\nTYPE %d :", message->type);
    switch(message->type){
        case MSG_TYPE_1:
            printf("%s", message->content.string);
            break;
        case MSG_TYPE_2:
            for (int i = 0; i < NUM_OF_INTEGERS; i++){
                printf(" %ld", message->content.integers[i]);
            }
            break;
        case MSG_TYPE_3:
            for (int i = 0; i < NUM_OF_DOUBLES; i++){
                printf(" %lf", message->content.doubles[i]);
            }
            break;
        case MSG_TYPE_4:
            for (int i = 0; i < NUM_OF_STRINGS; i++){
                printf(" %s", message->content.words[i]);
            }
            break;
    }
}

void freeMessage(MESSAGE *message){
    if (message->type == MSG_TYPE_1){
        free(message->content.string);
    }
    free(message);
}

void processMessage(MESSAGE *message)
{
    displayMessage(message);
    messageStats[0]++;
    messageStats[message->type]++;
    freeMessage(message);
}

void printStatistics(void)
{
    printf("\n\nDisplaying Statistics...");
    printf("\nNumber of batches processed : %d", nBatchesProcessed);
    printf("\nMessages Processed :");
    printf("\n\tTotal : %d", messageStats[0]);
    printf("\n\tType 1 : %d", messageStats[1]);
    printf("\n\tType 2 : %d", messageStats[2]);
    printf("\n\tType 3 : %d", messageStats[3]);
    printf("\n\tType 4 : %d", messageStats[4]);
}
