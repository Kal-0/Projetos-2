#include "utils.h"


//variaveis globais do banco de dados
sqlite3* db = NULL;
sqlite3_stmt* sql_stmt = NULL;
char* sql_cmd = NULL;
char* fdb_msg = NULL;

int ret;

struct lsAtividade{
  Atividade atividade;
  struct lsAtividade* next;
  struct lsAtividade* last;
};

//variaveis globais
Usuario* perfil = NULL;
void start();
void login();
void navBar();
void home();
void residencias();
void nutricao();
void nutricao_turma1();
void nutricao_turma1_residente();
void nutricao_turma1_preceptor();
void perfilGestao();
void perfilResidente();
void perfilPreceptor();
void perfilCoordenacao();
void menuPerfil();
void homeGestao();
void homeResidente();
void homeCoordenacao();
void homePreceptor();
void navbarGestao();
void navbarPreceptor();
void navbarCoordenacao();
void navbarResidente();
void feedbackResidente();
void feedbackPreceptor();
void printNomeAtividade(struct lsAtividade **head);

struct lsAtividade* tarefinha = NULL;





// int countFilesInFolder(const char* folderPath) {
//     DIR* directory = opendir(folderPath);
//     if (directory == NULL) {
//         perror("Unable to open the directory");
//         return -1;
//     }
//     int fileCount = 0;
//     struct dirent* entry;
//     while ((entry = readdir(directory)) != NULL) {
//         if (entry->d_type == DT_REG) {  // Regular file
//             fileCount++;
//         }
//     }
//     closedir(directory);
//     return fileCount;
// }




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
      "RESIDENCIA_FK INTEGER NOT NULL, "\
      "CARGO TEXT NOT NULL, "\
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
      "MATRICULA TEXT NOT NULL UNIQUE, "\
      "TURMA_FK INTEGER NOT NULL, "\
      "PRECEPTOR_FK INTEGER, "\
      "ATIVIDADES_LS TEXT, "\
      "SUBMISSOES_LS TEXT, "\
      "NOTAS TEXT NOT NULL, "\
      "FEEDBACKS_LS TEXT, "\
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
      "RESIDENTES_LS TEXT, "\
      "PRECEPTORES_LS TEXT, "\
      "ATIVIDADES_LS TEXT, "\
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



    "CREATE TABLE RESIDENCIAS_LS_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "RESIDENCIA_FK INTEGER NOT NULL, "\
      "GESTAO_FK INTEGER NOT NULL, "\
      "FOREIGN KEY (RESIDENCIA_FK) REFERENCES RESIDENCIA_TB(ID), "\
      "FOREIGN KEY (GESTAO_FK) REFERENCES GESTAO_TB(ID) "\
      
    "); "\

    "CREATE TABLE RESIDENTES_LS_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "RESIDENTE_FK INTEGER NOT NULL, "\
      "PRECEPTOR_FK INTEGER, "\
      "ATIVIDADE_FK INTEGER, "\
      "TURMA_FK INTEGER, "\
      "FOREIGN KEY (RESIDENTE_FK) REFERENCES RESIDENTE_TB(ID), "\
      "FOREIGN KEY (PRECEPTOR_FK) REFERENCES PRECEPTOR_TB(ID), "\
      "FOREIGN KEY (ATIVIDADE_FK) REFERENCES ATIVIDADE_TB(ID), "\
      "FOREIGN KEY (TURMA_FK) REFERENCES TURMA_TB(ID) "\

    "); "\

    "CREATE TABLE ATIVIDADES_LS_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "ATIVIDADE_FK INTEGER NOT NULL, "\
      "RESIDENTE_FK INTEGER, "\
      "PRECEPTOR_FK INTEGER, "\
      "TURMA_FK INTEGER, "\
      "FOREIGN KEY (ATIVIDADE_FK) REFERENCES ATIVIDADE_TB(ID), "\
      "FOREIGN KEY (RESIDENTE_FK) REFERENCES RESIDENTE_TB(ID), "\
      "FOREIGN KEY (PRECEPTOR_FK) REFERENCES PRECEPTOR_TB(ID), "\
      "FOREIGN KEY (TURMA_FK) REFERENCES TURMA_TB(ID) "\

    "); "\

    "CREATE TABLE SUBMISSOES_LS_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "SUBMISSAO_FK INTEGER NOT NULL, "\
      "ATIVIDADE_FK INTEGER NOT NULL, "\
      "RESIDENTE_FK INTEGER NOT NULL, "\
      "PRECEPTOR_FK INTEGER, "\
      "FOREIGN KEY (SUBMISSAO_FK) REFERENCES SUBMISSAO_TB(ID), "\
      "FOREIGN KEY (ATIVIDADE_FK) REFERENCES ATIVIDADE_TB(ID), "\
      "FOREIGN KEY (RESIDENTE_FK) REFERENCES RESIDENTE_TB(ID), "\
      "FOREIGN KEY (PRECEPTOR_FK) REFERENCES PRECEPTOR_TB(ID) "\

    "); "\


  "", NULL);

  
    ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
    //printf("%s\n", sql_cmd);
    sysStatus(&db, ret);

  if(0){



    //inserindo tables
    strFOverwrite(&sql_cmd,  
      "INSERT INTO USUARIO_TB (ID,NOME,EMAIL,SENHA,TIPO) "\
      "VALUES (999, 'Paulo', 'paulinho@gmail.com', 'paulinho123', 'gestao' ); "\

    "", NULL);
    
    ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
    //printf("%s\n", sql_cmd);
    sysStatus(&db, ret);


    strFOverwrite(&sql_cmd,  
      "INSERT INTO ATIVIDADE_TB (ID,NOME,DESCRICAO,TURMA_FK,SUBMISSOES_LS,DATA_POSTAGEM,DATA_ENTREGA,STATUS) "\
      "VALUES (51, 'E DE PEIXE?', 'TU É DOIDOOOO', '61', 'gestao','09/06/2023','12/06/2023','A FAZER' ); "\

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
    sql_stmt = NULL;
  }
  //fechando o banco de dados
  sqlite3_close(db);

//pegando dados usando a funcao getStmt();
  
  // getStmt(&db, &sql_stmt, sql_cmd);

  // printf("getStmt: %s\n", sqlite3_column_text(sql_stmt, 1));
  // sqlite3_finalize(sql_stmt);
  // sql_stmt = NULL;













  //testando
  






  tarefinha = (struct lsAtividade*)malloc(sizeof(struct lsAtividade));
  tarefinha->atividade.nomeDaAtividade = "teste";

  printf("noem: %s\n",tarefinha->atividade.nomeDaAtividade);

  strFOverwrite(&sql_cmd,  
    "SELECT * FROM ATIVIDADE_TB; "\

  "", NULL);
    
  ret = getStmt(&db, &sql_stmt, sql_cmd);
  printf("stmt: %s", sqlite3_column_text(sql_stmt, 1));

  sysStatus(&db, ret);
  char* str = strFOverwrite(NULL,sqlite3_column_text(sql_stmt, 1),NULL);
  printf("noem: %s\n", str);
  
  if (ret == SQLITE_ROW){
   
    (tarefinha->atividade).nomeDaAtividade = strFOverwrite(NULL,(char*)sqlite3_column_text(sql_stmt, 1),NULL);
  }

  printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
  printf("uepa: %s", tarefinha->atividade.nomeDaAtividade);







  printf("ADICIONANDO_RESIDENCIA===\n");
  addResidenciaTB(&db, "nutricao");

  printf("ADICIONANDO_TURMA===\n");
  addTurmaTB(&db, 1, "nutricao_1", "2022");


  printf("CADASTRO_USUARIO===\n");
  addUsuarioTB(&db, "caio", "caio@gmail.com", "caio123", "gestao");
  addUsuarioTB(&db, "camila", "camila@gmail.com", "camila123", "gestao");
  addUsuarioTB(&db, "carol", "carol@gmail.com", "carol123", "coordenacao");
  addUsuarioTB(&db, "paulo", "paulo@gmail.com", "paulo123", "preceptor");
  addUsuarioTB(&db, "diogo", "diogo@gmail.com", "diogo123", "residente");

  printf("CADASTRO_GESTAO===\n");
  addGestaoTB(&db, 1, "financeiro");
  addGestaoTB(&db, 2, "gestora_lider");


  printf("CADASTRO_COORDENACAO===\n");
  addCoordenacaoTB(&db, 3, "diretora", 1);


  printf("CADASTRO_PRECEPTOR===\n");
  addPreceptorTB(&db, 4, 1);

  printf("CADASTRO_RESIDENTE===\n");
  addResidenteTB(&db, 5, "1234567", 1, 1, "[0,0,0,0]");


  printf("PRINT_GESTAO===\n");
  lsID* lsGestao = NULL;
  lsGestao = getTableIDLs(&db, "USUARIO_TB", "TIPO = 'gestao'");

  printLs(&lsGestao);

  printf("LOGIN===\n");
  perfil = getUsuarioTB(&db, "caio@gmail.com", "caio123");



  printf("nome: %s\n", perfil->nome);






  Usuario* decoyUser1 = NULL;
  Usuario* decoyUser2 = NULL;
  Residente* r1 = NULL;

  decoyUser1 = (Usuario*)malloc(sizeof(Usuario));
  decoyUser2 = (Usuario*)malloc(sizeof(Usuario));

  r1 = (Residente*)malloc(sizeof(Residente));



  decoyUser2->email = "arromba@123";
  decoyUser2->tipoUsuario = r1;
  decoyUser2->categoriaUsuario = (void*)r1;
  
  r1->matricula = 1234567;

  printf("%s\n", decoyUser2->email);
  printf("%d\n", ((Residente*)decoyUser2->tipoUsuario)->matricula);
  
  
  decoyUser1->tipoUsuario = (void*)decoyUser2;

  ((Usuario*)(decoyUser1->tipoUsuario))->email = "bebe\n";
  
  printf("%s\n\n", decoyUser2->email);

  perfil = NULL;




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

      vef = addUsuarioTB(&db, nome, email, senha, tipo);
      //printf("vef: %d\n", vef);
      
      if(vef == 0){
        Usuario* usuarioCriado = NULL;
        usuarioCriado = getUsuarioTB(&db, email, senha);

        int usuario_fk = usuarioCriado->id;

        free(usuarioCriado);
        



        if(!strcmp(tipo, "residente")){

        }

        if(!strcmp(tipo, "preceptor")){

        }

        if(!strcmp(tipo, "coordenacao")){
          int residencia_fk;
          char cargo[50];
          
          printf("selecione sua residencia: ");
          scanf("%d", residencia_fk);
          getchar();

          printf("insira seu cargo: ");
          scanf("%s", cargo);
          getchar();



          vef = addGestaoTB(&db, usuario_fk, cargo);

          if(vef == 0){
            printf("cadastro realisado com sucesso!\n\n");
            break;
          }
        }


        if(!strcmp(tipo, "gestao")){
          char cargo[50];
          


          printf("insira seu cargo: ");
          scanf("%s", cargo);
          getchar();



          vef = addGestaoTB(&db, usuario_fk, cargo);

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

    perfil = getUsuarioTB(&db, email, senha);

    if(perfil == NULL){
      printf("credenciais invalidas!\n"\
        "deseja tentar novamente? (s/n): "
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
      
      break;
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

        if(perfil != NULL){
          home();
        }
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
  
  if(!strcmp(perfil->categoriaUsuario,"gestao"))
  {
    navbarGestao();
  }else if (!strcmp(perfil->categoriaUsuario,"residente"))
  {
   navbarResidente();
  }
  else if (!strcmp(perfil->categoriaUsuario,"preceptor"))
  {
    navbarPreceptor();
  }
  else if (!strcmp(perfil->categoriaUsuario,"coordenacao"))
  {
    navbarCoordenacao();
  }

}


void home(){

  if(!strcmp(perfil->categoriaUsuario,"gestao"))
  {
    homeGestao();
  }else if (!strcmp(perfil->categoriaUsuario,"residente"))
  {
    homeResidente();
  }
  else if (!strcmp(perfil->categoriaUsuario,"preceptor"))
  {
    homePreceptor();
  }
  else if (!strcmp(perfil->categoriaUsuario,"coordenacao"))
  {
    homeCoordenacao();
  }
  
}
 

void verAtividades(){
    printf("---ATIVIDADES---\n"\
      "ATIVIDADES DISPONIVEIS ( 1 )\n"\
      "   ver atividades [ 1 ] [ 2 ]\n"\
      "ATIVIDADES PASSADAS ( 3 )\n"\
      "\n:"
    );
    
}


void residencias(){
  while (1)
  {
    printf("RESIDENCIAS\n"\
    "[1] NUTRICAO\n"\
    "[2] PSICOLOGIA\n"\
    "[-1] Sair\n"\
    "Digite uma das opcoes\n"\
    
  );
    int aux;
    scanf("%d", &aux);
    getchar();
    if(aux==-1){
      break;
    }
    switch (aux)
    {
    case 1:
      nutricao();
      break;
    
    case -1 :
      break;
    }

  }
  
}


void nutricao(){
    printf("NUTRICAO\n"\
      "Iformacoes da residencia: \n"\
      "    Numeros de turma ( 2 )\n"\
      "[1] : Turma 1  \n"\
      "[2] : Turma 2  \n"\
      "[-1] : Sair\n"\
      "\nPara visualizar digite seu numero: \n"
    );
      int aux;
      scanf("%d", &aux);
      getchar();
      if(aux==-1){
        residencias();
      }
      switch (aux)
      {
      case 1:
        nutricao_turma1();
        break;

      // FAZ A TURMA 2 AE PO

      case -1:
        residencias();
       }       
}


void nutricao_turma1(){
  printf("TURMA {1}\n"\
        "Iformacoes da turma: \n"\
        "    Numeros de residentes: ( 4 )\n"\
        "    Numeros de preseptores responsaveis: ( 2 )\n"\
        "[1] Visualizar residentes desta turma\n"\
        "[2]Visualizar preceptores responsaveis por esta turma\n"\
        "[3] Adicionar uma atividade para esta turma\n"\
        "[-1] Voltar\n"\
        "Digite uma das opcoes: \n"\
        );
        int aux;
        scanf("%d", &aux);
        getchar();
        if(aux==-1){
          nutricao();
        }
        switch (aux)
        {
        case 1:
        //
          nutricao_turma1_residente();
          break;

        case 2:
        //
          nutricao_turma1_preceptor();
          break;

        case 3:
           printf("Nao existe add atividade por enquanto\n");
          break;
         
        default:
          break;
        }
}


void nutricao_turma1_residente(){
  printf("Residentes \n"\
        "Iformacoes da turma: \n"\
        "    Numeros de residentes: ( 4 )\n"\
        "    Ano de ingresso 2022.2\n"\
        "1) Rebeca ID23\n"\
        "2) Ana ID24\n"\
        "3) Marcos ID25\n"\
        "4) Rubens ID26\n"\
        "-1) Sair\n"\
        "Digite uma das opcoes\n"\
        );
        int aux;
        scanf("%d", &aux);
        getchar();
        if(aux=2){
          menuPerfil();
        }
        else{
        printf("Nao existe por enquanto\n");
          return;
        }
}


void nutricao_turma1_preceptor(){
   printf("Preceptores \n"\
        "Iformacoes da turma: \n"\
        "    Numeros de residentes: ( 2 )\n"\
        "    Ano de ingresso 2022.2\n"\
        "1) Habrosio ID33\n"\
        "2) Ana ID34\n"\
        "-1) Sair\n"\
        "Digite uma das opcoes\n"\
        );
        int aux;
        scanf("%d", &aux);
        getchar();
        if(aux=2){
          menuPerfil();
        }
        else{
        printf("Nao existe por enquanto\n");
          return;
        }
}


void perfilGestao(){
  char aux;
  while (1)
  {
   
    printf("\n [0] -> navBar\n\
          \nPERFIL DA GESTAO\n"\
          "Nome: %s\n"\
          "ID: %d\n"\
          "CARGO: %s\n"\
          "\n", 
          perfil->nome,
          perfil->id, 
          perfil->categoriaUsuario);

    printf("Gostaria de sair? (s/n):  \n");
    scanf("%c", &aux);

    getchar();

    if(aux == 's'){
    break;
    }
    else if (aux == 'n')
    {
      perfilGestao();
    }
    else if (aux == '0')
    {
      navBar();
    }
    else{
      printf("Opcao invalida\n");
    }
  }
}


void perfilResidente() {
  while (1) {
    printf("\n [0] -> navBar\n"\
        "PERFIL DO RESIDENTE\n"\
        "Nome: %s\n"\
        "ID: %d\n"\
        "Residencia: Nutricao\n"\
        "Atividades disponiveis: 1 \n"\
        "Notificacoes:  1\n"
        "     Gostaria de ver as notificacoes? (s/n): "
        "\n:",
         perfil->nome,
         perfil->id
         );

    char esc;
    scanf(" %c", &esc);
    getchar();

    if (esc == 's') {
      printf(
        "\n [0] -> navBar\n"\
        "----NOTIFICACAO----\n"\
        "NOTIFICACOES:\n"\
        "1) EU QUERO MORRER\n"\
        "Deseja voltar para seu perfil? (s/n): ");

      char aux;
      scanf(" %c", &aux);
      getchar();

      if (aux == 's') {
        perfilResidente();
      }
      else if (aux == 'n') {
        homeResidente();
      }
      else if (aux == '0') {
        navBar();
      }
    }
    else if (esc == 'n') {
      homeResidente();
    }
    else if (esc == '0') {
      navBar();
    }
  }
}


void perfilPreceptor() {
  while (1) {
    printf("\n [0] -> navBar\n"\
        "PERFIL DO PRECEPTOR \n"\
        "Nome: %s\n"\
        "ID: %d\n"\
        "CARGO: %s\n"\
        "Residencia: Nutricao\n"\
        "Atividades disponiveis: 1 \n"\
        "Notificacoes:  1\n"
        "     Gostaria de ver as notificacoes? (s/n): "
        "\n:",
         perfil->nome,
         perfil->id, 
         perfil->categoriaUsuario
         );
    char esc;
    scanf(" %c", &esc);
    getchar();

    if (esc == 's') {
      printf(
        "\n [0] -> navBar\n"\
        "----NOTIFICACAO----\n"\
        "NOTIFICACOES:\n"\
        "1) EU QUERO MORRER\n"\
        "Deseja voltar para seu perfil? (s/n): ");
      char aux;
      scanf(" %c", &aux);
      getchar();

      if (aux == 's') {
        perfilPreceptor();
      }
      else if (aux == 'n') {
        homePreceptor();
      }
      else if (aux == '0') {
        navBar();
      }
    }
    else if (esc == 'n') {
      homePreceptor();
    }
    else if (esc == '0') {
      navBar();
    }
  }
}


void perfilCoordenacao() {
  char aux;
  while (1)
  {

    printf("\n [0] -> navBar\n\
          \nPERFIL DA COORDENACAO\n"\
          "Nome: %s\n"\
          "ID: %d\n"\
          "CARGO: %s\n"\
          "\n", 
          perfil->nome,
          perfil->id, 
          perfil->categoriaUsuario);

    printf("Gostaria de sair? (s/n): ");
    scanf(" %c", &aux);

    getchar();

    if (aux == 's') {
      return;
    }
    else if (aux == 'n') {
      perfilCoordenacao();
    }
    else if (aux == '0') {
      navBar();
    }
    else {
      printf("Opcao invalida\n");
    }
  }
}


void menuPerfil(){
  if(!strcmp(perfil->categoriaUsuario,"gestao"))
  {
    perfilGestao();
  }else if (!strcmp(perfil->categoriaUsuario,"residente"))
  {
    perfilResidente();
  }
  else if (!strcmp(perfil->categoriaUsuario,"preceptor"))
  {
    perfilPreceptor();
  }
  else if (!strcmp(perfil->categoriaUsuario,"coordenacao"))
  {
    perfilCoordenacao();
  }
}


void homeGestao(){
    int input = 0;
    while (1)
    {
    printf("===HOME GESTAO===\n"\
        "selecione uma opcao:\n"\
        "[-1] -> Sair do app\n"\
        "[0] -> navBar\n"\
        "[1]-> ver residencias\n"\
        
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
          residencias();
          break;

        default:
          printf("opcao invalida\n");
          break;
      }


      printf("\n\n");
    }
}    


void homeResidente(){
  int input = 0;
  while (1)
  {
  
  
    printf("===HOME RESIDENTE===\n"\
      "selecione uma opcao:\n"\
      "-1 -> Sair\n"\
      "0 -> Barra de navegacao\n"\
      "1 -> Todas atividades\n"\
      "2 -> Atividades do dia\n"\
      
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
        //printNomeAtividade(Tarefinha);
        printf("========TODAS ATIVIDADES=========\n");
        break;

      case 2:
        printf("========ATIVIDADES DO DIA=========\n");
        break;

      default:
        printf("opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}
//A FAZER

void homePreceptor(){
  int input = 0;
    printf("===HOME COORDENACAO===\n"\
        "selecione uma opcao:\n"\
        "[-1] -> voltar\n"\
        "[0] -> navBar\n"\
        "[1]-> ver turma\n"\
        
        "\n:"
      );
    
      scanf("%d", &input);
      getchar();
      
      if(input == -1){
        return;
      }
      switch(input){
        case 0:
          navBar();
          break;
        
        case 1:
          printf("TURMAS FICARA AQUI\n");
          break;

        default:
          printf("opcao invalida\n");
          break;
      }


      printf("\n\n");
}


void homeCoordenacao(){
  int input = 0;
    printf("===HOME COORDENACAO===\n"\
        "selecione uma opcao:\n"\
        "[-1] -> voltar\n"\
        "[0] -> navBar\n"\
        "[1]-> ver turma\n"\
        
        "\n:"
      );
    
      scanf("%d", &input);
      getchar();
      
      if(input == -1){
        return;
      }
      switch(input){
        case 0:
          navBar();
          break;
        
        case 1:
          printf("TURMAS FICARA AQUI\n");
          break;

        default:
          printf("opcao invalida\n");
          break;
      }


      printf("\n\n");
}


void navbarResidente(){
   int input = 0;

  while(1){
    printf("===NavBar===\n\n"\
      "selecione uma opcao:\n"\
      "[-1] -> Voltar\n"\
      "[1]-> Perfil\n"\
      "[2]-> Home\n"\
      "[3]-> Atividade\n"\
      "[4]-> Feeedbacks\n"\
      "[5]-> Notas\n"\
  
       "\n:"
    );
    scanf("%d", &input);
    getchar();
  
    if(input == -1){
      break;
    }
    switch(input){
      case 1:
      menuPerfil();
      break;   
      
      case 2:
        home();
        break;
      
      case 3:
        verAtividades();
        break;
      
      case 4:
        feedbackResidente();
        break;
      
      case 5:
        printf("NOTAS\n");
        break;
      
      default:
        printf("opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}


void navbarGestao(){
int input = 0;

  while(1){
    printf("===NavBar===\n\n"\
      "selecione uma opcao:\n"\
      "[-1] -> voltar\n"\
      "[1]-> perfil\n"\
      "[2]-> atividades\n"\
  
       "\n:"
    );
    scanf("%d", &input);
    getchar();
  
    if(input == -1){
      break;
    }
    switch(input){
      case 1:
      menuPerfil();
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


void navbarPreceptor(){
int input = 0;

  while(1){
    printf("===NavBar===\n\n"\
      "selecione uma opcao:\n"\
      "[-1] -> voltar\n"\
      "[1]-> perfil\n"\
      "[2]-> Feedback\n"\
  
       "\n:"
    );
    scanf("%d", &input);
    getchar();
  
    if(input == -1){
      break;
    }
    switch(input){
      case 1:
      menuPerfil();
      break;   
      
      case 2:
        feedbackPreceptor();
        break;
      
      default:
        printf("opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}


void navbarCoordenacao(){
  int input = 0;

  while(1){
    printf("===NavBar===\n\n"\
      "selecione uma opcao:\n"\
      "[-1] -> voltar\n"\
      "[1]-> perfil\n"\
      "[2]-> atividades\n"\
  
       "\n:"
    );
    scanf("%d", &input);
    getchar();
  
    if(input == -1){
      break;
    }
    switch(input){
      case 1:
      menuPerfil();
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

// TIRAR O PARAMETRO DA FUNÇÃO E PEGAR DIRETO DO PERFIL
void printNomeAtividade(struct lsAtividade **head) {
  struct lsAtividade *temp = *head;
  if(temp==NULL){
    printf("NÃO EXISTE NENHUMA ATIVIDADE\n");
  }
  else{
    while (temp != NULL) {
    printf("%d\n", temp->atividade.nomeDaAtividade);
    temp = temp->next;
    }
  }
}

void feedbackPreceptor(){
    FILE *file;
    char texto[200];

    file = fopen("arquivo.txt", "w");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Digite o texto a ser escrito no arquivo:\n");
    fgets(texto, sizeof(texto), stdin);

    fprintf(file, "%s", texto);

    fclose(file);
    return;
}

void feedbackResidente(){
    FILE *file;
    char texto[200];
    char resposta;

    file = fopen("arquivo.txt", "r");

    if (file == NULL) {
        printf("O arquivo não existe.\n");
        return;
    }

    while (fgets(texto, sizeof(texto), file) != NULL) {
        printf("%s", texto);
    }

    fclose(file);

    printf("Deseja contestar? (s/n): ");
    scanf(" %c", &resposta);

    if (resposta == 's' || resposta == 'S') {
        file= fopen("contestacao.txt", "w");

        if (file == NULL) {
            printf("Erro ao criar o arquivo de contestação.\n");
            return;
        }

        printf("Digite a contestação:\n");
        scanf(" %[^\n]", texto);

        fprintf(file, "%s", texto);

        fclose(file);
    }
  return;
}