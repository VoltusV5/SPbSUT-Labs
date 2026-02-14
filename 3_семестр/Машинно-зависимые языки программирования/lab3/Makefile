all:
	nasm -f elf64 -F dwarf -g calc16.asm -o calc16.o        
	nasm -f elf64 -F dwarf -g calc16s.asm -o calc16s.o      
	nasm -f elf64 -F dwarf -g main.asm -o main.o           
	gcc -m64 -o lab3 main.o calc16.o calc16s.o -g -ggdb -no-pie -z noexecstack 

clean:
	rm -f *.o lab3