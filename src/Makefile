PLATFORM=$(shell uname)
CC = gcc
AR = ar

SHARED_LIB = ltrace.so
OBJS = ltrace.o

LLIBS = 
CFLAGS = -c -O3 -Wall -fPIC -Werror=declaration-after-statement -std=c89
LDFLAGS = -O3 -Wall --shared

ifeq ($(PLATFORM),Linux)
else
	ifeq ($(PLATFORM), Darwin)
		LLIBS += -dynamiclib -Wl,-undefined,dynamic_lookup
	endif
endif

all : $(SHARED_LIB)

$(SHARED_LIB): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LLIBS)

$(OBJS) : %.o : %.c
	$(CC) -o $@ $(CFLAGS) $<

clean : 
	rm -f $(OBJS) $(SHARED_LIB)

.PHONY : clean

