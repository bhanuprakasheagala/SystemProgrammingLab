### Threads in Linux: Background and Implementation

#### 1. **Background Concepts:**
   To understand thread implementation in Linux, it’s essential to cover key concepts like processes, kernel-level threads, user-level threads, and their role in concurrent execution.

   - **Processes:** 
     A process is an instance of a program in execution. In Linux, each process has its own memory space, with code, data, stack, and heap segments. Processes are isolated from each other, which ensures protection but comes with a performance overhead for inter-process communication (IPC).

   - **Threads:**
     A thread is the smallest unit of CPU execution within a process. Multiple threads within the same process share memory and other resources (e.g., open file descriptors) but execute independently, allowing better utilization of multi-core processors for parallel tasks. In Linux, threads are treated as lightweight processes.

   - **Concurrency vs. Parallelism:**
     Concurrency refers to multiple tasks making progress over time, while parallelism refers to multiple tasks running at the same time on multiple cores. Threads enable both concepts by splitting tasks into smaller units that can run concurrently or in parallel.

#### Processes vs. Threads:

- **Processes**: A process in Linux has its own memory space (code, data, heap, and stack). Each process is isolated from other processes, ensuring protection, but this also incurs overhead for inter-process communication (IPC). A process is the fundamental unit of resource ownership.
  
- **Threads**: Threads are lightweight processes that share the same memory space but execute independently. They exist within processes and are the unit of execution.

**Key Concept: Process and Threads**

```
 Process 1               Process 2
+-----------+         +-----------+
| Code      |         | Code      |
| Data      |         | Data      |
| Heap      |         | Heap      |
|-----------|         |-----------|
| Thread 1  |         | Thread 1  |
| Stack 1   |         | Stack 1   |
|-----------|         |-----------|
| Thread 2  |         | Thread 2  |
| Stack 2   |         | Stack 2   |
+-----------+         +-----------+
```

In this diagram:
- Both processes have their own memory (Code, Data, Heap).
- Each thread within a process shares the code and heap but has its own stack.

#### 2. **Linux Thread Model:**

   Linux implements threads using the **"1:1 thread model"** at the kernel level, where each user thread is mapped to a kernel thread (also known as a "lightweight process"). The kernel handles both threads and processes with similar structures, and each thread has its own task structure.

   - **Kernel-Level Threads (KLT):**
     Threads are managed directly by the kernel. Each thread in Linux is represented by a **task_struct** (process control block), which tracks the state of the thread and its resources.

     - The kernel scheduler is responsible for managing the execution of threads.
     - Threads in Linux share certain resources like memory, open files, and signal handlers, but they have their own **stack** and **CPU registers**.
     - KLTs allow true parallelism on multi-core CPUs.

   - **User-Level Threads (ULT):**
     These are implemented entirely in user space, without kernel support. User-level threads are managed by a thread library (like POSIX threads, or Pthreads), but Linux primarily uses kernel-level threads.
Linux uses the **1:1 thread model**, meaning each user thread is mapped directly to a kernel thread. Threads and processes are treated similarly in the kernel.

#### Kernel-Level Threads (KLT):

In Linux, a thread is essentially treated as a **lightweight process**, and each thread is represented by a `task_struct` in the kernel.

**task_struct** fields for threads:
- **PID**: Process ID
- **TID**: Thread ID
- **Memory information**: Shared between threads
- **CPU state**: Registers and stack (unique per thread)

```
Kernel
+----------------------------------------+
| task_struct (Process 1)                |
| +------------------------------------+ |
| | PID 123                            | |
| | Thread 1 (TID 123)                 | |
| | Stack, CPU Registers               | |
| +------------------------------------+ |
| +------------------------------------+ |
| | Thread 2 (TID 124)                 | |
| | Stack, CPU Registers               | |
| +------------------------------------+ |
+----------------------------------------+
```

Here, each thread in a process has its own TID and stack but shares the same PID and memory.

#### 3. **Task Structures in Linux:**
   
   In Linux, the **task_struct** data structure is used to represent both processes and threads. For threads, several fields in the task_struct are shared with other threads in the same process, while some are unique.

   - **Unique to Each Thread:**
     - Thread ID (TID)
     - Stack
     - CPU registers

   - **Shared Among Threads in the Same Process:**
     - Memory space (code, data, heap)
     - Open files
     - Signal handlers

   Threads are created using the `clone()` system call in Linux, which provides fine-grained control over what resources a new thread shares with the parent process.
   
The **task_struct** is the fundamental structure used to represent both processes and threads in the Linux kernel.

- For processes: The task_struct represents an entire process with all resources (memory, file descriptors, etc.).
- For threads: A task_struct represents an individual thread, with fields shared with other threads in the same process.

**Structure Fields:**
- **task_struct**: The structure stores process/thread details, including:
  - Process ID (PID) / Thread ID (TID)
  - Parent process (for hierarchy)
  - Memory map (shared among threads)
  - Scheduling information (priority, timeslice)

```
Thread 1            Thread 2
+----------------+  +----------------+
| task_struct     |  | task_struct     |
| +------------+  |  | +------------+  |
| | TID: 101   |  |  | | TID: 102   |  |
| | PID: 100   |  |  | | PID: 100   |  |
| +------------+  |  | +------------+  |
| | CPU State  |  |  | | CPU State  |  |
| | Stack      |  |  | | Stack      |  |
+----------------+  +----------------+
```

#### 4. **Thread Creation:**

   Threads in Linux are created using the `clone()` system call, which allows the parent process to decide what parts of its context to share with the new thread. The `clone()` function allows the creation of both processes and threads based on the flags passed to it.

   ```c
   pid_t clone(int (*fn)(void *), void *child_stack, int flags, void *arg, ...);
   ```

   - `fn`: The function to be executed by the new thread.
   - `child_stack`: A pointer to the new thread’s stack.
   - `flags`: Control what to share (e.g., `CLONE_VM` to share memory).
   - `arg`: Arguments passed to the function.

   Flags for thread creation:
   - `CLONE_VM`: Share memory space.
   - `CLONE_FS`: Share file system information.
   - `CLONE_FILES`: Share open file descriptors.
   - `CLONE_SIGHAND`: Share signal handlers.
   - `CLONE_THREAD`: Create a thread instead of a separate process.

   Example using `pthread_create` (which internally uses `clone()`):
   ```c
   #include <pthread.h>
   #include <stdio.h>

   void* thread_function(void* arg) {
       printf("Thread is running\n");
       return NULL;
   }

   int main() {
       pthread_t thread;
       pthread_create(&thread, NULL, thread_function, NULL);
       pthread_join(thread, NULL);
       return 0;
   }
   ```
Threads are created in Linux using the `clone()` system call. This system call allows fine-grained control over what resources a new thread will share with its parent.

#### The `clone()` System Call:

```c
pid_t clone(int (*fn)(void *), void *child_stack, int flags, void *arg);
```

- `fn`: The function the new thread will run.
- `child_stack`: A pointer to the thread's stack.
- `flags`: Control what is shared between threads.
  - `CLONE_VM`: Share memory space.
  - `CLONE_FILES`: Share file descriptors.
  - `CLONE_THREAD`: Share the same PID, make this thread part of the same process.

#### Thread Sharing with `clone()`:
```
Process 1 (Parent)
+------------------+
| Memory (Shared)  |
| File Descriptors  |
+------------------+
      |
      V
Thread 1 (Child)
+------------------+
| Stack (Private)  |
| CPU Registers    |
+------------------+
```

#### Example Using Pthreads:

Pthreads is the user-level library used to create threads, which internally uses `clone()`.

```c
pthread_t thread;
pthread_create(&thread, NULL, thread_function, NULL);
```

The `pthread_create()` function calls `clone()` and creates a new thread that shares memory and file descriptors with the parent process.

---

#### 5. **Thread Scheduling in Linux:**

   The Linux kernel uses a **completely fair scheduler (CFS)** to manage thread execution. Each thread, like a process, is treated as a separate scheduling entity, and the scheduler is responsible for ensuring fair CPU time distribution.

   - **Timeslices:** Each thread gets a certain time slice to execute. If a thread's time slice expires, it is preempted, and another thread is scheduled to run.
   - **Priority:** Threads can have different priority levels, affecting how much CPU time they get.
   - **Load Balancing:** On multi-core systems, the scheduler balances the load across CPUs, distributing threads to maximize performance.

 **Completely Fair Scheduler (CFS)** ensures that each thread gets its fair share of CPU time.

- **Timeslices**: Each thread gets a certain amount of CPU time (timeslice). After the timeslice expires, the thread is preempted.
- **Priority**: Threads with higher priority get more CPU time.

#### Basic Scheduling:

```
+----------+          +----------+          +----------+
| Thread 1 |  ----->  | Thread 2 |  ----->  | Thread 3 |
+----------+          +----------+          +----------+
```

- Each thread is executed for a certain amount of time, based on its priority and load.

#### Load Balancing on Multi-core Systems:

Linux ensures that threads are distributed across CPUs in multi-core systems for better parallelism.

```
+----------+          +----------+  
| CPU 1    |  ----->  | CPU 2    |
| Thread A |          | Thread B |
+----------+          +----------+
```
#### 6. **Thread Synchronization:**

   Since multiple threads share the same memory space, there’s a risk of race conditions. Linux provides several synchronization primitives to handle this:

   - **Mutexes:** Used to ensure exclusive access to shared resources.
   - **Semaphores:** Used for controlling access to resources with limited capacity.
   - **Condition Variables:** Used for signaling between threads.
   - **Spinlocks:** Lightweight locks for short critical sections.

   These synchronization primitives are part of the Pthreads library and are supported by the Linux kernel.
Since threads share the same memory space, it’s crucial to synchronize their actions to avoid race conditions (where two threads try to access or modify the same data simultaneously).

#### Synchronization Primitives:

1. **Mutexes**: A mutex ensures exclusive access to a resource.
   
   ```c
   pthread_mutex_lock(&mutex);
   // Critical section
   pthread_mutex_unlock(&mutex);
   ```

   **Mutex Lock Diagram**:

   ```
   Thread 1      Thread 2
   +----------+  +----------+
   | Lock     |  | Waiting  |
   | Critical |  |          |
   | Section  |  |          |
   +----------+  +----------+
   ```

2. **Semaphores**: Semaphores control access to a resource pool.
   
3. **Condition Variables**: These are used to signal state changes between threads.

4. **Spinlocks**: A busy-wait mechanism used for short critical sections.

---

#### 7. **Thread Termination and Cleanup:**

   Threads in Linux can terminate by returning from the thread function or by calling `pthread_exit()`. To ensure proper cleanup, the parent thread often waits for the child thread to finish using `pthread_join()`. If a thread terminates without being joined, its resources are cleaned up after the parent thread terminates.

   - **Zombie Threads:** Similar to zombie processes, if a thread terminates and the parent hasn’t called `pthread_join()`, the thread becomes a zombie.
   - **Thread-Specific Data:** Linux provides mechanisms to store thread-local storage, allowing threads to maintain unique data without interfering with other threads in the same process.

When a thread finishes its execution, it either:
- Returns from its thread function.
- Calls `pthread_exit()` to explicitly terminate itself.

To ensure proper resource cleanup, the parent thread can wait for a child thread to finish using `pthread_join()`.

- **Zombie Threads**: If a thread terminates and the parent doesn’t call `pthread_join()`, the thread remains in a "zombie" state until the parent cleans it up.

```
Thread 1 (Parent)
+------------------+
| Waiting for T2   |
+------------------+

Thread 2 (Zombie)
+------------------+
| Terminated       |
+------------------+
```

---

#### 8. **Kernel Threads vs. User Threads:**

   Although Linux mostly uses kernel threads, user threads (managed entirely in user space) can still be implemented using threading libraries like Pthreads. User threads have lower overhead since no kernel intervention is required for context switching, but they can't benefit from multiple processors.

#### 9. **Tools and Commands:**
   
   - **`ps -T`:** Shows threads for a specific process.
   - **`htop`:** A graphical tool that can show threads as well as processes.
   - **`pthread` API:** Commonly used for thread management in Linux.
---
