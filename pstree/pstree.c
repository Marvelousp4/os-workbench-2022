#include <dirent.h>
#include <stdbool.h>
#include <stddef.h>
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
    char name[64]; // 为 name 字段分配内存
    int child_count;
    struct Process** children;
};

// 定义 Process 类型的别名
typedef struct Process Process;

Process* processList;

void printTree(Process* root, int level)
{
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%d %s\n", root->pid, root->name);
    for (int i = 0; i < root->child_count; i++) {
        printTree(root->children[i], level + 1);
    }
}

void free_process_tree(Process* root)
{
    for (int i = 0; i < root->child_count; i++) {
        free_process_tree(root->children[i]);
    }
    free(root->children);
    free(root);
}

int get_process_list(struct Process* process_List, int max_count)
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
        char* endptr;
        int pid = strtol(ent->d_name, &endptr, 10);
        if (*endptr != '\0') {
            // 字符串中包含了无效的字符，跳过
            continue;
        }

        // 获取进程的状态文件
        char path[64];
        sprintf(path, "/proc/%d/status", pid);
        FILE* fp = fopen(path, "r");
        if (!fp)
            continue;

        // 读取文件中的第一行（Name）
        char line[128];
        if (fgets(line, sizeof(line), fp) == NULL) {
            perror("fgets");
            fclose(fp);
            continue;
        }

        char name[64];
        // 解析 Name 信息
        if (sscanf(line, "Name:\t%s", name) < 0) {
            perror("sscanf");
            fclose(fp);
            continue;
        }

        // 读取文件中的第七行（PPid）
        for (int i = 0; i < 6; i++) {
            if (fgets(line, sizeof(line), fp) == NULL) {
                perror("fgets");
                fclose(fp);
                continue;
            }
        }

        // 解析 PPid 信息
        int ppid = -1;
        if (sscanf(line, "PPid: %d", &ppid) < 0) {
            perror("sscanf");
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

int find_process(int pid, struct Process* processList, int num_processes)
{
    for (int i = 0; i < num_processes; i++) {
        if (processList[i].pid == pid) {
            return i;
        }
    }
    return -1;
}

struct Process* create_node(int pid, int ppid, char* name)
{
    struct Process* node = malloc(sizeof(struct Process));
    node->pid = pid;
    node->ppid = ppid;
    strcpy(node->name, name);
    node->child_count = 0;
    node->children = malloc(sizeof(struct Process*) * MAX_PROCESS);
    for (int i = 0; i < MAX_PROCESS; i++) {
        node->children[i] = NULL;
    }
    return node;
}

struct Process* build_tree(struct Process* processList, int num_processes)
{
    // Find the root process
    int root_index = -1;
    for (int i = 0; i < num_processes; i++) {
        if (processList[i].ppid == 0) {
            root_index = i;
            break;
        }
    }
    if (root_index == -1) {
        return NULL; // No root process found
    }

    // Add the children to the tree
    struct Process* root = create_node(processList[root_index].pid, processList[root_index].ppid, processList[root_index].name);
    for (int i = 0; i < num_processes; i++) {
        int child_ppid = processList[i].ppid;
        if (child_ppid == root->pid) {
            root->children[root->child_count] = create_node(processList[i].pid, processList[i].ppid, processList[i].name);
            root->child_count++;
        } else {
            int parent_index = find_process(child_ppid, processList, num_processes);
            if (parent_index != -1) {
                struct Process* parent = root->children[parent_index];
                parent->children[parent->child_count] = create_node(processList[i].pid, processList[i].ppid, processList[i].name);
                parent->child_count++;
            }
        }
    }

    return root;
}

bool showPid()
{
    processList = (Process*)malloc(sizeof(Process) * MAX_PROCESS);

    // 获取进程列表
    int count = get_process_list(processList, MAX_PROCESS);
    if (count == 0)
        return false;

    Process* root = build_tree(processList, count);

    // 输出进程树
    printTree(root, 0);

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
    int result, optopt;
    char* optarg;
    while ((result = getopt(argc, argv, "p::n::V::")) != -1) {
        switch (result) {
        case 'p':
            if (showPid()) {
                processList = NULL;
                free(processList);
            }
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