#include <stdio.h>

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("bad input\n");
        return 0;
    }

    FILE *file;

    if ((file = popen(argv[1], "w")) == NULL) {
        perror("open");
        return 0;
    }

    char line[20] = "HeLLo FrOM CHiLd 1\n";
    char line2[20] = "Hello AGain !\n";

    fputs(line, file);
    fputs(line2, file);

    if (pclose(file) == -1) {
        perror("close");
    }

    return 0;
}
