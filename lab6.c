#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

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

    while (read(fd, &c, 1) > 0) {
        len++;
        if (c == '\n') {
            lengths[line++] = len;
            offsets[line] = lseek(fd, 0L, SEEK_CUR);
            if (offsets[line] == -1) {
                perror("lseek: ");
            }
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
                if (lseek(fd, offsets[line_number], SEEK_SET) == -1) {
                    perror("lseek: ");
                    break;
                }
                if (read(fd, buff, lengths[line_number]) <= 0) {
                    perror("read: ");
                    break;
                }
                buff[lengths[line_number]] = '\0';
                printf("%s", buff);
            }
            break;
        }

        if (line_number == 0)
            break;

        if (line_number < 0 || line_number > MAX_SIZE || lengths[line_number] == -1) {
            printf("Wrong line number!\n");
        } else {
            if (lseek(fd, offsets[line_number], SEEK_SET) == -1) {
                perror("lseek: ");
                break;
            }
            if (read(fd, buff, lengths[line_number]) <= 0) {
                perror("read: ");
                break;
            }
            buff[lengths[line_number]] = '\0';
            printf("%s", buff);
        }

        gettimeofday(&start, NULL);
    }
    close(fd);
    return 0;
}
