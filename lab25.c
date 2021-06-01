#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define MAX_SIZE 1000
#include <ctype.h>

int main(){
        int fd[2];
        if (pipe(fd) == -1){
                perror("pipe failed");
                return 0;
        }
        int child1 = fork();
        if (child1 < 0){
                perror("fork1 failed");
                return 0;
        }
        if (child1 == 0){
                //child1 code
                if (close(1) == -1) {
                        perror("close");
                        return 0;
                }
                int f1 = dup(fd[1]);
                if (f1 == -1){
                        perror("dup");
                        return 0;
                }
                if (close(fd[0]) == -1){
                        perror("close");
                        return 0;
                }
                if (close(fd[1]) == -1){
                        perror("close");
                        return 0;
                }
                char line[20] = "HeLLo FrOM CHiLd 1\n";
                char line2[20] = "Hello AGain !\n";
                if (write(f1, line, strlen(line)) == -1){
                        perror("write");
                        return 0;
                }
                write(f1, line2, strlen(line2));
                if (close(f1) == -1) {
                        perror("close f1");
                        return 0;
                }
        }
        else {
                int child2 = fork();
                if (child2 < 0) {
                        perror("fork2 failed");
                        return 0;
                }
                if (child2 == 0){
                        //child2 code
                        char buf[MAX_SIZE];
                        close(0);
                        int f2 = dup(fd[0]);
                        if (f2 == -1){
                                perror("dup");
                                return 0;
                        }
                        if (close(fd[0]) == -1) perror("close");
                        if (close(fd[1])== -1) perror("close");
                        int len;
                        while ((len = read(f2, buf, MAX_SIZE))) {
                                for (int i = 0; i<len; i++){
                                        buf[i] = toupper(buf[i]);
                                }
                                if (write(1, buf, len) == -1){
                                        perror("write");
                                        return 0;
                                }
                        }
                        if (close(f2) == -1) {
                                perror("close f2");
                                return 0;
                        }
                }
                else {
                        if (close(fd[0]) == -1) {
                                perror("close fd0");
                                return 0;
                        }
                        if (close(fd[1]) == -1) {
                                perror("close fd1");
                                return 0;
                        }
                }
        }
}
