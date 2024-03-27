#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

// 返回路径中的最后一个文件
// 输入的路径是"/usr/local/bin/myprogram"，返回"myprogram"。
char *baseName(char *pathname)
{
    char *prev = 0;
    char *cur = strchr(pathname, '/');
    while (cur != 0)
    {
        prev = cur;
        cur = strchr(cur + 1, '/');
    }
    return prev;
}

void find(char *cur_path, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(cur_path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", cur_path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", cur_path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        char* find_name = baseName(cur_path);
        if(find_name == 0 || strcmp(find_name + 1, target) !=  0){
            close(fd);
            break;;
        }else{
            printf("%s\n", cur_path);
        }
        close(fd);
        break;

    case T_DIR:

        memset(buf, 0, sizeof(buf));
        uint cur_path_len = strlen(cur_path);
        memcpy(buf, cur_path, cur_path_len);
        buf[cur_path_len] = '/';
        p = buf + cur_path_len + 1;
        
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
                continue;
            memcpy(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            find(buf, target);
        }
        close(fd);
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "usage: find [directory] [target filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}