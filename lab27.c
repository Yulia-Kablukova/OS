#include <stdio.h>

#define MAX_SIZE 1000

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("bad input\n");
        return 0;
    }

    FILE *fin, *fout;
    char line[MAX_SIZE];

    if ((fin = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        return 0;
    }

    if ((fout = popen("wc -l", "w")) == NULL) {
        perror("popen");
        return 0;
    }

    while (fgets(line, MAX_SIZE, fin) != NULL) {
        if (line[0] == '\n') {
            fputs(line, fout);
        }
    }

    if (fclose(fin) != 0) {
        perror("fclose");
    }
    if (pclose(fout) == -1) {
        perror("pclose");
    }

    return 0;
}
