#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/mman.h>

#define MAX_SIZE 100

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong number of arguments");
        return 0;
    }

    int fd;

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("Cannot open file");
        return 0;
    }

    char c;
    int len = 0;
    int line = 1;
    long offsets[MAX_SIZE];
    int lengths[MAX_SIZE];

    for (int i = 0; i < MAX_SIZE; i++) {
        lengths[i] = -1;
    }

    offsets[0] = 0L;
    offsets[1] = 0L;

    off_t size = lseek(fd, 0, SEEK_END);
    char* pa = (int *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

    for (int i = 0; i < size; i++) {
        len++;
        if (*(pa + i) == '\n') {
            lengths[line++] = len;
            offsets[line] = pa + i + 1;
            /*if (offsets[line] == -1) {
                perror("lseek: ");
            }*/
            len = 0;
        }
    }

    int line_number = 0;
    char buff[MAX_SIZE];
    struct timeval start, end;

    gettimeofday(&start, NULL);

    while (printf("Line number (0 for exit): ") && scanf("%d", &line_number)) {
        gettimeofday(&end, NULL);

        if (end.tv_sec - start.tv_sec > 5) {
            printf("Time limit exceeded!\n");
            for (line_number = 1; line_number < line; line_number++) {
                write(1, offsets[line_number], lengths[line_number]);
            }
            break;
        }

        if (line_number == 0)
            break;

        if (line_number < 0 || line_number > MAX_SIZE || lengths[line_number] == -1) {
            printf("Wrong line number!\n");
        } else {
            write(1, offsets[line_number], lengths[line_number]);
        }

        gettimeofday(&start, NULL);
    }
    close(fd);
    return 0;
}
