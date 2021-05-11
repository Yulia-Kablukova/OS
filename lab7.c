#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <poll.h>

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

    int size = lseek(fd, 0, SEEK_END);
    char *pa = (char *) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

    int len = 0;
    int line = 1;
    char *offsets[MAX_SIZE];
    int lengths[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        lengths[i] = -1;
    }

    offsets[1] = pa;

    for (int i = 0; i < size; i++) {
        if (len == MAX_SIZE && *(pa + i) != '\n') {
            char warningMessage[] = "Warning: too long line\n";
            write(0, warningMessage, sizeof(warningMessage));
            while (++i < size && *(pa + i) != '\n') continue;
        }
        if (*(pa + i) == '\n') {
            lengths[line++] = len;
            offsets[line] = pa + i + 1;
            len = 0;
        } else {
            len++;
        }
    }

    int line_number = 0;
    char buff[MAX_SIZE];
    int flg;
    struct pollfd pfds[1];
    char lineNumberMessage[] = "Line number (0 for exit): ";

    while (1) {

        pfds[0].fd = 0;
        pfds[0].events = POLLIN;

        write(0, lineNumberMessage, sizeof(lineNumberMessage));
        fflush(stdout);

        flg = poll(pfds, 1, 5000);

        if (!flg) {
            char str[] = "Time limit exceeded!\n";
            write(0, str, sizeof(str));
            for (int i = 0; i < size; i++) {
                write(0, pa + i, 1);
            }
            break;
        }

        if (pfds[0].revents & POLLIN) {
            if (!read(0, buff, MAX_SIZE)) {
                char str[] = "stdin closed\n";
                write(0, str, sizeof(str));
                return 0;
            }
        }
        line_number = atoi(buff);

        if (line_number == 0)
            return 0;

        if (line_number < 0 || line_number > MAX_SIZE || lengths[line_number] == -1) {
            char str[] = "Wrong line number!\n";
            write(0, str, sizeof(str));
        } else {
            write(1, offsets[line_number], lengths[line_number]);
            char str[] = "\n";
            write(0, str, sizeof(str));
        }
    }

    munmap(0, size);
    close(fd);
}
