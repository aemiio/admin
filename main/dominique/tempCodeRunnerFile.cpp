#include <iostream>
#include <thread>
#include <vector>

// Function to be executed by thread 1
void printEvenNumbers(int start, int end) {
    std::cout << "Thread 1: ";
    for (int i = start; i <= end; i += 2) {
        std::cout << i << " ";
    }
}

// Function to be executed by thread 2
void printOddNumbers(int start, int end) {
    std::cout << "Thread 2: ";
    for (int i = start; i <= end; i += 2) {
        std::cout << i << " ";
    }
}

int main() {
    int start = 1;
    int end = 10;

    // Create and launch thread 1
    std::thread t1(printEvenNumbers, start, end);

    // Create and launch thread 2
    std::thread t2(printOddNumbers, start, end);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    return 0;
}