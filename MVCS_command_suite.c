#include <stdio.h>
#include <windows.h>
#include "VCS_Structure.h"


int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "init") == 0) {
        printf("MVCS repository initialized.\n");
        CreateDirectory(".mvcs", NULL);
        CreateDirectory(".//.mvcs//objects", NULL);
        CreateDirectory(".//.mvcs//refs", NULL);
        CreateDirectory(".//.mvcs//refs//heads", NULL);
        FILE *headFile = fopen(".//.mvcs//HEAD", "w");
        if (headFile != NULL) {
            fprintf(headFile, "ref: refs/heads/main\n");
            fclose(headFile);
        }
        return 0;
    } else {
        printf("MVCS: no command given.\n");
        return 0;
    }
   
}