*This project has been created as part of the 42 curriculum by gpecelli.*

## Description
Codexion is a concurrency simulation built in C that models a group of coders sharing limited resources. Several coders sit in a circular inclusive co-working hub and must share "quantum USB dongles" to compile their code. 

To compile successfully, a coder needs both the dongle on their left and the dongle on their right simultaneously. The goal of this project is to implement robust thread synchronization to prevent resource starvation, deadlocks, and ultimately ensure that no coder "burns out" while waiting for resources. The simulation manages shared resources using multiple arbitration policies (FIFO and EDF) through POSIX threads and mutexes.

## Instructions

### Compilation
A standard `Makefile` is provided. You can compile the project using `make`:
```bash
make
```
This will produce an executable named `codexion`.

Other available rules:
- `make clean`: Removes object files.
- `make fclean`: Removes object files and the executable.
- `make re`: Recompiles the entire project from scratch.

### Execution
Run the simulation by providing the required parameters:
```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- `number_of_coders`: Number of coders (and also dongles).
- `time_to_burnout`: Milliseconds a coder can wait before burning out.
- `time_to_compile`: Milliseconds spent compiling (requires two dongles).
- `time_to_debug`: Milliseconds spent debugging.
- `time_to_refactor`: Milliseconds spent refactoring.
- `number_of_compiles_required`: Simulation stops once all coders reach this number (optional bounds for completion).
- `dongle_cooldown`: Milliseconds a dongle must cool down before being used again.
- `scheduler`: `fifo` (First In, First Out) or `edf` (Earliest Deadline First).

### Checking for Memory Leaks
To ensure there are no memory leaks, you can compile the project with AddressSanitizer or run it using Valgrind.

**Using AddressSanitizer (fsanitize):**
Compile with the `-fsanitize=address -g3` flags by temporarily modifying your `Makefile`'s `CFLAGS`, then run:
```bash
make re
./codexion 5 800 200 200 200 5 10 fifo
```

**Using Valgrind:**
```bash
make
valgrind --leak-check=full --show-leak-kinds=all ./codexion 5 800 200 200 200 5 10 fifo
```
*Note: Do not use Valgrind and fsanitize at the same time.*

**Using Helgrind**

Summary Checklist for Reading Helgrind: 
1. Find the functions: Look at the at 0x...: lines to
see the two functions conflicting (e.g., check_complete
vs coder_routine). 
2. Find the memory block: Look at the Address 0x... is 
X bytes inside a block alloc'd by... to figure out 
which variable or array was malloc'd.
3. Identify the variable: Match the size of the block
and the allocation function (table_allocation) to
figure out exactly what struct or array the variable 
belongs to.
4. Check the locks: Notice that Helgrind says Locks
held: none. This confirms that the thread was accessing
the variable without having pthread_mutex_lock wrapped 
around it.

## Blocking cases handled

- **Deadlock Prevention**: Deadlocks occur if every coder simultaneously grabs their left dongle and waits indefinitely for the right dongle. This is mitigated through controlled dongle acquisition, ensuring that resources are only granted when the arbitration queue allows it.
- **Starvation Prevention**: Using FIFO (First In, First Out) and EDF (Earliest Deadline First) queues ensures that every coder eventually gets a turn to use the dongles, avoiding indefinite resource starvation.
- **Cooldown Handling**: The state of each dongle records when it was last released. A dongle remains locked or marked as unavailable until the requested `dongle_cooldown` period elapses, properly simulating hardware timeouts.
- **Precise Burnout Detection**: A dedicated monitor thread continually evaluates the time elapsed since each coder's last compile start time. It aggressively wakes up and logs if a coder crosses the `time_to_burnout` limit, fulfilling the precision requirement (<= 10ms delay).
- **Log Serialization**: The terminal output is protected by a dedicated mutex to ensure state messages from multiple competing threads do not interleave or become corrupted.

## Thread synchronization mechanisms

- **`pthread_mutex_t`**: Mutexes are the primary synchronization primitive. They are used to protect shared states such as the dongle availability (`dongle_state`), the waitlist/priority queue, the printing function (`print_mutex`), and the overall simulation status.
- **`pthread_cond_t`**: Condition variables allow coder threads to block efficiently when dongles are unavailable. Once a coder releases a dongle, a condition broadcast or signal wakes up sleeping threads so they can re-evaluate resource availability.
- **Arbiter & Heap Queue**: An arbitration lock (`arbiter`) ensures thread-safe modification of the priority queue (`t_heap`). Coders push their request onto the heap and wait on the condition variable. The scheduler determines priority based on ticket number (for FIFO) or burnout deadline (for EDF).

## Resources
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/) - Classic reference for `pthreads`.
- [Coffman's Conditions](https://en.wikipedia.org/wiki/Deadlock) - Understanding deadlocks and how to prevent them in operating systems.
- [Introduction To Threads (pthreads) | C Programming Tutorial](https://youtu.be/ldJ8WGZVXZk?si=-MsHwkjzMGNp8tnJ)
- [Mutex Introduction (pthreads) | C Programming Tutorial](https://youtu.be/raLCgPK-Igc?si=BHPHN_PRpYPoPsq0)
- [What is a mutex in C? (pthread_mutex)](https://youtu.be/oq29KUy29iQ?si=t5NtU8w999Sym8Mp)
- **AI Usage**: AI was primarily utilized during the development of this project for debugging concurrency issues, providing insights on thread-safe data structures like the priority queue (heap), and formatting this very documentation according to the project specifications.
