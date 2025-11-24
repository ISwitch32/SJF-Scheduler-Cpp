# Non-Preemptive SJF CPU Scheduler 🖥️

A robust C++ simulation of the **Shortest Job First (Non-Preemptive)** process scheduling algorithm. This project simulates the core logic of an Operating System scheduler by parsing process data, managing a ready queue, and calculating precise performance metrics.

## 🚀 Features
* **Object-Oriented Architecture:** Built using a modular `clsProcess` class and STL Vectors for memory management.
* **Dynamic File Parsing:** Reads process data (Arrival Time, Burst Time) from `ProcessInput.txt` using `fstream` and `stringstream`.
* **State Machine Logic:** Implements a precise "Busy vs. Free" CPU state machine to handle arrival times and idle gaps without logic errors.
* **Starvation Simulation:** Accurately demonstrates the "Starvation" phenomenon where long jobs are delayed by shorter arriving jobs.
* **Statistical Reporting:** Calculates and exports **Turnaround Time**, **Waiting Time (Delay)**, and **Response Time** to both the console and `ProcessOutput.txt`.

## 🛠️ Tech Stack
* **Language:** C++
* **Concepts:** OOP, STL Vectors, File Streams, Formatting (`iomanip`).

## 📊 Sample Output
The program generates the **Execution Order** followed by a detailed analysis of each process.

**Input (`ProcessInput.txt`):**
```text
A 0 3
B 2 6
C 4 4
D 6 5
E 8 3

Ouput
Execution Order: ABECD

A: (response=0, turnaround=3, delay=0)
B: (response=1, turnaround=7, delay=1)
C: (response=7, turnaround=11, delay=7)
D: (response=9, turnaround=14, delay=9)
E: (response=1, turnaround=3, delay=1)
