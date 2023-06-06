#include "utils.h"







// Funções:


// -Gerais:

char* strFOverwrite(char** output_str, char* base_str, ...){
  char* formatted_str = NULL;
  int str_size = 0;

  if(base_str != NULL){

    va_list args;
    va_start(args, base_str);

    str_size = vsnprintf(NULL, 0, base_str, args);
    //printf("size: %d\n", str_size);

    va_end(args);
    

    formatted_str = (char*)malloc(str_size+ 1);
    strcpy(formatted_str, base_str);

    if (formatted_str == NULL)
        return NULL;


    va_start(args, base_str);

    vsnprintf(formatted_str, str_size, base_str, args);

    va_end(args);

    printf("result: %s\n", formatted_str);

    if(output_str != NULL){

      if(*output_str != NULL){
        free(*output_str);
      }

      *output_str = formatted_str;
    }
  }

  return formatted_str;
}


int sysStatus(sqlite3** db_ptr, int ret){
  sqlite3* db = *db_ptr;

  printf("\nCODE: %d\n", ret);

  if(ret){
    fprintf(stderr, "nao foi possivel acessar o banco de dados, \n ERRO: %s\n\n", sqlite3_errmsg(db));
    return 1;
  } 
  else{
    fprintf(stderr, "banco de dados acessado.\n STATUS: %s\n\n", sqlite3_errmsg(db));
    return 0;
  }
}


Usuario *fazerLogin(sqlite3** db_ptr, char *email, char *senha) {
  // Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  char* fdb_msg = NULL;
  
  int ret;

  sqlite3_open("BD/db.sqlite3", &db);

  // funcao
  Usuario *usuarioLogin = NULL;
  

  //pegando dados do banco de dados
  //strFormater()

  strFOverwrite(&sql_cmd,
    "SELECT * FROM USUARIO_TB "\
    "WHERE (EMAIL = '%s'); "\

  "", email);

  ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
  sysStatus(&db, ret);

  if(ret == SQLITE_OK){

    ret = sqlite3_step(sql_stmt);
    sysStatus(&db, ret);


    if(!strcmp(email, sqlite3_column_text(sql_stmt, 2))){
      printf("usuario encotrado.\n");

      if(!strcmp(senha, sqlite3_column_text(sql_stmt, 3))){
      


        usuarioLogin = (Usuario *)malloc(sizeof(Usuario));


        
        usuarioLogin->id = sqlite3_column_int(sql_stmt, 0);
        usuarioLogin->nome = sqlite3_column_text(sql_stmt, 1);
        usuarioLogin->email = sqlite3_column_text(sql_stmt, 2);
        usuarioLogin->senha = sqlite3_column_text(sql_stmt, 3);
        usuarioLogin->tipoDeUsuario = sqlite3_column_text(sql_stmt, 4);



        
        sysStatus(&db, ret);
      }else{
        printf("senha incorreta...\n");
      }
    }else{
      printf("email incorreto...\n");
    }

    
  }

  sqlite3_close(db);
  
  return usuarioLogin;
};