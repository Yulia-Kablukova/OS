#include <unistd.h>
#include <sys/types.h>
#include <iostream>

void print_and_open(){
        printf("EUID: %d\n", geteuid());
        printf("UID: %d\n", getuid());
        FILE* file = fopen("secret.txt", "r");
        if (file == NULL){
                perror("Cannot open file");
        }
        else {
        fclose(file);
        }
}

int main(){
        print_and_open();
        printf("changing  EUID...\n");
        if (-1 == setuid(getuid())){
                perror("setuid: ");
                return 0;
        }
        print_and_open();
}
