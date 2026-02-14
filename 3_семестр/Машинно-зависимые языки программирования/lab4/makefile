all:
	nasm -f elf64 -F dwarf -g main.asm -o mytest.o
	gcc -m64 -o app mytest.o -g -ggdb -no-pie -z noexecstack
