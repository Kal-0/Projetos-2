typedef struct{
  char nome[60];
  int id;
  char* email;
  char* senha;
  char* tipoDeUsuario;
  void* categoriaUsuario;
  int status;
  
}Usuario;