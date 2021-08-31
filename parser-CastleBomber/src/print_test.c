#include "print.h"

int main(int argc, char **argv)
{
    freopen(argv[1], "r", stdin);
    //printf("This:\n");
    //printf(argv[1]);

    NODE *fullProgram = program();
    printf("Done parsing...\n");
    printProgram(fullProgram, 0);
    freeParseTree(&fullProgram);

    exit(0);
}
