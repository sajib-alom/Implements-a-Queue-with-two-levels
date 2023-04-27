#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Process {
    char* name;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
} Process;

Process* create_process(char* name, int arrival_time, int burst_time, int priority) {
    Process* process = (Process*) malloc(sizeof(Process));
    process->name = name;
    process->arrival_time = arrival_time;
    process->burst_time = burst_time;
    process->priority = priority;
    process->remaining_time = burst_time;
    return process;
}

typedef struct Node {
    Process* process;
    struct Node* next;
} Node;

Node* create_node(Process* process) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->process = process;
    node->next = NULL;
    return node;
}

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue* create_queue() {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(Queue* queue, Process* process) {
    Node* node = create_node(process);
    if (queue->front == NULL) {
        queue->front = node;
    } else {
        queue->rear->next = node;
    }
    queue->rear = node;
}

Process* dequeue(Queue* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    Node* node = queue->front;
    Process* process = node->process;
    queue->front = node->next;
    free(node);
    return process;
}

bool is_empty(Queue* queue) {
    return queue->front == NULL;
}

bool is_not_empty(Queue* queue) {
    return !is_empty(queue);
}

void print_processes(Process** processes, int num_processes) {
    printf("| Process | Arrival Time | Burst Time | Priority |\n");
    printf("|---------|--------------|------------|----------|\n");
    for (int i = 0; i < num_processes; i++) {
        Process* process = processes[i];
        printf("| %s | %d | %d | %d |\n", process->name, process->arrival_time, process->burst_time, process->priority);
    }
}

void two_level_scheduling(Process** processes, int num_processes) {
    int time = 0;
    Queue* q1 = create_queue();
    Queue* q2 = create_queue();
    Process* current_process = NULL;

    for (int i = 0; i < num_processes; i++) {
        Process* process = processes[i];
        enqueue(q1, process);
    }

    printf("Starting two-level scheduling\n");

    while (is_not_empty(q1) || is_not_empty(q2) || current_process != NULL) {
        if (is_not_empty(q1)) {
            Process* next_process = q1->front->process;
            if (current_process == NULL || next_process->priority < current_process->priority) {
                if (current_process != NULL) {
                    printf("Process named %s preempted and has been enqueued in Queue 2\n", current_process->name);
                    enqueue(q2, current_process);
                }
                current_process = dequeue(q1);
            } else {
                enqueue(q1, dequeue(q1));
            }

            printf("Executing process %s from Queue 1\n", current_process->name);
            time += 2;
            current_process->remaining_time -= 2;
                  if (current_process->remaining_time <= 0) {
            printf("Process named %s completed its execution\n", current_process->name);
            current_process = NULL;
        }
    } else if (is_not_empty(q2)) {
        Process* next_process = q2->front->process;
        if (current_process == NULL || next_process->remaining_time < current_process->remaining_time) {
            current_process = dequeue(q2);
        } else {
            enqueue(q2, dequeue(q2));
        }

        printf("Executing process %s from Queue 2\n", current_process->name);
        time += 1;
        current_process->remaining_time -= 1;
        if (current_process->remaining_time <= 0) {
            printf("Process named %s completed its execution\n", current_process->name);
            current_process = NULL;
        }
    } else {
        time++;
    }
}

}

int main() {
Process* processes[] = {
create_process("P1", 0, 4, 2),
create_process("P2", 1, 1, 1),
create_process("P3", 2, 2, 3),
create_process("P4", 3, 1, 4),
create_process("P5", 4, 5, 0)
};
int num_processes = sizeof(processes) / sizeof(Process*);
print_processes(processes, num_processes);
two_level_scheduling(processes, num_processes);
return 0;
}