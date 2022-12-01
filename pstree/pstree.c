#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool showPid()
{
    FILE* fp;
    char buffer[80];
    char* token;
    fp = popen("ls /proc/", "r");
    fgets(buffer, sizeof(buffer), fp);
    printf("%s", buffer);
    token = strtok(buffer, " ");
    while (token != NULL) {
        printf("Token %s\n", token);
        printf("Token+1 %s\n", token + 7);
        token = strtok(NULL, " ");
    }
    pclose(fp);
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

int main(int argc, char* argv[])
{
    int result;
    while ((result = getopt(argc, argv, "p::n::V::")) != -1) {
        switch (result) {
        case 'p':
            showPid();
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