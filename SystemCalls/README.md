## Basic System Calls

1. **open()**:
   - **Purpose**: Opens a file and returns a file descriptor.
   - **Parameters**: 
     - `const char *pathname`: Pathname of the file to be opened.
     - `int flags`: Flags specifying the mode of opening.
     - `mode_t mode`: Permissions to be applied if the file is created.
   - **Return value**: Returns a file descriptor on success, -1 on error.
  
    ```
      #include <fcntl.h>
      int open(const char *pathname, int flags);
      int open(const char *pathname, int flags, mode_t mode);
    ```


2. **read()**:
   - **Purpose**: Reads data from an open file descriptor.
   - **Parameters**: 
     - `int fd`: File descriptor of the file to be read from.
     - `void *buf`: Buffer to store the read data.
     - `size_t count`: Number of bytes to read.
   - **Return value**: Returns the number of bytes read, 0 at end of file, -1 on error.
  
     ```
        #include <unistd.h>
        ssize_t read(int fd, void *buf, size_t count);
     ```
3. **write()**:
   - **Purpose**: Writes data to an open file descriptor.
   - **Parameters**: 
     - `int fd`: File descriptor of the file to be written to.
     - `const void *buf`: Buffer containing the data to be written.
     - `size_t count`: Number of bytes to write.
   - **Return value**: Returns the number of bytes written, -1 on error.
  
     ```
      #include <unistd.h>
      ssize_t write(int fd, const void *buf, size_t count);
     ```


4. **close()**:
   - **Purpose**: Closes a file descriptor.
   - **Parameters**: 
     - `int fd`: File descriptor to be closed.
   - **Return value**: Returns 0 on success, -1 on error.
  
     ```
       #include <unistd.h>
       int close(int fd);
     ```

5. **fork()**:
   - **Purpose**: Creates a new process by duplicating the calling process.
   - **Parameters**: None.
   - **Return value**: In the parent process, the process ID (PID) of the child process is returned, and in the child process, 0 is returned. On failure, -1 is returned.
  
     ```
         #include <unistd.h>
         pid_t fork(void);
     ```


6. **execve()**:
   - **Purpose**: Executes a program.
   - **Parameters**: 
     - `const char *filename`: Path to the executable file.
     - `char *const argv[]`: Array of pointers to null-terminated strings that represent the argument list available to the executed program.
     - `char *const envp[]`: Array of pointers to null-terminated strings representing the environment of the new process.
   - **Return value**: Only returns on error, -1, and sets errno accordingly.
  
     ```
         #include <unistd.h>
         int execve(const char *filename, char *const argv[], char *const envp[]);
     ```


These are just a few examples of Linux system calls. There are many more available, each serving specific purposes in managing processes, files, memory, etc., within the Linux environment.
