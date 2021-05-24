#include <sys/types.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {

    srand(time(NULL));

    FILE *ptrs[2];
    char num[4];

    p2open("/bin/sort -r", ptrs);

    for (int i = 1; i <= 100; i++) {
        sprintf(num, "%2d\n", rand() % 100);
        fputs(num, ptrs[0]);
    }

    if (fclose(ptrs[0]) != 0) {
        perror("fclose");
    }

    int splitter = 0;

    while (fgets(num, 4, ptrs[1]) != NULL) {
        num[2] = '\0';
        printf("  %s", num);
        if (splitter == 9) {
            printf("\n\n");
        }
        splitter = (splitter + 1) % 10;
    }

    if (fclose(ptrs[1]) != 0) {
        perror("fclose");
    }

    return 0;
}
