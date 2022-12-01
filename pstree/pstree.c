#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// strace可以查看所有系统调用
// cat /proc/12434/status | grep PPid查看父进程
bool showPid()
{
    FILE* fp;
    char buf[80];
    char* token;
    int process[1000] = {};
    int pid, count = 0;
    fp = popen("ls /proc/", "r");
    while (fgets(buf, sizeof(buf), fp) != 0) {
        pid = atoi(buf);
        if (pid) {
            process[count++] = pid;
            printf("%d\n", pid);
        }
        memset(buf, 0x0, sizeof buf);
    }
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