#include "kernel/types.h"
#include "user/user.h"

void runprocess(int fd){
    int passed_num = 0; //传输过来的数值
    int forked = 0;     //标志位
    int pipes[2];
    int my_num = 0;
    while(1){
        int read_bytes = read(fd, &passed_num, 4);
        //管道传输的36个数据已经传输完毕
        if(read_bytes == 0){
            close(fd);
            if(forked){
                close(pipes[1]);
                //等待子进程返回
                int childPid;
                wait(&childPid);
            }
            exit(0);
        }
        //如果是第一次读取
        if(my_num == 0){
            my_num = passed_num;
            printf("prime %d\n", my_num);
        }
        //不能被整除的数字将会传入下一个管道
        if(passed_num % my_num != 0){
            if(!forked){  //每进入一个新的进程后forked = 0
                pipe(pipes);
                forked = 1;
                if(fork() == 0){
                    close(pipes[1]);
                    close(fd);
                    runprocess(pipes[0]);
                }else{
                    close(pipes[0]);
                }
            }
            //被传到下一个管道
            write(pipes[1], &passed_num, 4);
        }
    }
}
int main(int argc, char* argv[]){
    int pipes[2];
    pipe(pipes);
    for(int i = 2; i <= 35; i++){
        write(pipes[1], &i, 4);
    }
    close(pipes[1]);
    runprocess(pipes[0]);
    exit(0);
}