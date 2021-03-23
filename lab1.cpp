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
    int c, invalid = 0, dflg = 0, fflg = 0, gflg = 0;
    char* f_ptr, * g_ptr;
    struct rlimit rlim;
    gid_t list[];

    printf("argc equals %d\n", argc);
    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
        case 'i':
            printf("uid = %ld\neuid = %ld\ngid = %ld\negid = %ld\ngroups = %ld\n", getuid(), geteuid(), getgid(), getegid());
            getgroups(0, list);
            for (int i = 0;i < lst.size();i++)
            {
                printf("
            }
            break;
        case 's':
            setpgid(0, 0);
            break;
        case 'p':
            printf("pid = %ld\nppid = %ld\npgid = %ld\nsid = %ld\n", getpid(), getppid(), getpgid(0), getsid(0));
            break;
        case 'u':
            printf("ulimit = %ld\n", ulimit(UL_GETFSIZE, 0));
            break;
        case 'U':
            if (ulimit(UL_SETFSIZE, atol(optarg)) == -1) {
                perror("Unable to change ulimit\n");
            }
            break;
        case 'c':
            getrlimit(RLIMIT_CORE, &rlim);
            printf("core size = %ld\n", rlim.rlim_cur);
            break;
        case 'C':
            getrlimit(RLIMIT_CORE, &rlim);
            rlim.rlim_cur = atol(optarg);
            if (setrlimit(RLIMIT_CORE, &rlim) == -1) {
                perror("Unable to change core\n");
            }
            break;
        case 'd':
            printf("cwd: %s\n", getcwd(NULL, 100));
            //dflg++;
            break;
        case 'v':
            printf("My environment variables are:\n");
            for (char** p = environ; *p; p++)
                printf("(%8x) = %8x -> %s\n", p, *p, *p);
            putchar('\n');
            break;
        case 'V':
            putenv(optarg);
            break;
            /*case 'f':
                fflg++;
                f_ptr = optarg;
                break;
            case 'g':
                gflg++;
                g_ptr = optarg;
                break;*/
        case '?':
            printf("invalid option is %c\n", optopt);
            invalid++;
            break;
        default:
            break;
        }
    }
    printf("dflg equals %d\n", dflg);
    if (fflg)
        printf("f_ptr points to %s\n", f_ptr);
    if (gflg)
        printf("g_ptr points to %s\n", g_ptr);
    printf("invalid equals %d\n", invalid);
    printf("optind equals %d\n", optind);
    if (optind < argc)
        printf("next parameter = %s\n", argv[optind]);
    return 0;
}
