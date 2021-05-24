#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_SIZE 1000

int main() {

    char buf[MAX_SIZE];
    int len;

    while ((len = read(0, buf, MAX_SIZE)) > 0) {
        for (int i = 0; i < len; i++) {
            buf[i] = toupper(buf[i]);
        }
        if (write(1, buf, len) == -1){
            perror("write");
        }
    }

    return 0;
}
