#ifndef RTOS_H
#define RTOS_H

#include <Arduino.h>

#define MAX_TASKS 5
#define STACK_SIZE 128
#define QUEUE_SIZE 10

// Task states
typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED
} TaskState;

// Task Control Block (TCB)
typedef struct {
    void (*taskFunction)(void);  // Task function pointer
    uint8_t *stackPointer;       // Pointer to task's stack
    uint8_t stack[STACK_SIZE];   // Task stack
    TaskState state;             // Task state (ready, running, blocked)
} TaskControlBlock;

extern TaskControlBlock tcbArray[MAX_TASKS];
extern uint8_t currentTaskIndex;

// Function prototypes
void createTask(void (*taskFunction)(void));
void scheduler();
void yield();
void enqueue(int data);
int dequeue();
void setupTimer();
void timerISR();

#endif
