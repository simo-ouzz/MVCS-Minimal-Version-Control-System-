#include <stdio.h>
#include <string.h>
#include <windows.h>


void Init(int argc, char *argv[]) {

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
        return;
    } else {
        printf("MVCS: no command given.\n");
        return;
    }


}