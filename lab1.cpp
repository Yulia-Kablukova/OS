#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ulimit.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

extern char** environ;

int main(int argc, char* argv[])
{
     char options[] = "ispuU:cC:dvV:";
     int c;
     struct rlimit rlim;

     while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
        case 'i':
            printf("uid = %d\neuid = %d\ngid = %d\negid = %d\n\n", getuid(), geteuid(), getgid(), getegid());
            break;
        case 's':
            setpgid(0, 0);
            break;
        case 'p':
            printf("pid = %d\nppid = %d\npgid = %d\nsid = %d\n\n", getpid(), getppid(), getpgid(0), getsid(0));
            break;
        case 'u':
            printf("ulimit = %ld\n\n", ulimit(UL_GETFSIZE, 0));
            break;
        case 'U':
            if (ulimit(UL_SETFSIZE, atol(optarg)) == -1) 
            {
                perror("Unable to change ulimit\n");
            }
            break;
        case 'c':
            getrlimit(RLIMIT_CORE, &rlim);
            printf("core size = %ld\n\n", rlim.rlim_cur);
            break;
        case 'C':
            getrlimit(RLIMIT_CORE, &rlim);
            rlim.rlim_cur = atol(optarg);
            if (setrlimit(RLIMIT_CORE, &rlim) == -1) 
            {
                perror("Unable to change core\n\n");
            }
            break;
        case 'd':
            printf("cwd: %s\n\n", getcwd(NULL, 100));
            break;
        case 'v':
            for (char** p = environ; *p; p++)
            {
                printf("%s\n", *p);
            }
            printf("\n");
            break;
        case 'V':
            putenv(optarg);
            break;
        case '?':
            printf("Invalid option: %c\n\n", optopt);
            break;
        default:
            break;
        }
    }
     return 0;
}
