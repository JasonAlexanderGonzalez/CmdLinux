#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>


//metodo imprime el usuario y la ruta del usuario
void mostrarPrompt(){
    char *buf;
    char *ptr;
    long size;    

    size = pathconf(".", _PC_PATH_MAX);
    if((buf = (char *) malloc((size_t)size)) != NULL){
        ptr = getcwd(buf, (size_t)size);
        struct passwd *getpwuid(uid_t uid);
        struct passwd *p;
        uid_t uid = 0;
        if((p = getpwuid(uid)) == NULL){
            perror("getpuid() error");
        } else{
            printf("%s@%s> ", p->pw_name, ptr);
            sleep(2);
        }
        free(buf);
    }
}

//Limpieza final , espera ha que termine el proceso
// el parametro hace referencia a las veces que el comando fue invocado
void limpieza(int n)
{
	int i;
	for (i = 0; i < n; ++i) 
		wait(NULL); 
}

