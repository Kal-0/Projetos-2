#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
  char nome[60];
  int id;
  char* email;
  char* senha;
  char* tipoDeUsuario;
  void** categoriaUsuario;
  int status;
  
}Usuario;



int main(void) {

  Usuario* decoyUser1 = NULL;
  Usuario* decoyUser2 = NULL;
  

  decoyUser1 = (Usuario*)malloc(sizeof(Usuario));
  decoyUser2 = (Usuario*)malloc(sizeof(Usuario));
  
  decoyUser1->categoriaUsuario = (void**)&decoyUser2;

  
  printf("Hello World\n");

  return 0;
}