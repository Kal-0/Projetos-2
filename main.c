#include "utils.h"


//variaveis globais do banco de dados
sqlite3* db = NULL;
sqlite3_stmt* sql_stmt = NULL;
char* sql_cmd = NULL;
char* fdb_msg = NULL;

int ret;

//variaveis globais
Usuario* perfil = NULL;
void start();
void cadastro();
void login();
void navBar();
void verPerfil();
void home();









int countFilesInFolder(const char* folderPath) {
    DIR* directory = opendir(folderPath);
    if (directory == NULL) {
        perror("Unable to open the directory");
        return -1;
    }

    int fileCount = 0;
    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) {  // Regular file
            fileCount++;
        }
    }

    closedir(directory);
    return fileCount;
}




int main(void) {
  // char* str1 = "ola %s e %s, bem vindo!";
  // char* str2 = "caio";

  // char* str3 = NULL;
  // strFOverwrite(&str3, str1, str2, "cleber");
  // printf("result: %s\n", str3);


  

  //links para estudo do banco de dados sqlite3:
  //https://www.tutorialspoint.com/sqlite/
  //https://www.sqlitetutorial.net/


  //oppening or creating the database
  //abrindo ou criando o banco de dados

  ret = sqlite3_open("BD/db.sqlite3", &db);


  if(ret){
    fprintf(stderr, "\n\nnao foi possivel abrir o banco de dados.\n ERRO: %s\n\n", sqlite3_errmsg(db));
  }
  else{
    fprintf(stderr, "\n\nabrindo o banco de dados.\n STATUS: %s\n\n", sqlite3_errmsg(db));
  }




  //criando tables
  strFOverwrite(&sql_cmd, 
    "CREATE TABLE USUARIO_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "EMAIL TEXT NOT NULL, "\
      "SENHA TEXT NOT NULL, "\
      "TIPO TEXT NOT NULL, "\
      "TIPO_FK INTEGER "\
    "); "\
    
    "CREATE TABLE GESTAO_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "USUARIO_FK INTEGER NOT NULL UNIQUE, "\
      "CARGO TEXT NOT NULL, "\
      "RESIDENCIAS_LS TEXT, "\
      "FOREIGN KEY (USUARIO_FK) REFERENCES USUARIO_TB(ID) "\
    "); "\

    "CREATE TABLE COORDENACAO_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "USUARIO_FK INTEGER NOT NULL UNIQUE, "\
      "CARGO TEXT NOT NULL, "\
      "RESIDENCIA_FK INTEGER NOT NULL, "\
      "FOREIGN KEY (USUARIO_FK) REFERENCES USUARIO_TB(ID), "\
      "FOREIGN KEY (RESIDENCIA_FK) REFERENCES RESIDENCIA_TB(ID) "\
    "); "\

    "CREATE TABLE PRECEPTOR_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "USUARIO_FK INTEGER NOT NULL UNIQUE, "\
      "TURMA_FK INTEGER NOT NULL, "\
      "RESIDENTES_LS TEXT, "\
      "ATIVIDADES_LS TEXT, "\
      "FEEDBACKS_LS TEXT, "\
      "FOREIGN KEY (USUARIO_FK) REFERENCES USUARIO_TB(ID), "\
      "FOREIGN KEY (TURMA_FK) REFERENCES TURMA_TB(ID) "\
    "); "\

    "CREATE TABLE RESIDENTE_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "USUARIO_FK INTEGER NOT NULL UNIQUE, "\
      "MATRICULA INTEGER NOT NULL UNIQUE, "\
      "TURMA_FK INTEGER NOT NULL, "\
      "PRECEPTOR_FK INTEGER, "\
      "ATIVIDADES_LS TEXT, "\
      "SUBMISSOES_LS TEXT, "\
      "NOTAS TEXT NOT NULL, "\
      "FEEDBACKS_LS TEXT NOT NULL, "\
      "FOREIGN KEY (USUARIO_FK) REFERENCES USUARIO_TB(ID), "\
      "FOREIGN KEY (TURMA_FK) REFERENCES TURMA_TB(ID), "\
      "FOREIGN KEY (PRECEPTOR_FK) REFERENCES PRECEPTOR_TB(ID) "\
    "); "\

    "CREATE TABLE RESIDENCIA_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "COORDENACAO_LS TEXT, "\
      "TURMAS_LS TEXT "\
    "); "\

    "CREATE TABLE TURMA_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "RESIDENCIA_FK INTEGER NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "ANO TEXT NOT NULL, "\
      "RESIDENTES_LS TEXT NOT NULL, "\
      "PRECEPTORES_LS TEXT NOT NULL, "\
      "ATIVIDADES_LS TEXT NOT NULL, "\
      "FOREIGN KEY (RESIDENCIA_FK) REFERENCES RESIDENCIA_TB(ID) "\
    "); "\

    "CREATE TABLE ATIVIDADE_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "DESCRICAO TEXT NOT NULL, "\
      "TURMA_FK INTEGER NOT NULL, "\
      "SUBMISSOES_LS TEXT, "\
      "DATA_POSTAGEM TEXT NOT NULL, "\
      "DATA_ENTREGA TEXT, "\
      "STATUS TEXT NOT NULL, "\
      "FOREIGN KEY (TURMA_FK) REFERENCES TURMA_TB(ID)"\

    "); "\
    
    "CREATE TABLE SUBMISSAO_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "RESIDENTE_FK INTEGER NOT NULL, "\
      "PRECEPTOR_FK INTEGER NOT NULL, "\
      "ATIVIDADE_FK INTEGER NOT NULL, "\
      "NOTA FLOAT, "\
      "RESPOSTA TEXT, "\
      "FEEBACK TEXT, "\
      "STATUS TEXT NOT NULL, "\
      "FOREIGN KEY (RESIDENTE_FK) REFERENCES RESIDENTE_TB(ID), "\
      "FOREIGN KEY (PRECEPTOR_FK) REFERENCES PRECEPTOR_TB(ID), "\
      "FOREIGN KEY (ATIVIDADE_FK) REFERENCES ATIVIDADE_TB(ID) "\

    "); "\

    "CREATE TABLE FEEDBACK_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "RESIDENTE_FK INTEGER NOT NULL, "\
      "PRECEPTOR_FK INTEGER NOT NULL, "\
      "NOTA FLOAT NOT NULL, "\
      "CRITERIOS TEXT NOT NULL, "\
      "FEEBACK TEXT NOT NULL, "\
      "CONTESTACAO TEXT, "\
      "RESPOSTA_CONTESTACAO TEXT, "\
      "DATA TEXT NOT NULL, "\
      "STATUS TEXT NOT NULL, "\
      "FOREIGN KEY (RESIDENTE_FK) REFERENCES RESIDENTE_TB(ID), "\
      "FOREIGN KEY (PRECEPTOR_FK) REFERENCES PRECEPTOR_TB(ID) "\

    "); "\

  "", NULL);


  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  //printf("%s\n", sql_cmd);
  sysStatus(&db, ret);




  //inserindo tables
  strFOverwrite(&sql_cmd,  
    "INSERT INTO USUARIO_TB (ID,NOME,EMAIL,SENHA,TIPO) "\
    "VALUES (999, 'Paulo', 'paulinho@gmail.com', 'paulinho123', 'gestao' ); "\

  "", NULL);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  //printf("%s\n", sql_cmd);
  sysStatus(&db, ret);





  //atualizando tables
  strFOverwrite(&sql_cmd,  
    "UPDATE USUARIO_TB "\
    "SET "\
      "NOME = 'Paula', "\
      "EMAIL = 'paulao@gmail.com' "\
    ""\

    "WHERE (ID = 1); "\

  "", NULL);
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
  //printf("%s\n", sql_cmd);
  sysStatus(&db, ret);




  //pegando dados do banco de dados
  strFOverwrite(&sql_cmd,  
    "SELECT * FROM USUARIO_TB; "\

  "", NULL);

  ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
  sysStatus(&db, ret);


  ret = sqlite3_step(sql_stmt);
  sysStatus(&db, ret);
  
  if (ret == SQLITE_ROW) {
    printf("%s\n", sqlite3_column_text(sql_stmt, 1));
    ret = sqlite3_step(sql_stmt);
    sysStatus(&db, ret);
    
  }
  printf("%s\n", sqlite3_column_text(sql_stmt, 1));

  sqlite3_finalize(sql_stmt);
    

  //fechando o banco de dados
  sqlite3_close(db);






  //testando

  printf("CADASTRO===\n");
  fazerCadastro(&db, "jose", "jose@gmail.com", "jose123", "residente");

  printf("LOGIN===\n");
  perfil = fazerLogin(&db, "caio@gmail.com", "paulinho123");

  //printf("nome: %s\n", perfil->nome);

  printf("fazer gestao===\n");

  fazerGestaoTB(&db, 999, "financeiro");




  Usuario* decoyUser1 = NULL;
  Usuario* decoyUser2 = NULL;
  Residente* r1 = NULL;

  decoyUser1 = (Usuario*)malloc(sizeof(Usuario));
  decoyUser2 = (Usuario*)malloc(sizeof(Usuario));

  r1 = (Residente*)malloc(sizeof(Residente));



  decoyUser2->email = "arromba@123";
  decoyUser2->categoriaUsuario = (void*)r1;
  
  r1->matricula = 1234567;

  printf("%s\n", decoyUser2->email);
  printf("%d\n", ((Residente*)decoyUser2->categoriaUsuario)->matricula);
  
  
  decoyUser1->categoriaUsuario = (void*)decoyUser2;

  ((Usuario*)(decoyUser1->categoriaUsuario))->email = "bebe\n";
  
  printf("%s\n\n", decoyUser2->email);




  start();
  //home();
    

  
  printf("Hello World\n");

  return 0;
}

void cadastro(){
  int input = 0;
  int vef = 0;

  char tipo[30];
  char nome[200];
  char email[120];
  char senha[120];
  char op = '_';


  while(1){
    printf("===CADASTRO===\n"\
      "selecione o tipo de usuario:\n"\
      "[-1] -> cancelar\n"\
      "[1] -> residente\n"\
      "[2] -> preceptor\n"\
      "[3] -> coordenacao\n"\
      "[4] -> gestao\n"\

      "\n"
    );
    
    scanf("%d", &input);
    getchar();

    if(input == -1){
      break;
    }
    switch(input){
      case 1:
        strcpy(tipo, "residente");
        break;
      
      case 2:
        strcpy(tipo, "preceptor");
        break;
      
      case 3:
        strcpy(tipo, "coordenacao");
        break;
      
      case 4:
        strcpy(tipo, "gestao");
        break;

      default:
        printf("opcao invalida...\n");

        strcpy(tipo, "invalido");
        break;
    }

    if(strcmp(tipo, "invalido")){

      printf("insira seu nome: ");
      scanf("%s", nome);
      getchar();

      printf("insira seu e-mail: ");
      scanf("%s", email);
      getchar();
      
      printf("insira sua senha: ");
      scanf("%s", senha);
      getchar();

      printf("\n\n");

      vef = fazerCadastro(&db, nome, email, senha, tipo);
      printf("vef: %d", vef);
      if(vef == 0){
        Usuario* usuarioCriado = NULL;
        usuarioCriado = fazerLogin(&db, email, senha);
        int usuario_fk = usuarioCriado->id;

        



        if(!strcmp(tipo, "residente")){

        }

        if(!strcmp(tipo, "preceptor")){

        }

        if(!strcmp(tipo, "coordenacao")){

        }


        if(!strcmp(tipo, "gestao")){
          char cargo[50];
          


          printf("insira seu cargo: ");
          scanf("%s", cargo);
          getchar();

          vef = fazerGestaoTB(&db, usuario_fk, cargo);

          if(vef == 0){
            printf("cadastro realisado com sucesso!\n\n");
            break;
          }
          

        }
      }

      if(vef){
        printf("credenciais invalidas!\n"\
          "deseja tantar novamente? (s/n): "
        );

        scanf("%c", &op);
        getchar();

        if(op == 'n'){
          break;
        }
      }
    }
  }
}




void login(){
  char email[120];
  char senha[120];
  char op = '_';


  while(1){
    printf("===LOGIN===\n"\
      "insira seus dados.\n"\

      "\n"
    );

    printf("insira seu e-mail: ");
    scanf("%s", email);
    getchar();
    
    printf("insira sua senha: ");
    scanf("%s", senha);
    getchar();

    printf("\n\n");

    perfil = fazerLogin(&db, email, senha);

    if(perfil == NULL){
      printf("credenciais invalidas!\n"\
        "deseja tantar novamente? (s/n): "
      );

      scanf("%c", &op);
      getchar();

      if(op == 'n'){
        break;
      }
    }else{
      printf("login realizado com sucesso!\n"\
        "nome de usuario: %s"\
        "\n\n"

        ,perfil->nome
      );
      
      

      home();
    }
    printf("\n\n");
  }
}

void start(){
  int input = 0;

  while(1){
    printf("===START===\n"\
      "selecione uma opcao:\n"\
      "[-1] -> sair\n"\
      "[1] -> fazer login\n"\
      "[2] -> fazer cadastro\n"\
      
      "\n:"
    );
  
    
    scanf("%d", &input);
    getchar();
    
    if(input == -1){
      break;
    }
    switch(input){
      case 1:
        login();
        break;
      
      case 2:
        cadastro();
        break;
      
      default:
        printf("opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}


void navBar(){
  int input = 0;

  while(1){
    printf("===NavBar===\n\n"\
      "selecione uma opcao:\n"\
      "[-1] -> voltar\n"\
      "[1] -> perfil\n"\
      "[2] -> atividades\n"\
  
      "\n:"
    );
  
    
    scanf("%d", &input);
    getchar();
  
    if(input == -1){
      break;
    }
    switch(input){
      case 1:
        verPerfil();
        break;
      
      case 2:
        printf("ver atividades\n");
        break;
      
      default:
        printf("opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}




void home(){
  int input = 0;

  while(1){

    //home gestao
    if(!strcmp(perfil->categoriaUsuario, "gestao")){
      printf("===HOME===\n"\
        "selecione uma opcao:\n"\
        "[-1] -> voltar\n"\
        "[0] -> navBar\n"\
        "[1] -> ver residencias\n"\
        
        "\n:"
      );
    
      
      scanf("%d", &input);
      getchar();
      
      if(input == -1){
        break;
      }
      switch(input){
        case 0:
          navBar();
          break;
        
        case 1:
          //residencias();
          printf("==============================");
          break;

        default:
          printf("opcao invalida\n");
          //home();
          break;
      }

      printf("\n\n");
    }
    


    //home residente
    if(!strcmp(perfil->categoriaUsuario, "residente")){
      printf("===HOME===\n"\
        "selecione uma opcao:\n"\
        "[-1] -> voltar\n"\
        "[0] -> navBar\n"\
        "[1] -> ver residencias\n"\
        
        "\n:"
      );
    
      
      scanf("%d", &input);
      getchar();
      
      if(input == -1){
        break;
      }
      switch(input){
        case 0:
          navBar();
          break;
        
        case 1:
          //residencias();
          printf("==============================");
          break;

        default:
          printf("opcao invalida\n");
          //home();
          break;
      }

      printf("\n\n");
    }



  }
}

void verPerfil(){
  int input = 0;

  while(1){
    printf("PERFIL\n"\
        "Nome: ANA\n"\
        "Matricula: 00892023\n"\
        "Residencia: Nutricao\n"\
        "Atividades disponiveis: 1 \n"\
        "Notificacoes:  1\n"
        "     Gostaria de ver as notificacoes? [1] S [2] N"
        "\n:"
      );
      int esc;
      scanf("%d", &esc);
      getchar();
      if (esc==1){
        printf("----NOTIFICACAO----\n"\
        "NOTIFICACOES:\n"\
        "1 EU QUERO MORRER\n"\
        "Deseja voltar para seu perfil? [1] S [2] N ");
        int aux;
        scanf("%d", &aux);
        if(aux == 1){
          verPerfil();
        }
        else{
          break;
        }
      }
      else{
        break;
      }
  }
}

void verAtividades(){
  int input = 0;

  while(1){
    printf("---ATIVIDADES---\n"\
      "ATIVIDADES DISPONIVEIS ( 1 )\n"\
      "   ver atividades [ 1 ] [ 2 ]\n"\
      "ATIVIDADES PASSADAS ( 3 )\n"\
      "\n:"
    );

  }
}




