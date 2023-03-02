
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include "metodos.h"
#define READ  0
#define WRITE 1
 
 //este vector guardara los argumentos: los comandos
 char* args[450];

//creamos un proceso pid
pid_t pid;

//los comandos los pasaremos por tuberias
int command_pipe[2];
 
 // vector para el main
 char linea[1024];

// el numero de llamadas que se realizara por comando
int n = 0; 


//metodos
int ejecutar(char* cmd, int input, int first, int last);
static void separacion(char* cmd); //aqui
char* espacio_blanco(char* s);
int comando(int input, int first, int last);
void limpieza(int n);

//para trabajar el cd
char *getcwd(char *buf, size_t size);//muestra el path del folder actual



 //me esta aceptando 3 parametros, entonces puede hacer pipes de 2 tuberias

 int comando(int input, int first, int last)
{
	int pipetas[2]; 
 
	//tuberias
	pipe( pipetas );	
	pid = fork(); 
	
 
	if (pid == 0) {
		if (first == 1 && last == 0 && input == 0) { 
			// primer comando
			dup2( pipetas[WRITE], STDOUT_FILENO );  
		} else if (first == 0 && last == 0 && input != 0) {
			// comando del medio
			dup2(input, STDIN_FILENO);  
			dup2(pipetas[WRITE], STDOUT_FILENO);  
		} else {
			// ultimo comando
			dup2( input, STDIN_FILENO );  
		}
 
		if (execvp( args[0], args) == -1)
			_exit(EXIT_FAILURE); // En caso que el hijo falle
	}
 
	if (input != 0)   
		close(input);  
 
	// cierran los pipes
	close(pipetas[WRITE]);  
 
	// se cierra la lectura al terminar el ultimo comando
	if (last == 1)
		close(pipetas[READ]);
 
	return pipetas[READ];  
}

 
int main()
{
	
	while (1) {		
	
		mostrarPrompt();
		fflush(NULL);    
		
 
		//lee toda una linea de comando
		if (!fgets(linea, 1024, stdin))   //aqui
			return 0;
 
		int input = 0;
		int first = 1;
 
		char* cmd = linea;  //aqui
		char* next = strchr(cmd, '|'); // Encuentra el primer pipe '|' 
 
		while (next != NULL) {
			//'apunta al pipe '|' 
			*next = '\0';
			input = ejecutar(cmd, input, first, 0); 
 
			cmd = next + 1;
			next = strchr(cmd, '|'); // encuentra el otro pipe  '|' 
			first = 0;
		}
		
		input = ejecutar(cmd, input, first, 1); 
		limpieza(n); 
		n = 0;
	}
	return 0;
}
 

 //metodo ejecuta los comandos
 int ejecutar(char* cmd, int input, int first, int last) 
{
	separacion(cmd);           
	if (args[0] != NULL) {
		if (strcmp(args[0], "exit") == 0) 
			exit(0);
		n += 1;
		return comando(input, first, last); 
	}
	return 0;
}
 
//metodo valida los espacios en blanco
 char* espacio_blanco(char* s)
{
	while (isspace(*s)) ++s;
	return s;
}

 //metodo valida las separaciones
 void separacion(char* cmd)   
{
	cmd = espacio_blanco(cmd);  
	char* next = strchr(cmd, ' ');
	int i = 0;
 
	while(next != NULL) {
		next[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = espacio_blanco(next + 1); 
		next = strchr(cmd, ' ');
	}
 
	if (cmd[0] != '\0') {
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		++i; 
	}
 
	args[i] = NULL;
}

