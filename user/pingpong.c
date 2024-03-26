#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    
    int pipe1[2], pipe2[2];
    char buf[] = {'a'}; 
    pipe(pipe1);
    pipe(pipe2);
    int ret = fork();
    
    if(ret == 0){
        close(pipe1[1]);    //关闭管道1的写端
        close(pipe2[0]);    //关闭管道2的读端
        read(pipe1[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(pipe2[1], buf, 1);
        exit(0);
    }else{
        close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], buf, 1);
        read(pipe2[0], buf, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}