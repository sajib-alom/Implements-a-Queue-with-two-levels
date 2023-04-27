import queue
import random

class Process:
    def __init__(self, name, arrival_time, burst_time, priority):
        self.name = name
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.priority = priority
        self.remaining_time = burst_time

    def __lt__(self, other):
        if self.priority == other.priority:
            return self.arrival_time < other.arrival_time
        return self.priority < other.priority

def two_level_scheduling(processes):
    time = 0
    q1 = queue.PriorityQueue()
    q2 = queue.PriorityQueue()
    completed_processes = []

    for process in processes:
        q1.put(process)

    current_process = None
    while not q1.empty() or not q2.empty() or current_process is not None:
        if not q1.empty():
            next_process = q1.get()
            if current_process is None or next_process.priority < current_process.priority:
                if current_process is not None:
                    print(f'Process named {current_process.name} preempted and has been enqueued in Queue 2')
                    q2.put(current_process)
                current_process = next_process
            else:
                q1.put(next_process)

            print(f'Executing process {current_process.name} from Queue 1')
            time += 2
            current_process.remaining_time -= 2
            if current_process.remaining_time <= 0:
                completed_processes.append(current_process)
                print(f'Process {current_process.name} completed')
                current_process = None
            else:
                print(f'Process named {current_process.name} preempted and has been enqueued in Queue 2')
                q2.put(current_process)
                current_process = None
        else:
            if current_process is None and not q2.empty():
                current_process = q2.get()
            if current_process is not None:
                print(f'Executing process {current_process.name} from Queue 2')
                time += 1
                current_process.remaining_time -= 1
                if current_process.remaining_time <= 0:
                    completed_processes.append(current_process)
                    print(f'Process {current_process.name} completed')
                    current_process = None

    print("Completed processes:")
    for process in completed_processes:
        print(process.name)

num_processes = int(input('Enter the number of processes: '))
processes = []
for i in range(num_processes):
    name = 'P' + str(i+1)
    arrival_time = random.randint(0, 10)
    burst_time = random.randint(1, 10)
    priority = random.randint(0, 10)
    processes.append(Process(name, arrival_time, burst_time, priority))

print('| Process | Arrival Time | Burst Time | Priority |')
print('|---------|--------------|------------|----------|')
for process in processes:
    print(f'| {process.name} | {process.arrival_time} | {process.burst_time} | {process.priority} |')

two_level_scheduling(processes)
