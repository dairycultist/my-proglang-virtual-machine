#include <stdio.h>
#include "lexer.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");

    if (!file) {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    printf("\n");

    fclose(file);

    return 0;
}
