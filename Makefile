# A simple makefile for building a program composed of C source files.
# Refer to CS107 guide to Make for background info on makefiles

PROGRAMS = heap
all:: $(PROGRAMS)

CC = gcc
CFLAGS = -g -O0 -std=gnu99 -Wall $$warnflags
export warnflags = -Wfloat-equal -Wtype-limits -Wpointer-arith -Wlogical-op -Wshadow -fno-diagnostics-show-option
LDFLAGS =
LDLIBS = -lm

$(PROGRAMS): %:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

code: CFLAGS += -w

clean::
	rm -f $(PROGRAMS) *.o

.PHONY: clean all
