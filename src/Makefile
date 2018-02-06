CC_GCC = gcc

CFLAGS = -Wall -g -O2 

SIM_OBJS = mu-mips.o
ASS_OBJS = assembler.o
SHARED_OBJS = Instructions.o memory.o 

%.o: %.c
	$(CC_GCC) $(CFLAGS) -o $@ -c $< 

mu-mips: $(SIM_OBJS)
	$(CC_GCC) -o mu-mips $(SIM_OBJS) $(SHARED_OBJS) $(CFLAGS)

ass-mips: $(ASS_OBJS)
	$(CC_GCC) -o ass-mips $(ASS_OBJS) $(SHARED_OBJS) $(CFLAGS)

clean:
	rm -f mu-mips $(OBJ_LINK_LIST)

