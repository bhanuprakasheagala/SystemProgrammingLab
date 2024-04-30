## System Calls

Linux system calls are implemented within the kernel of the Unix-like operating systems. Here's a simplified overview of how this typically works:

1. **System Call Interface**: The kernel provides a set of interfaces, typically in the form of functions, that user-space programs can call to request services from the kernel. These interfaces are standardized and documented, allowing programmers to interact with the kernel in a consistent manner.

2. **User-Space Invocation**: When a user-space program needs to perform a privileged operation or access system resources (such as files, devices, or network sockets), it makes a system call by invoking a corresponding function from a library, such as the C standard library. Examples of such functions include `open()`, `read()`, `write()`, `fork()`, `exec()`, etc.

3. **Transition to Kernel Mode**: When a user-space program makes a system call, the CPU switches from user mode to kernel mode, which gives the kernel full access to system resources and privileged instructions.

4. **System Call Handling**: Once the CPU is in kernel mode, the kernel intercepts the system call and identifies the requested service based on the provided parameters. It then executes the necessary code to fulfill the request. This may involve accessing hardware, managing memory, scheduling tasks, or performing other low-level operations.

5. **Return to User Space**: After the system call has been executed, control returns to the user-space program, and any results or error codes are passed back to the calling program.

6. **Error Handling**: If an error occurs during the execution of a system call, the kernel returns an error code to the user-space program, indicating the nature of the error. The program can then handle the error appropriately, either by retrying the operation, displaying an error message to the user, or taking other corrective actions.

The actual implementation of system calls varies depending on the specific Unix-like operating system. However, most Unix-like systems follow a similar overall structure, with the kernel providing a set of well-defined interfaces for user-space programs to interact with the underlying system hardware and resources. These interfaces are carefully designed to balance flexibility, performance, and security.

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
