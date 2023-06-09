#include "utils.h"

//printf("===DEBUG===\n");
int ACTIVE = 0;




// Funções:

//(settar variaveis "SWITCH" para "1" para debugar o codigo)
// -Gerais:

char* strFOverwrite(char** output_str, char* base_str, ...){
  char* formatted_str = NULL;
  int str_size = 0;

  int SWITCH = 0;

  if(base_str != NULL){
    

    
    va_list args;
    va_start(args, base_str);
    
    //pegando tamanho total da string que sera formada
    str_size = vsnprintf(NULL, 0, base_str, args) + 1;
   
    //printf("size: %d\n", str_size);

    va_end(args);
    
    

    //allocando memoria
    formatted_str = (char*)malloc(str_size);
    
    strncpy(formatted_str, base_str, str_size);
  
    if(SWITCH){
      printf("NF_result: %s\n", formatted_str);
    }

    if (formatted_str == NULL){
      return NULL;
    }


    va_start(args, base_str);
    char* arg1 = va_arg(args, char*);
   
    //verifica se o primeiro args e NULL
    if(arg1 != (void*)NULL || (int)arg1 == 0){
      va_end(args);
    

      va_start(args, base_str);
      
      //substituindo: "%x"s pelos args
      vsnprintf(formatted_str, str_size, base_str, args);

      va_end(args);

      if(SWITCH){
        printf("F_result: %s\n", formatted_str);
      }
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
  //printf("FF_result: %s\n", formatted_str);
  return formatted_str;
}



//listas
void append(lsID **head, int item) {
  lsID *newItem = (lsID *)malloc(sizeof(lsID));
  newItem->id = item;
  newItem->next = NULL;
  newItem->last = NULL;
  
  if (*head == NULL) {
    *head = newItem;
    
  } else {
    lsID *temp = *head;
    while (temp->next != NULL) {
      temp = temp->next;
    }

    temp->next = newItem;
    temp->next->last = temp;
  }
}

int getItemLs(lsID** head, int index){
  lsID *temp = *head;
  int cursor = 0;

  if(abs(index)>lenLs(head)){
    return 0;
  }

  if(index >= 0){
    while (temp != NULL) {
      if(cursor == index){
        return temp->id;
      }
      cursor++;
      temp = temp->next;
    }

  }else{
    while (temp->next != NULL) {
      temp = temp->next;
    }

    cursor = -1;
    while (temp != NULL) {
      if(cursor == index){
        return temp->id;
      }
      cursor--;
      temp = temp->last;
    }
    
  }
}

int lenLs(lsID **head) {
  lsID *temp = *head;
  int length = 0;

  while (temp != NULL) {
    length++;
    temp = temp->next;
  }
  return length;
}

void printLs(lsID **head) {
  lsID *temp = *head;
  while (temp != NULL) {
    printf("%d\n", temp->id);
    temp = temp->next;
  }
}

void freeLs(lsID **head) {
  lsID *temp = *head;
  lsID *aux = NULL;
  if (*head == NULL) {
    return;
  } else {

    while (temp != NULL) {
      aux = temp;
      temp = temp->next;
      free(aux);
    }

    *head = NULL;
  }
}



//banco de dados
int sysStatus(sqlite3* db_ptr, int ret){
  //banco de dados
  sqlite3* db = db_ptr;

  //funcao
  int SWITCH = 0;

  if(SWITCH){
    printf("\nCODE: %d\n", ret);

    if(ret){
      fprintf(stderr, "ERRO: %s\n\n", sqlite3_errmsg(db));
      return 1;
    } 
    else{
      fprintf(stderr, "STATUS: %s\n\n", sqlite3_errmsg(db));
      return 0;
    }
  }else{

    return 0;
  }
}

int getStmt(sqlite3* db_ptr, sqlite3_stmt** sql_stmt_ptr, char* sql_cmd_p){
  // Banco de dados
  sqlite3* db = db_ptr;
  char* sql_cmd = sql_cmd_p;
  sqlite3_stmt* sql_stmt = NULL;

  int ret;

  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  //funcao

  if(*sql_stmt_ptr != NULL){
    sqlite3_finalize(*sql_stmt_ptr);
    *sql_stmt_ptr = NULL;
  }

  ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
  sysStatus(db, ret);
  
  if(ret == SQLITE_OK){
    
    ret = sqlite3_step(sql_stmt);
    sysStatus(db, ret);


    if(ret == SQLITE_ROW){

      *sql_stmt_ptr = sql_stmt;

      if(ACTIVE){
        sqlite3_close(db);
      }
      return ret;
    }else{

      sqlite3_finalize(sql_stmt);
      if(ACTIVE){
        sqlite3_close(db);
      }
      return ret;
    }


  }else{
    //printf("===DEBUG===\n");
    if(ACTIVE){
      sqlite3_close(db);
    }
    return ret;
  }
}

void* getCellVoid(sqlite3* db_ptr, int* cell_size, char* type, char* tableName, char* field, char* condition){
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;

  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }

  // funcao

  void* cellValue = NULL;

  strFOverwrite(&sql_cmd,  
    "SELECT %s FROM %s "\
    "WHERE (%s); "\
  "", field, tableName, condition);
  sysStatus(db, ret);

  ret = getStmt(db, &sql_stmt, sql_cmd);

  
  sysStatus(db, ret);
  if(ret != SQLITE_ROW){
    

    sqlite3_finalize(sql_stmt);

    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    if(ACTIVE){
      sqlite3_close(db);
    }

    return cellValue;
  }
  
  
  
  int cell_bytes = sqlite3_column_bytes(sql_stmt, 0);
  if(cell_size != NULL){
    *cell_size = cell_bytes;
  }

  printf("=============\n");
  //printf("===DEBUG===: %d\n", cell_bytes);

  //printf("===DEBUG===: %s\n", (char*)sqlite3_column_blob(sql_stmt, 0));
  //printf("===DEBUG===: %d\n", (*(int*)sqlite3_column_blob(sql_stmt, 0)));

  cellValue = malloc(cell_bytes);
  //memcpy(cellValue, (void*)sqlite3_column_blob(sql_stmt, 0), cell_bytes);

  //printf("===DEBUG===: %s\n", (char*)cellValue);

  if(!strcmp(type, "int")){
    *((int*)cellValue) = (sqlite3_column_int(sql_stmt, 0));
  }
  else  if(!strcmp(type, "text")){
    cellValue = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 0), NULL);
  }
  else  if(!strcmp(type, "blob")){
    cellValue = (void*)sqlite3_column_blob(sql_stmt, 0);
  }


  sqlite3_finalize(sql_stmt);
  sql_stmt = NULL;

  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  if(ACTIVE){
    sqlite3_close(db);
  }

  

  return cellValue;

}

lsID* getTableIDLs(sqlite3* db_ptr, char* tableName, char* condition){
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao

  lsID* list = NULL;

  strFOverwrite(&sql_cmd,  
    "SELECT ID FROM %s "\
    "WHERE (%s); "\
  "", tableName, condition);

  ret = getStmt(db, &sql_stmt, sql_cmd);
  if(ret != SQLITE_ROW){
    

    sqlite3_finalize(sql_stmt);

    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    if(ACTIVE){
      sqlite3_close(db);
    }

    return list;
  }
  
  

  while(ret == SQLITE_ROW){
    //printf("===DEBUG===: %d\n", sqlite3_column_int(sql_stmt, 0));

    append(&list, sqlite3_column_int(sql_stmt, 0));

    ret = sqlite3_step(sql_stmt);
  }
  
  sqlite3_finalize(sql_stmt);
  sql_stmt = NULL;

  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  if(ACTIVE){
    sqlite3_close(db);
  }

  return list;
  
}

int printTableColumn(sqlite3* db_ptr, char* tableName, char* field, char* condition){
  // Banco de dados
  sqlite3* db = db_ptr;
  char* sql_cmd = NULL;
  sqlite3_stmt* sql_stmt = NULL;
  int ret;
  
  //funcao
  int row = 0;

  strFOverwrite(&sql_cmd,  
    "SELECT ID, %s FROM %s "\
    "WHERE (%s);"\
  "", field, tableName, condition);

  ret = getStmt(db, &sql_stmt, sql_cmd);

  
    while (ret == SQLITE_ROW){
      printf(
        "[%d] -> %s \tID: %d\n"\
      "", row+1, sqlite3_column_text(sql_stmt, 1), sqlite3_column_int(sql_stmt, 0));

      ret = sqlite3_step(sql_stmt);

      row++;
    }

  sqlite3_finalize(sql_stmt);

  return row;
}


//login
Usuario *fazerLogin(sqlite3* db_ptr, char *email, char *senha) {
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;

  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }

  // funcao
  Usuario *usuarioLogin = NULL;
  

  //pegando dados do banco de dados
  

  strFOverwrite(&sql_cmd,
    "SELECT * FROM USUARIO_TB "\
    "WHERE (EMAIL = '%s'); "\

  "", email);

  
  
  
  ret = getStmt(db, &sql_stmt, sql_cmd);
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
          "WHERE (USUARIO_FK = %d); "\

        "",usuarioLogin->categoriaUsuario, usuarioLogin->id);



        ret = getStmt(db, &sql_stmt, sql_cmd);
        //printf("ID: %d\n", sqlite3_column_int(sql_stmt, 0));
        
        //verificando
        if(ret == SQLITE_ROW) {
          printf("tipo de usuario existente.\n");

          void* tipoUsuarioLogin = NULL;

          //residente
          if(!strcmp(usuarioLogin->categoriaUsuario, "residente")){
            tipoUsuarioLogin = malloc(sizeof(Residente));

            ((Residente*)tipoUsuarioLogin)->id = usuarioLogin->id;
            ((Residente*)tipoUsuarioLogin)->matricula = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 2), NULL);
            ((Residente*)tipoUsuarioLogin)->turmaFk = sqlite3_column_int(sql_stmt, 3);
            ((Residente*)tipoUsuarioLogin)->preceptorFk = sqlite3_column_int(sql_stmt, 4);
            


          }else if(!strcmp(usuarioLogin->categoriaUsuario, "preceptor")){
            tipoUsuarioLogin = malloc(sizeof(Preceptor));

            ((Preceptor*)tipoUsuarioLogin)->id = usuarioLogin->id;

          }else if(!strcmp(usuarioLogin->categoriaUsuario, "coordenacao")){
            tipoUsuarioLogin = malloc(sizeof(Coordenacao));

            ((Coordenacao*)tipoUsuarioLogin)->id = usuarioLogin->id;

          }else if(!strcmp(usuarioLogin->categoriaUsuario, "gestao")){
            tipoUsuarioLogin = malloc(sizeof(Gestao));

            ((Gestao*)tipoUsuarioLogin)->id = usuarioLogin->id;
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
    
  //printf("===DEBUG===\n");
  
  sqlite3_finalize(sql_stmt);
  

  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  if(ACTIVE){
    sqlite3_close(db);
  }
  
  
  return usuarioLogin;
};


//pegar usuarios
int getUsuarioTB(sqlite3* db_ptr, Usuario* usuario, int usuario_id) {
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;


  // funcao
  //Usuario *usuario = NULL;
  

  //pegando dados do banco de dados
  

  strFOverwrite(&sql_cmd,
    "SELECT * FROM USUARIO_TB "\
    "WHERE (ID = %d); "\

  "", usuario_id);

  ret = getStmt(db, &sql_stmt, sql_cmd);

  if(ret != SQLITE_ROW){
    sqlite3_finalize(sql_stmt);
    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    return 0;
  }

  //usuario = (Usuario *)malloc(sizeof(Usuario));

  usuario->id = sqlite3_column_int(sql_stmt, 0);
  usuario->nome = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 1), NULL);
  usuario->email = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 2), NULL);
  usuario->senha = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 3), NULL);
  usuario->categoriaUsuario = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 4), NULL);

  usuario->tipoUsuario = NULL;
  
  sqlite3_finalize(sql_stmt);

  //pegando tipo de usuario
  
  
  //verificando se o tipo do usuario existe
  strFOverwrite(&sql_cmd,
    "SELECT * FROM %s_TB "\
    "WHERE (USUARIO_FK = %d); "\

  "",usuario->categoriaUsuario, usuario->id);

  
  ret = getStmt(db, &sql_stmt, sql_cmd);
  
  
  //verificando
  if(ret != SQLITE_ROW) {
    sqlite3_finalize(sql_stmt);
    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    return 2;
  }


  void* tipousuario = NULL;

  //residente
  if(!strcmp(usuario->categoriaUsuario, "residente")){
    // tipousuario = malloc(sizeof(Residente));

    // ((Residente*)tipousuario)->id = usuario->id;
    // ((Residente*)tipousuario)->matricula = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 2), NULL);
    // ((Residente*)tipousuario)->turmaFk = sqlite3_column_int(sql_stmt, 3);
    // ((Residente*)tipousuario)->preceptorFk = sqlite3_column_int(sql_stmt, 4);

    getResidente(db, usuario->tipoUsuario, usuario->id);
    


  }else if(!strcmp(usuario->categoriaUsuario, "preceptor")){
    tipousuario = malloc(sizeof(Preceptor));

    ((Preceptor*)tipousuario)->id = usuario->id;

  }else if(!strcmp(usuario->categoriaUsuario, "coordenacao")){
    tipousuario = malloc(sizeof(Coordenacao));

    ((Coordenacao*)tipousuario)->id = usuario->id;

  }else if(!strcmp(usuario->categoriaUsuario, "gestao")){
    tipousuario = malloc(sizeof(Gestao));

    ((Gestao*)tipousuario)->id = usuario->id;
    ((Gestao*)tipousuario)->cargo = strFOverwrite(NULL, (char*)sqlite3_column_text(sql_stmt, 2), NULL);

  }
  
  //printf("===DEBUG===\n");
  sqlite3_finalize(sql_stmt);
  

  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  
  
  return 1;
};

void getResidente(sqlite3* db_ptr, Residente* residente, int residente_id){
// Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;

  // funcao

  
  //pegando dados do banco de dados
  

  strFOverwrite(&sql_cmd,
    "SELECT * FROM RESIDENTE_TB "\
    "WHERE (ID = %d); "\

  "", residente_id);

  ret = getStmt(db, &sql_stmt, sql_cmd);
    if(ret == SQLITE_ROW){
      
      residente->id = sqlite3_column_int(sql_stmt, 0);
      residente->usuarioFk = sqlite3_column_int(sql_stmt, 1);
      residente->matricula = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 2), NULL);
      residente->turmaFk = sqlite3_column_int(sql_stmt, 3);
      residente->preceptorFk = sqlite3_column_int(sql_stmt, 4);
      
      strFOverwrite(&sql_cmd,
        "WHERE (TURMA_FK = %d); "\
      "", residente->turmaFk);
      residente->listaAtividades = getTableIDLs(db, "ATIVIDADE_TB", sql_cmd);

      strFOverwrite(&sql_cmd,
        "WHERE (RESIDENTE_FK = %d); "\
      "", residente->id);
      residente->listaAtividades = getTableIDLs(db, "SUBMISSAO_TB", sql_cmd);

      strFOverwrite(&sql_cmd,
        "WHERE (RESIDENTE_FK = %d); "\
      "", residente->id);
      residente->listaAtividades = getTableIDLs(db, "FEEDBACK_TB", sql_cmd);
      //residente->notasTrimestrais = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 7), NULL);
    }
    else{
      sysStatus(db, ret);
    }
  sqlite3_finalize(sql_stmt);

  if(sql_cmd != NULL){
    free(sql_cmd);
  }


}


//pegar objetos
void getTurmaTB(sqlite3* db_ptr, Turma* turma, int turma_id){
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;

  // funcao

  
  //pegando dados do banco de dados
  

  strFOverwrite(&sql_cmd,
    "SELECT * FROM TURMA_TB "\
    "WHERE (ID = %d); "\

  "", turma_id);

  ret = getStmt(db, &sql_stmt, sql_cmd);
    if(ret == SQLITE_ROW){

      turma->id = sqlite3_column_int(sql_stmt, 0);
      turma->residenciaFK = sqlite3_column_int(sql_stmt, 1);
      turma->nome = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 2), NULL);
      turma->ano = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 3), NULL);
    }
    else{
      sysStatus(db, ret);
    }
  sqlite3_finalize(sql_stmt);

  if(sql_cmd != NULL){
    free(sql_cmd);
  }

}




//cadastro de usuarios
int addUsuarioTB(sqlite3* db_ptr, char *nome, char *email, char *senha, char *tipoDeUsuario) {
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao
  

  //pegando dados do banco de dados
  //strFormater()


  // verificando cadastro
  strFOverwrite(&sql_cmd,
    "SELECT EMAIL FROM USUARIO_TB "\
    "WHERE (EMAIL = '%s'); "\

  "", email);

  ret = getStmt(db, &sql_stmt, sql_cmd);
    
  // cadastro ja existente
  if(ret == SQLITE_ROW){
    printf("EMAIL:  %s\n", sqlite3_column_text(sql_stmt, 0));

    if(!strcmp(email, sqlite3_column_text(sql_stmt, 0))){

      printf("email ja cadastrado!\n");

      sqlite3_finalize(sql_stmt);
      if(ACTIVE){
        sqlite3_close(db);
      }
      return ret;

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
  sysStatus(db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  
  sqlite3_finalize(sql_stmt);
  if(ACTIVE){
    sqlite3_close(db);
  }
  
  return ret;
};

int addGestaoTB(sqlite3* db_ptr, int usuario_fk, char *cargo) {
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao

  // criando tabela do tipo
  strFOverwrite(&sql_cmd,  
    "INSERT INTO GESTAO_TB (USUARIO_FK, CARGO) "\
    "VALUES (%d, '%s'); "\

  "", usuario_fk, cargo);

  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);


  //verificando criacao de tabela
  if(ret != SQLITE_OK){

    if(sql_cmd != NULL){
      free(sql_cmd);
    }

    if(ACTIVE){
      sqlite3_close(db);
    }
    return ret;
  }


  //pegando ID do tipo criado
  strFOverwrite(&sql_cmd,  
    "SELECT ID FROM GESTAO_TB "\
    "WHERE (USUARIO_FK = %d); "\
  "", usuario_fk);
  
  ret = getStmt(db, &sql_stmt, sql_cmd);

  if(ret != SQLITE_ROW){
    

    sqlite3_finalize(sql_stmt);

    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    if(ACTIVE){
      sqlite3_close(db);
    }

    return ret;
  }
  
  int tipoID = sqlite3_column_int(sql_stmt, 0);

  sqlite3_finalize(sql_stmt);
  sql_stmt = NULL;


  //alterando ID do tipo de usuario
  strFOverwrite(&sql_cmd,  
    "UPDATE USUARIO_TB "\
    "SET "\
      "TIPO_FK = %d "\
    ""\

    "WHERE (ID = %d); "\

  "", tipoID, usuario_fk);
    
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);

  //tipo criado


  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  if(ACTIVE){
    sqlite3_close(db);
  }

  return ret;
}

int addCoordenacaoTB(sqlite3* db_ptr, int usuario_fk, int residencia_fk, char *cargo) {
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao

  // criando tabela do tipo
  strFOverwrite(&sql_cmd,  
    "INSERT INTO COORDENACAO_TB (USUARIO_FK, RESIDENCIA_FK, CARGO) "\
    "VALUES (%d, %d, '%s'); "\

  "", usuario_fk, residencia_fk, cargo);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);



  //verificando criacao de tabela
  if(ret != SQLITE_OK){

    if(sql_cmd != NULL){
      free(sql_cmd);
    }

    if(ACTIVE){
      sqlite3_close(db);
    }
    return ret;
  }


  //pegando ID do tipo criado
  strFOverwrite(&sql_cmd,  
    "SELECT ID FROM COORDENACAO_TB "\
    "WHERE (USUARIO_FK = %d); "\
  "", usuario_fk);


  ret = getStmt(db, &sql_stmt, sql_cmd);
  if(ret != SQLITE_ROW){
    

    sqlite3_finalize(sql_stmt);

    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    if(ACTIVE){
      sqlite3_close(db);
    }

    return ret;
  }

  
  
  int tipoID = sqlite3_column_int(sql_stmt, 0);

  sqlite3_finalize(sql_stmt);
  sql_stmt = NULL;


  //alterando ID do tipo do usuario
  strFOverwrite(&sql_cmd,  
    "UPDATE USUARIO_TB "\
    "SET "\
      "TIPO_FK = %d "\
    ""\

    "WHERE (ID = %d); "\

  "", tipoID, usuario_fk);
    
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);

  //tipo criado

  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  if(ACTIVE){
    sqlite3_close(db);
  }

  return ret;
}

int addPreceptorTB(sqlite3* db_ptr, int usuario_fk, int turma_fk) {
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao

  // criando tabela do tipo 
  strFOverwrite(&sql_cmd,  
    "INSERT INTO PRECEPTOR_TB (USUARIO_FK, TURMA_FK) "\
    "VALUES (%d, %d); "\

  "", usuario_fk, turma_fk);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);


  //verificando criacao de tabela
  if(ret != SQLITE_OK){

    if(sql_cmd != NULL){
      free(sql_cmd);
    }

    if(ACTIVE){
      sqlite3_close(db);
    }
    return ret;
  }


  //pegando ID do tipo criado
  strFOverwrite(&sql_cmd,  
    "SELECT ID FROM PRECEPTOR_TB "\
    "WHERE (USUARIO_FK = %d); "\
  "", usuario_fk);


  ret = getStmt(db, &sql_stmt, sql_cmd);
  if(ret != SQLITE_ROW){
    

    sqlite3_finalize(sql_stmt);

    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    if(ACTIVE){
      sqlite3_close(db);
    }

    return ret;
  }

  int tipoID = sqlite3_column_int(sql_stmt, 0);

  sqlite3_finalize(sql_stmt);
  sql_stmt = NULL;


  //alterando ID do tipo de usuario
  strFOverwrite(&sql_cmd,  
    "UPDATE USUARIO_TB "\
    "SET "\
      "TIPO_FK = %d "\
    ""\

    "WHERE (ID = %d); "\

  "", tipoID, usuario_fk);
    
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);

  //tipo criado


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  if(ACTIVE){
    sqlite3_close(db);
  }

  return ret;
}

int addResidenteTB(sqlite3* db_ptr, int usuario_fk, char *matricula, int turma_fk, int preceptor_fk, char* notas) {
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao


  // criando tabela do tipo
  strFOverwrite(&sql_cmd,  
    "INSERT INTO RESIDENTE_TB (USUARIO_FK, MATRICULA, TURMA_FK, PRECEPTOR_FK, NOTAS) "\
    "VALUES (%d, '%s', %d, %d, '%s'); "\

  "", usuario_fk, matricula, turma_fk, preceptor_fk, notas);


  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);


  //verificando criacao de tabela
  if(ret != SQLITE_OK){

    if(sql_cmd != NULL){
      free(sql_cmd);
    }

    if(ACTIVE){
      sqlite3_close(db);
    }
    return ret;
  }


  //pegando ID do tipo criado
  strFOverwrite(&sql_cmd,  
    "SELECT ID FROM RESIDENTE_TB "\
    "WHERE (USUARIO_FK = %d); "\
  "", usuario_fk);


  ret = getStmt(db, &sql_stmt, sql_cmd);
  if(ret != SQLITE_ROW){
    

    sqlite3_finalize(sql_stmt);

    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    if(ACTIVE){
      sqlite3_close(db);
    }

    return ret;
  }


  int tipoID = sqlite3_column_int(sql_stmt, 0);

  sqlite3_finalize(sql_stmt);
  sql_stmt = NULL;


  //alterando ID do tipo de usuario
  strFOverwrite(&sql_cmd,  
    "UPDATE USUARIO_TB "\
    "SET "\
      "TIPO_FK = %d "\
    ""\

    "WHERE (ID = %d); "\

  "", tipoID, usuario_fk);
    
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);

  //tipo criado


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  if(ACTIVE){
    sqlite3_close(db);
  }

  return ret;
}



//adicionado estruturas
int addResidenciaTB(sqlite3* db_ptr, char* nome){
// Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao

  // criando tabela residencia
  strFOverwrite(&sql_cmd,  
    "INSERT INTO RESIDENCIA_TB (NOME) "\
    "VALUES ('%s'); "\

  "", nome);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  if(ACTIVE){
    sqlite3_close(db);
  }

  return ret;
}

int addTurmaTB(sqlite3* db_ptr, int residencia_fk, char* nome, char* ano){
// Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao

  // criando tabela residencia
  strFOverwrite(&sql_cmd,  
    "INSERT INTO TURMA_TB (RESIDENCIA_FK, NOME, ANO) "\
    "VALUES (%d, '%s', '%s'); "\

  "", residencia_fk, nome, ano);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  if(ACTIVE){
    sqlite3_close(db);
  }

  

  return ret;
}




//adicionando objetos
int addAtividadeTB(sqlite3* db_ptr, int turma_fk, char* nome, char* descricao, char* data_p, char* data_e, char* status){
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;
  
  if(ACTIVE){
    sqlite3_open("BD/db.sqlite3", &db);
  }


  // funcao

  // criando tabela residencia
  strFOverwrite(&sql_cmd,  
    "INSERT INTO ATIVIDADE_TB (TURMA_FK, NOME, DESCRICAO, DATA_POSTAGEM, DATA_ENTREGA, STATUS) "\
    "VALUES (%d, '%s', '%s', '%s', '%s', '%s'); "\

  "", turma_fk, nome, descricao, data_p, data_e, status);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  sysStatus(db, ret);


  if(sql_cmd != NULL){
    free(sql_cmd);
  }

  if(ACTIVE){
    sqlite3_close(db);
  }

  

  return ret;

}






//adicionando objetos



//função residente visualizar feedback
void visualizarFeedbacks(char *nome);

void getNota(int** arryNota, char* notaDB) {
  if (*arryNota != NULL) {
    free(*arryNota);
  }

  int size = 0;
  for (int i = 0; i < strlen(notaDB); i++) {
    if (notaDB[i] >= '0' && notaDB[i] <= '9') {
      size++;
    }
  }

  *arryNota = (int*)malloc(size * sizeof(int));
  int index = 0;

  for (int i = 0; i < strlen(notaDB); i++) {
    if (notaDB[i] >= '0' && notaDB[i] <= '9') {
      (*arryNota)[index] = notaDB[i] - '0';
      index++;
    }
  }
}

//função residente visualizar feedback




//recursão
int forRecursivo(int i, int range, int ac){
  if(ac==0){
    return 0;
  }

  if(ac>0){
    if(i >= range){
      return 0;
    }

    printf("%d\n", i);

    forRecursivo(i+=ac, range, ac);
  }else{
    if(i <= range){
      return 0;
    }

    printf("%d\n", i);

    forRecursivo(i-=ac, range, ac);
  }

}


//arquivos
int arquivarUsuarios(sqlite3* db_ptr, char* filePath){
  // Banco de dados
  sqlite3* db = db_ptr;
  sqlite3_stmt* sql_stmt = NULL;
  char* sql_cmd = NULL;
  
  int ret;


  // aqruivo
  FILE *file;
  file = fopen(filePath, "w+");

  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  // funcao
  int id;
  char nome[100];
  char email[100];
  char senha[100];
  char tipo[100];
  int tipoFK;


  // pegando residentes do banco de dados
  strFOverwrite(&sql_cmd,
    "SELECT * FROM USUARIO_TB; "\

  "", NULL);
  ret = getStmt(db, &sql_stmt, sql_cmd);

  if(ret == SQLITE_ROW){
    while (ret == SQLITE_ROW){    
      id = sqlite3_column_int(sql_stmt, 0);
      strcpy(nome,(char*)sqlite3_column_text(sql_stmt, 1));
      strcpy(email,(char*)sqlite3_column_text(sql_stmt, 2));
      strcpy(senha,(char*)sqlite3_column_text(sql_stmt, 3));
      strcpy(tipo,(char*)sqlite3_column_text(sql_stmt, 4));
      tipoFK = sqlite3_column_int(sql_stmt, 5);
      
      fprintf(file, "ID:%d \t\tNOME:%s \t\tEMAIL:%s \t\tSENHA:%s \t\tTIPO:%s \t\tTIPO_FK:%d\n", id, nome, email, senha, tipo, tipoFK);

      ret = sqlite3_step(sql_stmt);

      //printf("%d\n", ret);
    }
  }
  else{
    sysStatus(db, ret);

    sqlite3_finalize(sql_stmt);
    if(sql_cmd != NULL){
      free(sql_cmd);
    }
    fclose(file);
    return ret;
  }

  //fprintf(file,"end\n");
  

  sqlite3_finalize(sql_stmt);
  if(sql_cmd != NULL){
    free(sql_cmd);
  }
  fclose(file);

  return 0;

}
