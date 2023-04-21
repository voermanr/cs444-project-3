#include "pcseml.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/semaphore.h>
#include <fcntl.h>

sem_t *mutex, *free_spots, *items;
int numProducers;
int consumers;
int events;
int numMaximum;

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
    //TODO: beef em up
    return 0;
}

int wait_for_producers() {
    return 0;
}

int wait_for_consumers() {
    //TODO: add new flesh
    return 0;
}

void start_consumers(pthread_t *consumer, int *consumer_id) {
    //TODO: add more meat to the bones
    for (int i = 0; i < numProducers; i++) {
        consumer_id[i] = i;
        pthread_create(consumer + i, NULL, consume, consumer_id + i);
    }
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

void *produce() {
    //TODO: Wait to see if there's enough space in the event buffer to post.
    //TODO: Lock a mutex around the eventbuf.
    //TODO: Print that it's adding the event, along with the event number.
    //TODO: Add an event to the eventbuf.
    //TODO: Unlock the mutex.
    //TODO: Signal waiting consumer threads that there is an event to be consumed.

    return NULL;
}

int main(int argc, char* argv[]) {
    validate_input(argc);

    int numProducers, consumers, events, numMaximum;

    parse_command_line(argv, &numProducers, &consumers, &events, &numMaximum);

    mutex = sem_open_temp("mutex-semaphore",1);
    free_spots = sem_open_temp("free-spots", numMaximum);
    items = sem_open_temp("items",0);


    event_buffer = eventbuf_create();



    start_producers(numProducers);

    for (int i = 0; i < numProducers; i++) {
        int *thread_id = calloc(numProducers, sizeof *thread_id);
        pthread_t *thread = calloc(numProducers, sizeof *thread);

    pthread_t *consumer;
    int *consumer_id;
    setup_consumer_threads(&consumer, &consumer_id);
    start_consumers(consumer, consumer_id);

    start_consumers(consumers);

    wait_for_producers();

    notify_consumers();

    wait_for_consumers();

    eventbuf_free(event_buffer);

    return 0;
}
