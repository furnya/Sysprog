BIN = mp_hashcrack
SRC = \
	sha256.c		\
	utils.c			\
	bruteforce.c	\
	main.c

CC	     = cc
CFLAGS   = --std=c99 -g -Wall -O2 -D_GNU_SOURCE

CXX	     = g++
CXXFLAGS = -g

GDB    = gdb
RM     = rm -f

ALL_FILES = $(SRC:%.c=%.o)

all: $(BIN)

$(BIN): $(ALL_FILES)
	$(CXX) $(ALL_FILES) -o $@ 


# pull in dependencies for .o files
-include $(ALL_FILES:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
	$(CC) -MM $(CFLAGS) $*.c > $*.d

debug: $(BIN)
	$(GDB) ./$(BIN)

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(ALL_FILES) *.d

.PHONY: clean debug run 
