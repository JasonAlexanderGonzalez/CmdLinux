consola : shell.o metodos.o 
	@gcc  shell.o metodos.o -o consola

metodos.o : metodos.c
	@gcc -c metodos.c


shell.o : shell.c metodos.h
	@gcc -c shell.c metodos.h