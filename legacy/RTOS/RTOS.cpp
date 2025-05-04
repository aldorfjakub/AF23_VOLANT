#include "RTOS.h"

TaskControlBlock tcbArray[MAX_TASKS];  // Array of task control blocks
uint8_t currentTaskIndex = 0;
uint8_t nextTaskIndex = 0;

int queue[QUEUE_SIZE];  // FIFO queue for message passing
int front = 0, rear = 0;

volatile uint32_t timerInterruptCount = 0;
// Create a task by initializing its TCB
void createTask(void (*taskFunction)(void)) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tcbArray[i].state == TASK_BLOCKED) {
            tcbArray[i].taskFunction = taskFunction;
            tcbArray[i].stackPointer = &tcbArray[i].stack[STACK_SIZE - 1];
            tcbArray[i].state = TASK_READY;
            break;
        }
    }
}

// Round-robin scheduler for cooperative multitasking
void scheduler() {
    while (true) {
        for (int i = 0; i < MAX_TASKS; i++) {
            nextTaskIndex = (currentTaskIndex + 1) % MAX_TASKS;
            if (tcbArray[nextTaskIndex].state == TASK_READY) {
                currentTaskIndex = nextTaskIndex;
                tcbArray[currentTaskIndex].state = TASK_RUNNING;
                tcbArray[currentTaskIndex].taskFunction();  // Call task function
                tcbArray[currentTaskIndex].state = TASK_READY;  // Set back to ready
            }
        }
    }
}

// Yield function for cooperative multitasking
void yield() {
    tcbArray[currentTaskIndex].state = TASK_READY;  // Set current task to ready
    scheduler();  // Run the scheduler to pick the next task
}

// Enqueue data into the FIFO queue
void enqueue(int data) {
    if ((rear + 1) % QUEUE_SIZE != front) {  // Check if the queue is full
        queue[rear] = data;
        rear = (rear + 1) % QUEUE_SIZE;
    }
}

// Dequeue data from the FIFO queue
int dequeue() {
    if (front != rear) {  // Check if the queue is empty
        int data = queue[front];
        front = (front + 1) % QUEUE_SIZE;
        return data;
    }
    return -1;  // Queue is empty
}

// Setup a timer interrupt for periodic task switching
void setupTimer() {
    // Set up a timer interrupt (1ms for demonstration)
    timerISR();

}

// Timer interrupt service routine (ISR)
void timerISR() {
    yield();  // Yield to allow task switching
}
