CC = gcc

# Red Pitaya API paths
RP_INCLUDE = /opt/redpitaya/include
RP_LIB     = /opt/redpitaya/lib

CFLAGS  = -I$(RP_INCLUDE)
LDFLAGS = -L$(RP_LIB) -lrp

# All .c files in the folder
SRCS = $(wildcard *.c)

# Executables: strip `.c`
EXES = $(SRCS:.c=)

all: $(EXES)

# Pattern rule: build each .c into its own executable
%: %.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

# Run specific program:
# Example: make run NAME=blinktest
run:
	
	LD_LIBRARY_PATH=$(RP_LIB):$$LD_LIBRARY_PATH ./$(NAME)	

clean:
	rm -f $(EXES)
