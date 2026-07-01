*This project has been created as part of the 42 curriculum by jabuleje.*

# 🧩 Codexion

## 📖 Description

**Codexion** is a concurrent simulator developed in C using POSIX threads (pthreads). The project simulates a group of programmers (‘coders’) who share a limited number of dongles in order to compile code.

The simulation implements two scheduling algorithms:

- **FIFO** (First In, First Out)
- **EDF** (Earliest Deadline First)

---

## 🎯 Objectives

- To learn concurrent programming.
- To avoid data races.
- To manage mutual exclusion using mutexes.
- To synchronise threads using condition variables.
- To implement two scheduling algorithms.

---

## 🛠️ Run and other Commands

```bash
make          # Compile the source code.
make clean    # Delete the object files (.o).
make fclean   # Perform a complete cleaning.
make re       # Force a rebuild from scratch.
```

## ▶️ Uso

```bash
./codexion <number_of_coders> <burnout_time> <compile_time> <debug_time> <refactor_time> <compile_required> <dongle_cooldown> <fifo|edf>
```

Example:

```bash
./codexion 5 550 100 100 100 3 50 fifo
```

---

## 📄 Parameters

| Parameter | Description |
|-----------|-------------|
| number_of_coders | Number of coders. |
| burnout_time | Maximum time without starting a compilation. |
| compile_time | Compilation time. |
| debug_time | Debugging time. |
| refactor_time | Refactoring time. |
| compile_required | Number of compilations required per coder. |
| dongle_cooldown | Time a dongle remains locked after being released. |
| scheduler | fifo or edf. |

---

## 🔹 FIFO Algorithm

Each dongle maintains its own FIFO queue.

When a coder requests a dongle:

1. They are added to the dongle’s queue.
2. They wait their turn.
3. When they are first in the queue and the dongle is free:
   - they obtain the dongle.
4. If they have both dongles:
   - they compile.


## 🔹 EDF Algorithm

Each dongle maintains a queue sorted by deadline.

The deadline is calculated as follows:

```
deadline = last_compile_start + burnout_time
```

The coder with the earliest deadline has priority.

---

## 🧩 Checks

**Memory leaks**

```bash
valgrind ./codexion 5 1000 300 10 10 3 10 edf
```

**Data races**

```bash
valgrind --tool=drd ./codexion 3 500 50 50 50 3 10 edf
```

Running Valgrind causes delays in codexion execution. It is recommended to increase the **time_burnout** and minimize the **number_of_coders**, **burnout_time**, **compile_time**, **debug_time**, **refactor_time**, **compile_required** and **dongle_cooldown**.

Example:

```bash
valgrind ./codexion 2 550000 50 50 50 1 50 fifo
```

```bash
valgrind --tool=drd ./codexion 2 550000 50 50 50 1 50 fifo
```

---

## 🧠 Additionnal informations

### ♦️ Blocking cases handled 

#### Deadlock prevention

The main objective was to allow coders to obtain the two dongles necessary for compilation while maintaining concurrent execution and avoiding deadlocks.

To achieve this, a strategy based on each coder's identifier was used. Coders with odd identifiers attempt to acquire the left dongle first and then the right, while coders with even identifiers perform the operation in the reverse order.

This strategy breaks the unique resource acquisition order. If all threads attempted to obtain the same dongle first, a circular wait could occur in which each coder would hold one resource while waiting indefinitely for the other. By alternating the acquisition order based on the identifier's parity, this condition is eliminated, and deadlocks are avoided.

Furthermore, coders with even identifiers implement a small delay before starting their execution. This time lag reduces initial contention over the dongles, better distributes resource requests, and promotes a higher degree of parallelism during the simulation.

### ♦️ Thread synchronization mechanisms 

#### Register Serialization

To ensure that register messages are readable and consistent, access to `printf` is protected by a dedicated mutex. Before writing to the console, each programmer must acquire this mutex, guaranteeing that only one thread can print at a time.

This prevents the output of multiple threads from becoming mixed, resulting in deterministic and readable registers that accurately reflect the order of events. Data accessed by threads is also protected with a mutex, such as dongle status, queue, and cooldown.

#### Exhaustion Detection and Termination

Exhaustion detection is handled by a dedicated monitoring thread that continuously checks whether any programmer has exceeded the allowed time without compiling.

Separating this responsibility from the worker threads centralizes the detection logic and allows exhaustion to be identified as soon as it occurs, regardless of what the programmers are doing. The monitoring thread terminates only when the simulation ends, either because a scheduler times out or because all necessary builds have completed. The main thread waits (`pthread_join`) for the monitoring thread to finish before closing the program, ensuring a clean and synchronized termination.

---

## 📚 Resources
- https://dev.to/yel-bakk/codexion-4fk8 Codexion concept.
- https://dev.to/yel-bakk/thread-in-c-codexion-42-1ao Codexion threads.
- https://www.youtube.com/watch?v=faZEhIHdJx8 use mutex.
- https://www.youtube.com/watch?v=M9HHWFp84f0 multithreading vision in programming.
- https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2 Understanding multi-threading in C.

---

## 🤖 Use of AI

AI tools (**ChatGPT**) were used to:

- Understand the topic and the development of the structures.
- Provide examples of the use of threads, mutexes, locks, and destruction.
- Understand memory leaks (**Valgrind**) and how to detect them.
