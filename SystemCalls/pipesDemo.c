/*
Create a Pipe between a parent and child process. The parent process should send a
string to the child process through the pipe, and the child process should print it.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {

    int fd[2];
    pid_t pid;
    char write_msg[] = "Hello, Child process!!";
    char read_msg[30];

    if(pipe(fd) == -1) {
        perror("Pipe creation failed!\n");
        return 1;
    }

    pid = fork();

    if(pid < 0) {
        perror("Creation of child process (fork) failed!\n");
        return 1;
    }

    if(pid > 0) {   // Parent process
        close(fd[0]); // Close unused read end
        write(fd[1], write_msg, strlen(write_msg)+1);
        close(fd[1]); // Close unused write end
    }
    else {  // Child process
        close(fd[1]); // Close unused write end
        read(fd[0], read_msg, sizeof(read_msg));
        printf("Child process received: %s\n", read_msg);
        close(fd[0]); // Close read end
    }

    return 0;
}