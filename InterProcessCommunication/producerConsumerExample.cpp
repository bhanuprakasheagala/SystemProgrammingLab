/*
Problem: Producer-Consumer with Semaphore

You're asked to implement the producer-consumer problem using semaphores in a multithreaded
environment.

The problem statement goes like this:

There are two types of threads: producers and consumers.
Producers produce items and put them into a shared buffer.
Consumers consume items from the buffer.
The buffer has a limited capacity.
If the buffer is full, producers should wait until there is space in the buffer.
If the buffer is empty, consumers should wait until there are items in the buffer.
You need to ensure that producers and consumers can work concurrently and safely,
without race conditions or deadlocks.

Task is to implement the producer and consumer functions, and ensure they work correctly
in a multithreaded environment.
*/
#include <iostream> // Standard input-output stream
#include <thread> // For threading
#include <vector> // For vectors
#include <mutex> // For mutual exclusion (mutex)
#include <condition_variable> // For condition variables
#include <queue> // For queues

using namespace std; // Using standard namespace

class Buffer { // Definition of Buffer class
private:
    const int capacity; // Maximum capacity of the buffer
    queue<int> items; // Queue to hold items
    mutex mtx; // Mutex to ensure thread safety
    condition_variable not_full, not_empty; // Condition variables to signal whether buffer is not full or not empty

public:
    Buffer(int capacity) : capacity(capacity) {} // Constructor to initialize capacity

    void produce(int item) { // Producer function to add item to buffer
        unique_lock<mutex> lock(mtx); // Lock the mutex
        not_full.wait(lock, [this] { return items.size() < capacity; }); // Wait until buffer is not full
        items.push(item); // Add item to buffer
        not_empty.notify_all(); // Notify waiting consumers that buffer is not empty
    }

    int consume() { // Consumer function to remove item from buffer
        unique_lock<mutex> lock(mtx); // Lock the mutex
        not_empty.wait(lock, [this] { return !items.empty(); }); // Wait until buffer is not empty
        int item = items.front(); // Get the front item from buffer
        items.pop(); // Remove item from buffer
        not_full.notify_all(); // Notify waiting producers that buffer is not full
        return item; // Return the consumed item
    }
};

void producer(Buffer &buffer, int id) { // Producer function
    for (int i = 0; i < 10; ++i) { // Loop for producing items
        buffer.produce(id * 100 + i); // Produce an item and add to buffer
        cout << "Produced: " << id * 100 + i << endl; // Output the produced item
        this_thread::sleep_for(chrono::milliseconds(100)); // Sleep for some time
    }
}

void consumer(Buffer &buffer, int id) { // Consumer function
    for (int i = 0; i < 10; ++i) { // Loop for consuming items
        int item = buffer.consume(); // Consume an item from buffer
        cout << "Consumed by " << id << ": " << item << endl; // Output the consumed item
        this_thread::sleep_for(chrono::milliseconds(200)); // Sleep for some time
    }
}

int main() { // Main function
    Buffer buffer(5); // Create a buffer with capacity 5

    vector<thread> producers; // Vector to hold producer threads
    vector<thread> consumers; // Vector to hold consumer threads

    for (int i = 0; i < 2; ++i) { // Create producer threads
        producers.emplace_back(producer, ref(buffer), i); // Add a new producer thread to the vector
    }

    for (int i = 0; i < 3; ++i) { // Create consumer threads
        consumers.emplace_back(consumer, ref(buffer), i); // Add a new consumer thread to the vector
    }

    for (auto &p : producers) { // Join producer threads
        p.join(); // Wait for producer thread to finish
    }

    for (auto &c : consumers) { // Join consumer threads
        c.join(); // Wait for consumer thread to finish
    }

    return 0; // Exit main function
}