#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments\n");
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
    int fd2;

    if ((fd2 = open("/dev/tty", O_RDWR | O_NDELAY)) == -1) {
        perror("/dev/tty");
        exit(2);
    }

    while (1) {
        printf("Line number (0 for exit): ");
        fflush(stdout);
        sleep(5);

        if (read(fd2, buff, BUFSIZ) == 0) {
            printf("Time limit exceeded!\n");
            lseek(fd,0,SEEK_SET);
            while (read(fd, &c, 1) > 0) {
                putchar(c);
            }
            break;
        }

        line_number = atoi(buff);

        if (line_number == 0)
            return 0;

        if (line_number < 0 || line_number > MAX_SIZE || lengths[line_number] == -1) {
            printf("Wrong line number!\n");
        } else {
            if (lseek(fd, offsets[line_number], SEEK_SET) == -1) {
                perror("lseek: ");
                return 0;
            }
            if (read(fd, buff, lengths[line_number]) <= 0) {
                perror("read: ");
                return 0;
            }
            buff[lengths[line_number]] = '\0';
            printf("%s", buff);
        }
    }

    close(fd);
    close(fd2);
}
