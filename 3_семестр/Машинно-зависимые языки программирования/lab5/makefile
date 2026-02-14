lab5: lab5.o
	ld -o lab5 lab5.o -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2

lab5.o: lab5.asm
	nasm -f elf64 -g -F dwarf lab5.asm -o lab5.o

clean:
	rm -f lab5 lab5.o