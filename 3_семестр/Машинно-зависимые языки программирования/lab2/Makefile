CC = gcc
NASM = nasm
CFLAGS = -g -ggdb -no-pie -m32 -Wall
NASMFLAGS = -f elf32 -g -F dwarf

all: lab2
lab2: main.o asm_signed.o asm_unsigned.o
	$(CC) $(CFLAGS) main.o asm_signed.o asm_unsigned.o -o lab2

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

asm_signed.o: asm_signed.asm
	$(NASM) $(NASMFLAGS) asm_signed.asm -o asm_signed.o

asm_unsigned.o: asm_unsigned.asm
	$(NASM) $(NASMFLAGS) asm_unsigned.asm -o asm_unsigned.o

clean:
	rm -f *.o lab2