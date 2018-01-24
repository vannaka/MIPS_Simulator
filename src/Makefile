CC_GCC = gcc

CFLAGS = -Wall -g -O2 

OBJS = memory.o Instructions.o mu-mips.o
 
OBJ_LINK_LIST:= $(OBJS)

%.o: %.c
	$(CC_GCC) $(CFLAGS) -o $@ -c $< 

mu-mips: $(OBJ_LINK_LIST)
	$(CC_GCC) -o mu-mips $(OBJ_LINK_LIST) $(CFLAGS)

clean:
	rm -f mu-mips $(OBJ_LINK_LIST)

