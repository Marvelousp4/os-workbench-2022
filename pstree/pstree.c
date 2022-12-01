#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

bool showPid();
bool numericSort();
bool version();

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
            version();
            break;
        default:
            printf("default, result=%c\n", result);
            break;
        }
        // printf("argv[%d]=%s\n", optind, argv[optind]);
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
    printf("pstree (PSmisc) 23.4\n\
Copyright (C) 1993-2020 Werner Almesberger and Craig Small\n\
\n \
PSmisc comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it under\n\
the terms of the GNU General Public License.\n\
For more information about these matters, see the files named COPYING.");
    return 0;
}