#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_PROCESS 1000

// strace可以查看所有系统调用
// cat /proc/12434/status | grep PPid查看父进程

// 定义 Process 结构体
struct Process {
    int pid;
    int ppid;
    char* name;
    int child_count;
    struct Process* children[MAX_PROCESS];
};

// 定义 Process 类型的别名
typedef struct Process Process;

// 定义 processList 数组，它的元素是 Process 结构体类型的
struct Process processList[MAX_PROCESS];

void print_process_tree(Process* process, int depth)
{
    // 输出进程信息
    for (int i = 0; i < depth; i++)
        printf("    ");
    printf("%d %s\n", process->pid, process->name);

    // 递归输出子进程
    for (int i = 0; i < process->child_count; i++)
        print_process_tree(process->children[i], depth + 1);
}

void free_process_tree(Process* process)
{
    // 递归释放子进程
    for (int i = 0; i < process->child_count; i++)
        free_process_tree(process->children[i]);
    free(process->children);
    free(process);
}

int get_processList(struct Process* process_List, int max_count)
{
    int count = 0;

    // 打开 /proc 目录
    DIR* dir = opendir("/proc");
    if (!dir) {
        perror("opendir");
        return 0;
    }

    // 遍历目录下的所有文件
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        // 跳过非目录文件
        if (ent->d_type != DT_DIR)
            continue;

        // 获取进程号
        int pid = atoi(ent->d_name);
        if (pid == 0)
            continue;

        // 获取进程的状态文件
        char path[64];
        sprintf(path, "/proc/%d/status", pid);
        FILE* fp = fopen(path, "r");
        if (!fp)
            continue;

        // 读取进程的名称
        char name[64];
        int ret = fscanf(fp, "Name: %s", name);
        if (ret < 0) {
            perror("fscanf");
            fclose(fp);
            continue;
        }
        fclose(fp);

        // 读取进程的父进程号
        fp = fopen(path, "r");
        if (!fp)
            continue;
        int ppid = -1;
        ret = fscanf(fp, "PPid: %d", &ppid);
        if (ret < 0) {
            perror("fscanf");
            fclose(fp);
            continue;
        }
        fclose(fp);

        // 保存进程信息
        process_List[count].pid = pid;
        process_List[count].ppid = ppid;
        strcpy(process_List[count].name, name);
        count++;
    }
    closedir(dir);
    return count;
}

bool showPid()
{
    // 获取进程列表
    Process processList[MAX_PROCESS];
    int count = get_processList(processList, MAX_PROCESS);
    if (count == 0)
        return false;

    // 构建进程树
    Process* root = NULL;
    for (int i = 0; i < count; i++) {
        Process* process = (Process*)malloc(sizeof(Process));
        process->pid = processList[i].pid;
        process->ppid = processList[i].ppid;
        strcpy(process->name, processList[i].name);
        process->children = NULL;
        process->child_count = 0;

        // 找到根进程
        if (process->ppid == 0)
            root = process;

        // 找到父进程
        for (int j = 0; j < count; j++) {
            if (processList[j].pid == process->ppid) {
                Process* parent = (Process*)malloc(sizeof(Process));
                parent->pid = processList[j].pid;
                parent->ppid = processList[j].ppid;
                strcpy(parent->name, processList[j].name);
                parent->children = NULL;
                parent->child_count = 0;

                // 添加子进程
                parent->child_count++;
                parent->children = (Process**)realloc(parent->children, sizeof(Process*) * parent->child_count);
                parent->children[parent->child_count - 1] = process;
                break;
            }
        }
    }

    // 输出进程树
    print_process_tree(root, 0);

    // 释放进程树
    free_process_tree(root);
    return true;
}

/*
在原来的代码中，虽然只执行了一次 ls /proc/ 命令，但是在执行过程中会读取多次 /proc 目录。
例如，每次执行 fgets 函数都会从 /proc 目录中读取一行数据，并将其保存到缓冲区中。
这样一来，尽管只执行了一次 ls 命令，但是实际上会多次读取 /proc 目录，因此也会占用较多的系统资源。

与此相比，上面的代码通过直接打开 /proc 目录并读取文件夹来获取进程编号，只需要读取一次 /proc 目录。
这样一来，可以节省大量的读取次数，从而减少对系统资源的占用。
*/
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

// -n --numeric-sort: 按照pid的数值从小到大顺序输出一个进程的直接孩子。
bool numericSort()
{
    // 按照pid的数值从小到大顺序输出一个进程的直接孩子。

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