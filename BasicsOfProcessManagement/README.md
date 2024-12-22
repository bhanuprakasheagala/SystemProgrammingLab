# Process Management and Threading Fundamentals

This repository provides a deep dive into **Process Management** and **Threading Fundamentals**, fundamental concepts in operating systems and concurrent programming. Below is a detailed exploration of each topic with explanations and examples written in C/C++.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Processes vs Threads](#processes-vs-threads)
3. [Process Control Block (PCB) and Thread Control Block (TCB)](#pcb-and-tcb)
4. [Context Switching Mechanisms](#context-switching-mechanisms)
5. [Process States and Lifecycle](#process-states-and-lifecycle)
6. [Thread States and Lifecycle](#thread-states-and-lifecycle)
7. [Examples and Code Snippets](#examples-and-code-snippets)

---

## Introduction

Processes and threads form the backbone of multitasking in modern operating systems. Understanding these concepts is crucial for writing efficient and scalable applications, especially in multi-core systems. Processes are independent entities with separate memory spaces, while threads are lightweight units of execution sharing the same process space.

---

## Processes vs Threads

### Process:
- **Definition**: A process is an independent program in execution.
- **Memory**: Each process has its own memory space (heap, stack, code, data).
- **Isolation**: Processes are isolated from one another.
- **Communication**: Processes communicate using Inter-Process Communication (IPC) mechanisms like pipes, shared memory, or message queues.

### Thread:
- **Definition**: A thread is a lightweight unit of execution within a process.
- **Memory**: Threads within the same process share the same address space.
- **Efficiency**: Threads are faster to create and manage compared to processes.
- **Communication**: Threads communicate directly since they share the same memory.

| Feature                 | Process                 | Thread               |
|-------------------------|-------------------------|----------------------|
| Memory Space            | Separate               | Shared               |
| Communication           | IPC                    | Direct               |
| Context Switching       | Slow                   | Fast                 |
| Creation Overhead       | High                   | Low                  |

---

## Process Control Block (PCB) and Thread Control Block (TCB)

### Process Control Block (PCB)
- The PCB is a data structure used by the operating system to store all information about a process.
- **Contents**:
  - Process ID (PID)
  - Process state
  - Program counter
  - CPU registers
  - Memory management information (page tables, base and limit registers)
  - I/O status information

### Thread Control Block (TCB)
- The TCB is similar to the PCB but contains information specific to a thread.
- **Contents**:
  - Thread ID
  - Thread state
  - Program counter
  - Stack pointer
  - CPU register set
  - Thread-specific storage

---

## Context Switching Mechanisms

### Definition:
Context switching is the process of saving the state of a currently executing process or thread and restoring the state of a new process or thread.

### Steps in Context Switching:
1. Save the current state (PCB/TCB) of the running process/thread.
2. Update the CPU scheduler to select the next process/thread.
3. Load the state of the selected process/thread.

### Overhead:
- Context switching incurs overhead as CPU cycles are spent saving and restoring states rather than executing user code.

---

## Process States and Lifecycle

### States:
1. **New**: The process is being created.
2. **Ready**: The process is ready to run but is waiting for CPU time.
3. **Running**: The process is currently executing on the CPU.
4. **Blocked/Waiting**: The process is waiting for an event (e.g., I/O completion).
5. **Terminated**: The process has completed execution.

### Diagram:

```
    [New] ---> [Ready] ---> [Running] ---> [Terminated]
                  |              |
                  v              v
               [Waiting] <-------
```

---

## Thread States and Lifecycle

### States:
1. **New**: The thread is created but not yet started.
2. **Runnable**: The thread is ready to run but waiting for CPU time.
3. **Running**: The thread is currently executing.
4. **Blocked/Waiting**: The thread is waiting for a resource or an event.
5. **Terminated**: The thread has finished execution.

---

## Examples and Code Snippets

### Example 1: Process Creation in C (using `fork`)
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        printf("Child process: PID = %d\n", getpid());
    } else if (pid > 0) {
        // Parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
    } else {
        perror("fork failed");
    }
    return 0;
}
```

### Example 2: Thread Creation in C++ (using `std::thread`)
```cpp
#include <iostream>
#include <thread>

void threadFunction() {
    std::cout << "Thread is running!" << std::endl;
}

int main() {
    std::thread t(threadFunction);
    t.join(); // Wait for the thread to finish
    std::cout << "Main thread continues..." << std::endl;
    return 0;
}
```

### Example 3: Context Switching Simulation
```c
#include <stdio.h>
#include <ucontext.h>

ucontext_t ctx1, ctx2;

void function1() {
    printf("Function 1: Entering\n");
    swapcontext(&ctx1, &ctx2); // Switch to ctx2
    printf("Function 1: Resuming\n");
}

void function2() {
    printf("Function 2: Entering\n");
    swapcontext(&ctx2, &ctx1); // Switch back to ctx1
    printf("Function 2: Resuming\n");
}

int main() {
    char stack1[8192], stack2[8192];

    getcontext(&ctx1);
    ctx1.uc_stack.ss_sp = stack1;
    ctx1.uc_stack.ss_size = sizeof(stack1);
    ctx1.uc_link = NULL;
    makecontext(&ctx1, function1, 0);

    getcontext(&ctx2);
    ctx2.uc_stack.ss_sp = stack2;
    ctx2.uc_stack.ss_size = sizeof(stack2);
    ctx2.uc_link = NULL;
    makecontext(&ctx2, function2, 0);

    swapcontext(&ctx1, &ctx2); // Start with ctx2
    return 0;
}
```