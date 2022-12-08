#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
// strace可以查看所有系统调用
// cat /proc/12434/status | grep PPid查看父进程
bool showPid()
{
    DIR* dir = opendir("/proc");
    if (!dir) {
        perror("opendir");
        return 0;
    }
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type != DT_DIR)
            continue;
        int pid = atoi(ent->d_name);
        if (pid == 0)
            continue;
        char path[64];
        sprintf(path, sizeof(path), "/proc/%d/status", pid);
        char name[64];
        int ret = fscanf(fp, "Name: %s", name);
        if (ret < 0) {
            perror("fscanf");
            fclose(fp);
            continue;
        }
        fclose(fp);
        printf("PID: %d, Name: %s", pid, name);
    }
    closedir(dir);
    return 1;
    // FILE* fp;
    // char buf[80];
    // char* token;
    // int process[1000];
    // memset(process, 0, sizeof(int) * 1000);
    // int pid, count = 0;
    // fp = popen("ls /proc/", "r");
    // while (fgets(buf, sizeof(buf), fp) != 0) {
    //     pid = atoi(buf);
    //     if (pid) {
    //         process[count++] = pid;
    //         printf("%d\n", process[count - 1]);
    //     }
    //     memset(buf, 0x0, sizeof buf);
    // }
    // pclose(fp);
    // return 0;
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