# 42_Philosophers

## Description
This project involves simulating the dining philosophers problem. Philosophers, represented as threads or processes, sit at a round table, where they alternate between eating, thinking, and sleeping. The goal is to prevent deadlocks and starvation while ensuring that each philosopher can eat without conflict.

## Structure
- **philo**: Program simulating philosophers using threads and mutex.

## Mandatory Part
### philo
- Simulates philosophers using threads and mutex.
- Each philosopher is a thread.
- One fork between each philosopher, protected by a mutex.
- Philosophers must follow the specified rules: eating, thinking, or sleeping without conflicts.
- Program arguments: `number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]`.

## Usage
Both programs share the same usage format:
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]

## Compilation
Run `make` in the respective directories to compile each program.

## External Functions
Both programs use standard C functions, including:

- **memset**: Fills a block of memory with a specified value.
- **printf**: Prints formatted output to the standard output.
- **malloc**: Dynamically allocates memory.
- **free**: Deallocates memory.
- **write**: Writes data to a file descriptor.
- **usleep**: Suspends execution of the calling thread for a specified number of microseconds.
- **gettimeofday**: Gets the current time.
- **pthread_create**: Creates a new thread.
- **pthread_detach**: Detaches a thread.
- **pthread_join**: Waits for a thread to terminate.
- **pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock**: Functions for thread synchronization using mutexes.

## Norm Compliance
Ensure compliance with the project's norms regarding leaks, crashes, undefined behavior, and coding standards.

## Note
Philosophers may seem peaceful, but in this simulation, philosophy can be deadly!

## Resources

[Medium article](https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2) - Philosophers 42 guide;<br>
[Youtube video](https://www.youtube.com/watch?v=UGQsvVKwe90&t=1788s&ab_channel=JamshidbekErgashev) by
Jamshidbek Ergashev; <br>
[Video tutorials on threads](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&ab_channel=CodeVault) - CodeVault;<br>
