# Where is the AllJoyn Thin Client installed
AJTCL=$(HOME)/Projects/AllJoyn/ajtcl
INCLUDES=-I$(AJTCL)/inc -I$(AJTCL)/target/darwin 

# Compiler
CC=gcc

# Compiler Flags
CFLAGS=-c -Wall $(INCLUDES)

# Link Flags
LDFLAGS= -L$(AJTCL) -framework Security

# Libraries to link against
LIBS= -lajtcl_st

all: model1_service model1_client

model1_service: model1_service.o
	$(CC) $(LDFLAGS) model1_service.o -o $@ $(LIBS)

model1_client: model1_client.o
	$(CC) $(LDFLAGS) model1_client.o -o $@ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o model1_service model1_client

