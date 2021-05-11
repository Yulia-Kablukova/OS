#include <poll.h>
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
        if (len == MAX_SIZE && c != '\n') {
            char warningMessage[] = "Warning: too long line\n";
            write(0, warningMessage, sizeof(warningMessage));
            while (c != '\n' && read(fd, &c, 1) > 0) continue;
        }
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
            lseek(fd, 0, SEEK_SET);
            while (read(fd, &c, 1) > 0) {
                write(0, &c, 1);
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
        if (line_number == 0) {
            return 0;
        }

        if (line_number < 0 || line_number > MAX_SIZE || lengths[line_number] == -1) {
            char str[] = "Wrong line number!\n";
            write(0, str, sizeof(str));
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
            write(0, buff, lengths[line_number]);
        }
    }

    close(fd);
}
