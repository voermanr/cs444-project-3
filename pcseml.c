#include "pcseml.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *mutex, *free_spots, *items;
int numProducers;
int numConsumers;
int numEvents;
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

void wait_for_producers(pthread_t *producer) {
    for (int i = 0; i < numProducers; i++) {
        pthread_join(producer[i], NULL);
    }
}

void wait_for_consumers(pthread_t *consumer) {
    for (int i = 0; i < numConsumers; i++) {
        pthread_join(consumer[i], NULL);
    }
}

void *consume (void *arg) {
    int *id = arg;

    while(1) {
        sem_wait(items);

        sem_wait(mutex);

        if (eventbuf_empty(event_buffer)) {
            sem_post(mutex);
            break;
        }

        int event = eventbuf_get(event_buffer);

        printf("C%d: got event %d\n", *id, event);

        sem_post(mutex);

        sem_post(free_spots);
    }

    printf("C%d: exiting\n", *id);

    return NULL;
}

void start_consumers(pthread_t *consumer, int *consumer_id) {
    for (int i = 0; i < numConsumers; i++) {
        consumer_id[i] = i;
        pthread_create(consumer + i, NULL, consume, consumer_id + i);
    }
}

void *produce(void *arg) {
    int *id = arg;

    for (int i = 0; i < numEvents; ++i) {
        int event_number = *id * 100 + i;
        sem_wait(free_spots);

        sem_wait(mutex);

        printf("P%d: adding event %d\n", *id, event_number);

        eventbuf_add(event_buffer, event_number);

        sem_post(mutex);

        sem_post(items);
    }

    printf("P%d: exiting\n", *id);

    return NULL;
}


int start_producers(pthread_t *producer, int *producer_id) {
    for (int i = 0; i < numProducers; i++) {
        producer_id[i] = i;
        pthread_create(producer + i, NULL, produce, producer_id + i);
    }
    return numProducers;
}

int parse_command_line(char **arg) {
    numProducers = (int)strtol((arg[1]),NULL,10);
    numConsumers = (int)strtol((arg[2]), NULL, 10);
    numEvents = (int)strtol((arg[3]), NULL, 10);
    numMaximum = (int)strtol((arg[4]),NULL,10);

    return 0;
}

void validate_input(int argc) {
    if (argc != 5) {
        printf("Usage: pcseml numProducers numConsumers, eventsPerProducer, maxEvents\n");
        exit(1);
    }
}

void setup_the_semaphores_of_Gondor() {
    mutex = sem_open_temp("mutex-semaphore", 1);
    free_spots = sem_open_temp("free-spots", numMaximum);
    items = sem_open_temp("items",0);
}

void setup_producer_threads(pthread_t **producer, int **producer_id) {
    // I'm stealing this from you

    // Allocate producer handle array for all brokers
    *producer_id = calloc(numProducers, sizeof **producer_id);

    // Allocate producer ID array for all brokers
    *producer = calloc(numProducers, sizeof **producer);
}

void setup_consumer_threads(pthread_t **producer, int **consumer_id) {
    // I'm stealing this from you

    // Allocate producer handle array for all brokers
    *consumer_id = calloc(numProducers, sizeof **consumer_id);

    // Allocate producer ID array for all brokers
    *producer = calloc(numProducers, sizeof **producer);
}

void operation_black_friday() {
    for (int i = 0; i < numConsumers + 1; ++i) {
        sem_post(items);
    }
}

int main(int argc, char* argv[]) {
    validate_input(argc);

    parse_command_line(argv);

    setup_the_semaphores_of_Gondor();

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

    operation_black_friday();

    wait_for_consumers(consumer);

    eventbuf_free(event_buffer);

    return 0;
}
