#include "utils.h"

//printf("===DEBUG===\n");





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

//banco de dados
int sysStatus(sqlite3** db_ptr, int ret){
  //banco de dados
  sqlite3* db = *db_ptr;

  //funcao
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

int getStmt(sqlite3** db_ptr, sqlite3_stmt** sql_stmt_ptr, char* sql_cmd_p){
  // Banco de dados
  sqlite3* db = *db_ptr;
  char* sql_cmd = sql_cmd_p;
  sqlite3_stmt* sql_stmt = NULL;

  int ret;

  sqlite3_open("BD/db.sqlite3", &db);


  //funcao

  if(*sql_stmt_ptr != NULL){
    sqlite3_finalize(*sql_stmt_ptr);
    *sql_stmt_ptr = NULL;
  }

  ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
  sysStatus(&db, ret);
  
  if(ret == SQLITE_OK){
    
    ret = sqlite3_step(sql_stmt);
    sysStatus(&db, ret);


    if(ret == SQLITE_ROW){

      *sql_stmt_ptr = sql_stmt;

      sqlite3_close(db);
      return ret;
    }else{

      sqlite3_finalize(sql_stmt);
      sqlite3_close(db);
      return ret;
    }


  }else{
    //printf("===DEBUG===\n");
    sqlite3_close(db);
    return ret;
  }
}



//login
Usuario *getUsuarioTB(sqlite3** db_ptr, char *email, char *senha) {
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

  
  
  
  ret = getStmt(&db, &sql_stmt, sql_cmd);
  //printf("email: %s\n", sqlite3_column_text(sql_stmt, 2));

  if(ret == SQLITE_ROW){
    printf("usuario encotrado.\n");

    if(!strcmp(email, sqlite3_column_text(sql_stmt, 2))){
      

      if(!strcmp(senha, sqlite3_column_text(sql_stmt, 3))){
      


        usuarioLogin = (Usuario *)malloc(sizeof(Usuario));


        
        usuarioLogin->id = sqlite3_column_int(sql_stmt, 0);
        usuarioLogin->nome = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 1), NULL);
        usuarioLogin->email = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 2), NULL);
        usuarioLogin->senha = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 3), NULL);
        usuarioLogin->categoriaUsuario = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 4), NULL);
        
        
        usuarioLogin->tipoUsuario = NULL;

        
        //pegando tipo de usuario
        


        //verificando se o tipo do usuario existe
        strFOverwrite(&sql_cmd,
          "SELECT * FROM %s_TB "\
          "WHERE (USUARIO_FK = '%d'); "\

        "",usuarioLogin->categoriaUsuario, usuarioLogin->id);



        ret = getStmt(&db, &sql_stmt, sql_cmd);
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
          printf("tipo de usuario ainda nao criado.\n");

        }
        

        
      }else{
        printf("senha incorreta...\n");
      }
    }else{
      printf("email incorreto...\n");
    }

  }
    
  printf("===DEBUG===\n");
  
  sqlite3_finalize(sql_stmt);
  

  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  sqlite3_close(db);
  
  
  return usuarioLogin;
};




//cadastro de usuarios
int addUsuarioTB(sqlite3** db_ptr, char *nome, char *email, char *senha, char *tipoDeUsuario) {
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

  ret = getStmt(&db, &sql_stmt, sql_cmd);
    
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

  
  



  // criando cadastroGeral(USUARIO_TB)
  strFOverwrite(&sql_cmd,  
    "INSERT INTO USUARIO_TB (NOME,EMAIL,SENHA,TIPO) "\
    "VALUES ('%s', '%s', '%s', '%s' ); "\

  "", nome, email, senha, tipoDeUsuario);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  
  sqlite3_finalize(sql_stmt);
  sqlite3_close(db);
  
  return 0;
};

int addGestaoTB(sqlite3** db_ptr, int usuario_fk, char *cargo) {
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


  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  sqlite3_close(db);

  return 0;
}

int addCoordenacaoTB(sqlite3** db_ptr, int usuario_fk, char *cargo, int residencia_fk) {
  // Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  sqlite3_open("BD/db.sqlite3", &db);


  // funcao

  // criando tabela gestao
  strFOverwrite(&sql_cmd,  
    "INSERT INTO COORDENACAO_TB (USUARIO_FK, CARGO, RESIDENCIA_FK) "\
    "VALUES ('%d', '%s', '%d'); "\

  "", usuario_fk, cargo, residencia_fk);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  sqlite3_close(db);

  return 0;
}

int addPreceptorTB(sqlite3** db_ptr, int usuario_fk, int turma_fk) {
  // Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  sqlite3_open("BD/db.sqlite3", &db);


  // funcao

  // criando tabela gestao
  strFOverwrite(&sql_cmd,  
    "INSERT INTO PRECEPTOR_TB (USUARIO_FK, TURMA_FK) "\
    "VALUES ('%d', '%s'); "\

  "", usuario_fk, turma_fk);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  sqlite3_close(db);

  return 0;
}

int addResidenteTB(sqlite3** db_ptr, int usuario_fk, char *matricula, int turma_fk, int preceptor_fk, char* notas) {
  // Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  sqlite3_open("BD/db.sqlite3", &db);


  // funcao

  // criando tabela gestao
  strFOverwrite(&sql_cmd,  
    "INSERT INTO RESIDENTE_TB (USUARIO_FK, MATRICULA, TURMA_FK, PRECEPTOR_FK, NOTAS) "\
    "VALUES ('%d', '%s', '%d', '%d', '%s'); "\

  "", usuario_fk, matricula, turma_fk, preceptor_fk, notas);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  sqlite3_close(db);

  return 0;
}



//adicionado estruturas
int addResidenciaTB(sqlite3** db_ptr, char* nome){
// Banco de dados
  sqlite3* db = *db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  sqlite3_open("BD/db.sqlite3", &db);


  // funcao

  // criando tabela residencia
  strFOverwrite(&sql_cmd,  
    "INSERT INTO RESIDENCIA_TB (NOME) "\
    "VALUES ('%s'); "\

  "", nome);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(&db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  sqlite3_close(db);

  return 0;
}


