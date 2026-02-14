all:
	gcc -c -g -ggdb -m32 -o main.o main.c
	nasm -f elf32 -g -o asm_8bit.o asm_8bit.asm
	nasm -f elf32 -g -o asm_16bit.o asm_16bit.asm
	gcc -m32 -no-pie -o lab1 main.o asm_8bit.o asm_16bit.o -g -ggdb

clean:
	rm -f *.o lab1