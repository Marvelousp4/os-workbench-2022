#include <assert.h>
#include <stdbool.h>
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
            PrintVersion();
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

bool PrintVersion()
{
    printf("pstree (PSmisc) 23.4\
Copyright (C) 1993-2020 Werner Almesberger and Craig Small\
PSmisc comes with ABSOLUTELY NO WARRANTY.\
This is free software, and you are welcome to redistribute it under\
the terms of the GNU General Public License.\
For more information about these matters, see the files named COPYING.");
    return 0;
}