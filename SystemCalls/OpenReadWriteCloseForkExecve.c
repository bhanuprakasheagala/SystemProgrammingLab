#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

   // open() system call
   int fd = open("example.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   
   /*
	Returns a file descriptor for 'example.txt'
        O_RDWR : Open for reading and writing
        O_CREAT: Create the file if it does not exist
        S_IRUSR | S_IWUSR: Sets permissions to read and write for the file owner
   */
   if(fd == -1) {
	perror("Can't Open"); // Print error message if open() fails
	exit(EXIT_FAILURE); // Exit program if open() fails
   }
   
   // read() system call
   char buffer[100];
   ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
   
   	/*
		Reads data from 'fd' into 'buffer'
		sizeof(buffer): Maximum number of bytes to read
	*/
  if(bytes_read == -1) {
	perror("Can't read");
	exit(EXIT_FAILURE); // Exit program with a failure message if read() fails.
  }
  
  // write() system call
  ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
  
	/*
		Writes data from 'buffer' to standard output(stdout)
		STDOUT_FILENO: File descriptor for stdout
	*/
  if(bytes_written == -1) {
	perror("Can't write");
	exit(EXIT_FAILURE);  // Exit program with a failure message if write() fails.
  }
  
  // close() system call
  if(close(fd) == -1) {
	perror("Can't close");
	exit(EXIT_FAILURE); // Exit program with a failure message if close() fails.
  }
  
  // fork() system call
  pid_t pid = fork();
  
  if(pid == -1) {
	perror("Fork failed!!");
	exit(EXIT_FAILURE); // Exit program with a failure message if fork() fails.
  }
  
  if(pid == 0) {
	// Child process
	printf("Chile process...\n");
	
	// execve() system call
	char* args[] = {"/bin/ls", "-1", NULL};
	
	// Arguments for the 'ls' command
	execve("/bin/ls", args, NULL); // Execute the ls command
  }
  else {
	// Parent process
	printf("Parent process...\n");
  }

  return 0;

}
