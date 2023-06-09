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
    
    va_start(args, base_str);
    char* arg1 = va_arg(args, char*);

    formatted_str = (char*)malloc(str_size+ 1);
    strcpy(formatted_str, base_str);
    printf("result: %s\n", formatted_str);

    if (formatted_str == NULL)
        return NULL;


    if(arg1 != NULL){
      va_end(args);
    

      va_start(args, base_str);

      vsnprintf(formatted_str, str_size, base_str, args);

      va_end(args);

      printf("result: %s\n", formatted_str);
    }
    else{
      va_end(args);
    }

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
  
  int ret;

  sqlite3_open("BD/db.sqlite3", &db);

  // funcao
  Usuario *usuarioLogin = NULL;
  

  //pegando dados do banco de dados
  

  strFOverwrite(&sql_cmd,
    "SELECT * FROM USUARIO_TB "\
    "WHERE (EMAIL = '%s'); "\

  "", email);

  ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
  sysStatus(&db, ret);
  
  if(ret == SQLITE_OK){

    ret = sqlite3_step(sql_stmt);
    sysStatus(&db, ret);
    //printf("email: %s\n", sqlite3_column_text(sql_stmt, 2));

    if(ret == SQLITE_ROW) {
      if(!strcmp(email, sqlite3_column_text(sql_stmt, 2))){
        printf("usuario encotrado.\n");

        if(!strcmp(senha, sqlite3_column_text(sql_stmt, 3))){
        


          usuarioLogin = (Usuario *)malloc(sizeof(Usuario));


          
          usuarioLogin->id = sqlite3_column_int(sql_stmt, 0);
          usuarioLogin->nome = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 1), NULL);
          usuarioLogin->email = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 2), NULL);
          usuarioLogin->senha = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 3), NULL);
          usuarioLogin->categoriaUsuario = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 4), NULL);
          
          
          usuarioLogin->tipoUsuario = NULL;

          sqlite3_finalize(sql_stmt);



          //verificando se o tipo do usuario existe
          strFOverwrite(&sql_cmd,
            "SELECT * FROM %s_TB "\
            "WHERE (USUARIO_FK = '%d'); "\

          "",usuarioLogin->categoriaUsuario, usuarioLogin->id);

          ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
          sysStatus(&db, ret);
          
          if(ret == SQLITE_OK){

            ret = sqlite3_step(sql_stmt);
            sysStatus(&db, ret);
            //printf("ID: %d\n", sqlite3_column_int(sql_stmt, 0));
            
            //verificando
            if(ret == SQLITE_ROW) {
              printf("tipo de usuario existente.\n");

              void* tipoUsuarioLogin = NULL;

              //residente
              if(!strcmp(usuarioLogin->categoriaUsuario, "residente")){
                tipoUsuarioLogin = malloc(sizeof(Residente));

                ((Residente*)tipoUsuarioLogin)->usuario = usuarioLogin;


              }else if(!strcmp(usuarioLogin->categoriaUsuario, "preceptor")){
                tipoUsuarioLogin = malloc(sizeof(Preceptor));

                ((Preceptor*)tipoUsuarioLogin)->usuario = usuarioLogin;

              }else if(!strcmp(usuarioLogin->categoriaUsuario, "coordenacao")){
                tipoUsuarioLogin = malloc(sizeof(Coordenacao));

                ((Coordenacao*)tipoUsuarioLogin)->usuario = usuarioLogin;

              }else if(!strcmp(usuarioLogin->categoriaUsuario, "gestao")){
                tipoUsuarioLogin = malloc(sizeof(Gestao));

                ((Gestao*)tipoUsuarioLogin)->usuario = usuarioLogin;
                ((Gestao*)tipoUsuarioLogin)->cargo = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 2), NULL);

              }



              
            }else{
              printf("tipo de usuario nao existente.\n");
            }
          }

          
          
          sysStatus(&db, ret);
        }else{
          printf("senha incorreta...\n");
        }
      }else{
        printf("email incorreto...\n");
      }

    }else{
      printf("email incorreto...\n");
    }
    
  }
  
  sqlite3_finalize(sql_stmt);
  
  sqlite3_close(db);
  
  
  return usuarioLogin;
};





int fazerCadastro(sqlite3** db_ptr, char *nome, char *email, char *senha, char *tipoDeUsuario) {
  // Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  sqlite3_open("BD/db.sqlite3", &db);


  // funcao
  

  //pegando dados do banco de dados
  //strFormater()


  // verificando cadastro
  strFOverwrite(&sql_cmd,
    "SELECT EMAIL FROM USUARIO_TB "\
    "WHERE (EMAIL = '%s'); "\

  "", email);

  ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
  sysStatus(&db, ret);

  if(ret == SQLITE_OK){

    ret = sqlite3_step(sql_stmt);
    sysStatus(&db, ret);
    
    // cadastro ja existente
    if(ret == SQLITE_ROW){
      printf("EMAIL:  %s\n", sqlite3_column_text(sql_stmt, 0));

      if(!strcmp(email, sqlite3_column_text(sql_stmt, 0))){

        printf("email ja cadastrado!\n");

        sqlite3_finalize(sql_stmt);
        sqlite3_close(db);
        return 1;

      }

    }
      
  
    //cadastro novo(OK)
    printf("cadastrando!\n");
    sqlite3_finalize(sql_stmt);

    
  }else{
    sqlite3_finalize(sql_stmt);
    sqlite3_close(db);
    return 1;
  }



  // criando cadastroGeral(USUARIO_TB)
  strFOverwrite(&sql_cmd,  
    "INSERT INTO USUARIO_TB (NOME,EMAIL,SENHA,TIPO) "\
    "VALUES ('%s', '%s', '%s', '%s' ); "\

  "", nome, email, senha, tipoDeUsuario);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);



  
  sqlite3_close(db);
  
  return 0;
};



int fazerGestaoTB(sqlite3** db_ptr, int usuario_fk, char *cargo) {
  // Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  sqlite3_open("BD/db.sqlite3", &db);


  // funcao

  // criando tabela gestao
  strFOverwrite(&sql_cmd,  
    "INSERT INTO GESTAO_TB (USUARIO_FK, CARGO) "\
    "VALUES ('%d', '%s'); "\

  "", usuario_fk, cargo);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);

  sqlite3_close(db);

  return 0;
}


int fazerResidenteTB(sqlite3** db_ptr, int usuario_fk, char *cargo) {
  // Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  sqlite3_open("BD/db.sqlite3", &db);


  // funcao

  // criando tabela gestao
  strFOverwrite(&sql_cmd,  
    "INSERT INTO GESTAO_TB (USUARIO_FK, CARGO) "\
    "VALUES ('%d', '%s'); "\

  "", usuario_fk, cargo);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);

  sqlite3_close(db);

  return 0;
}

