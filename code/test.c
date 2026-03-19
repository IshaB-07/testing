#include <stdio.h>

// Global variable
int shared_counter = 0;

// Function to increment the global variable
void increment() {
    shared_counter++;
    printf("Incremented: %d\n", shared_counter);
}

// Function to decrement the global variable
void decrement() {
    shared_counter--;
    printf("Decremented: %d\n", shared_counter);
    if(shared_counter<0){
        printf("Counter value negative!\r\n");
    }
}

int main() {
    printf("Initial value: %d\n", shared_counter);

    // Call functions (no threading yet)
    increment();
    increment();
    decrement();
    increment();
    decrement();

    printf("Final value: %d\n", shared_counter);

    return 0;
}
