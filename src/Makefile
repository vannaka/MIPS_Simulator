CC_GCC = gcc

CFLAGS = -Wall -g -O2 

SHARED_OBJS = Instructions.o memory.o 
SIM_OBJS = mu-mips.o $(SHARED_OBJS)
ASM_OBJS = assembler.o $(SHARED_OBJS)

%.o: %.c
	$(CC_GCC) -c -o $@ $< $(CFLAGS)

sim-mips: $(SIM_OBJS)
	$(CC_GCC) -o sim-mips $(SIM_OBJS) $(CFLAGS)

asm-mips: $(ASM_OBJS)
	$(CC_GCC) -o asm-mips $(ASM_OBJS) $(CFLAGS)

clean:
	rm -f mu-mips ass-mips $(SIM_OBJS) $(ASM_OBJS)

