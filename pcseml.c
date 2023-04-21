#include "pcseml.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/semaphore.h>
#include <fcntl.h>

sem_t *mutex, *free_spots, *items;

sem_t *sem_open_temp(const char *name, int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

int notify_consumers() {
    return 0;
}

int wait_for_producers() {
    return 0;
}

int wait_for_consumers() {
    return 0;
}

int start_consumers(int numConsumers) {
    return numConsumers;
}

int start_producers(int numProducers) {
    return numProducers;
}

int parse_command_line(char** arg, int *producers, int *consumers, int *events, int *maximum) {
    *producers = (int)strtol((arg[1]),NULL,10);
    *consumers = (int)strtol((arg[2]),NULL,10);
    *events = (int)strtol((arg[3]),NULL,10);
    *maximum = (int)strtol((arg[4]),NULL,10);

    return 0;
}

void validate_input(int argc) {
    if (argc != 5) {
        printf("Usage: pcseml numProducers numConsumers, eventsPerProducer, maxEvents\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    validate_input(argc);

    int producers, consumers, events, maximum;

    parse_command_line(argv, &producers, &consumers, &events, &maximum);

    event_buffer = eventbuf_create();

    start_producers(producers);

    start_consumers(consumers);

    wait_for_producers();

    notify_consumers();

    wait_for_consumers();

    eventbuf_free(event_buffer);

    return 0;
}
