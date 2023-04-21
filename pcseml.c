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

int wait_for_producers(pthread_t *producer) {
    for (int i = 0; i < numProducers; i++) {
        pthread_join(producer[i], NULL);
    }
    return numProducers;
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

void *produce(void *arg) {
    int *id = arg;

    for (int i = 0; i < events; ++i) {
        //TODO: Wait to see if there's enough space in the event buffer to post.
        sem_wait(free_spots);

        //TODO: Lock a mutex around the eventbuf.
        sem_wait(mutex);

        //TODO: Print ;that it's adding the event, along with the event number.
        printf("P%d: adding event %d\n", *id, i);

        //TODO: Add an event to the eventbuf.
        eventbuf_add(event_buffer, i);

        //TODO: Unlock the mutex.
        sem_post(mutex);

        //TODO: Signal waiting consumer threads that there is an event to be consumed.
        sem_post(free_spots);
    }

    printf("P%d: exiting\n", *id);

    return NULL;
}

void *consume (void *arg) {
    //TODO: make better
    return NULL;
}

int start_producers(pthread_t *producer, int *producer_id) {
    for (int i = 0; i < numProducers; i++) {
        producer_id[i] = i;
        pthread_create(producer + i, NULL, produce, producer_id + i);
    }
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

void setup_producer_threads(pthread_t **producer, int **producer_id) {
    // I'm stealing this from you

    // Allocate producer handle array for all brokers
    *producer_id = calloc(numProducers, sizeof *producer_id);

    // Allocate producer ID array for all brokers
    *producer = calloc(numProducers, sizeof *producer);
}

void setup_consumer_threads(pthread_t **producer, int **consumer_id) {
    // I'm stealing this from you

    // Allocate producer handle array for all brokers
    consumer_id = calloc(numProducers, sizeof *consumer_id);

    // Allocate producer ID array for all brokers
    producer = calloc(numProducers, sizeof *producer);
}

int main(int argc, char* argv[]) {
    validate_input(argc);

    int numProducers, consumers, events, numMaximum;

    parse_command_line(argv, &numProducers, &consumers, &events, &numMaximum);

    mutex = sem_open_temp("mutex-semaphore",1);
    free_spots = sem_open_temp("free-spots", numMaximum);
    items = sem_open_temp("items",0);


    event_buffer = eventbuf_create();

    pthread_t *producer;
    int *producer_id;
    setup_producer_threads(&producer, &producer_id);
    start_producers(producer, producer_id);

    pthread_t *consumer;
    int *consumer_id;
    setup_consumer_threads(&consumer, &consumer_id);
    start_consumers(consumer, consumer_id);

    wait_for_producers(producer);

    notify_consumers();

    wait_for_consumers();

    eventbuf_free(event_buffer);

    return 0;
}
