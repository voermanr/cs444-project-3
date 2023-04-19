#ifndef PCSEML_H
#define PCSEML_H

#include "eventbuf.h"

int parse_command_line(char** arg, int *p, int *c, int *e, int *m);

int start_producers(int producers);
int start_consumers(int producers);
int wait_for_producers();
int notify_consumers();
int wait_for_consumers();

#endif