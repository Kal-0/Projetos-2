#include "utils.h"







// Funções:


// -Gerais:

char* strFOverwrite(char** output_str, char* base_str, char* variable_str){
  char* formatted_str = base_str;
  if(base_str != NULL && variable_str != NULL){
    //printf("nnulo\n");

    formatted_str = (char*)malloc(strlen(base_str) + strlen(variable_str) + 1);
    sprintf(formatted_str, base_str, variable_str);
  }

  //printf("result: %s\n", formatted_str);

  if(output_str != NULL){

    if(*output_str != NULL){
      free(*output_str);
    }

    *output_str = formatted_str;
  }

  return formatted_str;
}

void strOverwrite(char** oldString, char* newString){
  if(*oldString != NULL){
    free(*oldString);
  }
  *oldString = NULL;

  *oldString = (char*)malloc(strlen(newString) + 1);
  strcpy(*oldString, newString);
}


int sysStatus(sqlite3** db_ptr, int ret){
  sqlite3* db = *db_ptr;
  if(ret){
    fprintf(stderr, "\n\nnao foi possivel acessar o banco de dados, \n ERRO: %s\n\n", sqlite3_errmsg(db));
    return 1;
  } 
  else{
    fprintf(stderr, "\n\nbanco de dados acessado.\n STATUS: %s\n\n", sqlite3_errmsg(db));
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

  // funcao
  Usuario *usuarioLogin = NULL;
  

  //pegando dados do banco de dados
  //strFormater()

  strOverwrite(&sql_cmd,
    "SELECT * FROM USUARIO_TB "\
    "WHERE (EMAIL = %s); "\

  "");

  ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
  sysStatus(&db, ret);

  if(ret){
    usuarioLogin = (Usuario *)malloc(sizeof(Usuario));


    ret = sqlite3_step(sql_stmt);
    sysStatus(&db, ret);
    
    if(ret == SQLITE_ROW) {
      printf("%s\n", sqlite3_column_text(sql_stmt, 1));
    }
  }
  
  return usuarioLogin;
};