#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
    // open() system call
    int fd = open("example.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    // Returns a file descriptor for 'example.txt'
    // O_RDWR: Open for reading and writing
    // O_CREAT: Create the file if it does not exist
    // O_TRUNC: Truncate the file to 0 length if it already exists
    // S_IRUSR | S_IWUSR: Sets permissions to read and write for the file owner

    if (fd == -1) {
        perror("open"); // Print error message if open() fails
        exit(EXIT_FAILURE); // Exit program if open() fails
    }

    // write() system call
    const char *data = "Hello, world!\n";
    ssize_t bytes_written = write(fd, data, strlen(data));
    // Writes data to 'fd' from 'data' buffer
    // strlen(data): Number of bytes to write

    if (bytes_written == -1) {
        perror("write"); // Print error message if write() fails
        exit(EXIT_FAILURE); // Exit program if write() fails
    }

    // close() system call
    if (close(fd) == -1) {
        perror("close"); // Print error message if close() fails
        exit(EXIT_FAILURE); // Exit program if close() fails
    }

    // open() system call to read
    fd = open("example.txt", O_RDONLY);
    // Reopen 'example.txt' for reading only

    if (fd == -1) {
        perror("open"); // Print error message if open() fails
        exit(EXIT_FAILURE); // Exit program if open() fails
    }

    // read() system call
    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    // Reads data from 'fd' into 'buffer'
    // sizeof(buffer): Maximum number of bytes to read

    if (bytes_read == -1) {
        perror("read"); // Print error message if read() fails
        exit(EXIT_FAILURE); // Exit program if read() fails
    }

    // write() system call to standard output
    ssize_t bytes_written_output = write(STDOUT_FILENO, buffer, bytes_read);
    // Writes data from 'buffer' to standard output (stdout)

    if (bytes_written_output == -1) {
        perror("write"); // Print error message if write() fails
        exit(EXIT_FAILURE); // Exit program if write() fails
    }

    // close() system call
    if (close(fd) == -1) {
        perror("close"); // Print error message if close() fails
        exit(EXIT_FAILURE); // Exit program if close() fails
    }

    // fork() system call
    pid_t pid = fork(); // Create a child process

    if (pid == -1) {
        perror("fork"); // Print error message if fork() fails
        exit(EXIT_FAILURE); // Exit program if fork() fails
    }

    if (pid == 0) {
        // Child process
        printf("Child process\n");
        // execve() system call
        char *args[] = {"/bin/ls", "-l", NULL};
        // Arguments for the 'ls' command
        if (execve("/bin/ls", args, NULL) == -1) {
            perror("execve"); // Print error message if execve() fails
            exit(EXIT_FAILURE); // Exit child process if execve() fails
        }
    } else {
        // Parent process
        printf("Parent process\n");
    }

    return 0;
}