#include <assert.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int result;
    while ((result = getopt(argc, argv, "p::n::V::")) != -1) {
        switch (result) {
        case 'p':
            printf("option=a, optopt=%c, optarg=%s\n", optopt, optarg);
            break;
        case 'n':
            printf("option=b, optopt=%c, optarg=%s\n", optopt, optarg);
            break;
        case 'V':
            printf("option=c, optopt=%c, optarg=%s\n", optopt, optarg);
            break;
        default:
            printf("default, result=%c\n", result);
            break;
        }
        printf("argv[%d]=%s\n", optind, argv[optind]);
    }
    return 0;
}

bool showPid()
{
    return 0;
}

bool numericSort()
{
    return 0;
}

bool version()
{
    return 0;
}