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
void cadastro();
void login();
void navBar();
void home();
void residencias();
void turmas(char* condicao);
void verTurma(int turma_id);
void residentes(char* condicao);
void verResidente(int residente_id);
void verAtividades();
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
void notasResi();
void telaPrecTurma();
void addResidente();
void addPreceptor();
void addAtividade();





int feedbackcont = 0;

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




  // lsID* lisa = NULL;
  // append(&lisa, 1);
  // append(&lisa, 2);
  // append(&lisa, 3);
  // append(&lisa, 4);
  // printLs(&lisa);

  // printf("ID: %d\n", getItemLs(&lisa, -1));








if(1){

  ret = sqlite3_open("BD/db.sqlite3", &db);

  forRecursivo(3, 10, 1);

  // arquivarUsuarios(db, "usuarios.txt");

  if(ret){
    fprintf(stderr, "\n\nnao foi possivel abrir o banco de dados.\n ERRO: %s\n\n", sqlite3_errmsg(db));
  }
  else{
    fprintf(stderr, "\n\nabrindo o banco de dados.\n STATUS: %s\n\n", sqlite3_errmsg(db));
  }


  //setup do banco de dados

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
      "TURMA_FK INTEGER NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "DESCRICAO TEXT NOT NULL, "\
      "DATA_POSTAGEM TEXT NOT NULL, "\
      "DATA_ENTREGA TEXT, "\
      "SUBMISSOES_LS TEXT, "\
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
    sysStatus(db, ret);



  //IGNORE
  if(0){



    //inserindo tables

    // strFOverwrite(&sql_cmd,  
    //   "INSERT INTO USUARIO_TB (ID,NOME,EMAIL,SENHA,TIPO) "\
    //   "VALUES (999, 'Paulo', 'paulinho@gmail.com', 'paulinho123', 'gestao' ); "\

    // "", NULL);
    
    // ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
    // //printf("%s\n", sql_cmd);
    // sysStatus(db, ret);


    // strFOverwrite(&sql_cmd,  
    //   "INSERT INTO ATIVIDADE_TB (ID,NOME,DESCRICAO,TURMA_FK,SUBMISSOES_LS,DATA_POSTAGEM,DATA_ENTREGA,STATUS) "\
    //   "VALUES (51, 'E DE PEIXE?', 'TU É DOIDOOOO', '61', 'gestao','09/06/2023','12/06/2023','A FAZER' ); "\

    // "", NULL);

    // ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
    // //printf("%s\n", sql_cmd);
    // sysStatus(db, ret);


    // //atualizando tables
    // strFOverwrite(&sql_cmd,  
    //   "UPDATE USUARIO_TB "\
    //   "SET "\
    //     "NOME = 'Paula', "\
    //     "EMAIL = 'paulao@gmail.com' "\
    //   ""\

    //   "WHERE (ID = 1); "\

    // "", NULL);
    
    // ret = sqlite3_exec(db, sql_cmd, NULL, 0, NULL);
    // //printf("%s\n", sql_cmd);
    // sysStatus(db, ret);







    // //pegando dados do banco de dados
    // strFOverwrite(&sql_cmd,  
    //   "SELECT * FROM USUARIO_TB; "\

    // "", NULL);

    // ret = sqlite3_prepare_v2(db, sql_cmd, -1, &sql_stmt, 0);
    // sysStatus(db, ret);


    // ret = sqlite3_step(sql_stmt);
    // sysStatus(db, ret);
    
    // if (ret == SQLITE_ROW) {
    //   printf("%s\n", sqlite3_column_text(sql_stmt, 1));
    //   ret = sqlite3_step(sql_stmt);
    //   sysStatus(db, ret);
      
    // }
    // printf("%s\n", sqlite3_column_text(sql_stmt, 1));

    // sqlite3_finalize(sql_stmt);
    // sql_stmt = NULL;
  }


  //fechando o banco de dados
  sqlite3_close(db);

//pegando dados usando a funcao getStmt();
  
  // getStmt(db, &sql_stmt, sql_cmd);

  // printf("getStmt: %s\n", sqlite3_column_text(sql_stmt, 1));
  // sqlite3_finalize(sql_stmt);
  // sql_stmt = NULL;



  // for(int n = 0; n<10; n++){
  //   char* tc = NULL;
  //   int i = 1;
  //   tc = strFOverwrite(NULL, "ID = %d", n);
  //   printf("===========: %s\n",tc);
  // }






  //testando
  



  ret = sqlite3_open("BD/db.sqlite3", &db);





  printf("|     TESTES UNITARIOS    |\n");

  printf("|  ADICIONANDO RESIDENCIA  |\n");
  addResidenciaTB(db, "nutricao");
  addResidenciaTB(db, "psicologia");

  printf("| ADICIONANDO TURMA   |\n");
  addTurmaTB(db, 1, "nutricao_1", "2022");
  addTurmaTB(db, 1, "nutricao_2", "2022");


  printf("|  CADASTRO USUARIO  |\n");
  addUsuarioTB(db, "caio", "caio@gmail.com", "caio123", "gestao");
  addUsuarioTB(db, "camila", "camila@gmail.com", "camila123", "gestao");
  addUsuarioTB(db, "carol", "carol@gmail.com", "carol123", "coordenacao");
  addUsuarioTB(db, "paulo", "paulo@gmail.com", "paulo123", "preceptor");
  addUsuarioTB(db, "diogo", "diogo@gmail.com", "diogo123", "residente");

  printf("|  CADASTRO  GESTAO |\n");
  addGestaoTB(db, 1, "financeiro");
  addGestaoTB(db, 2, "gestora_lider");


  printf("|  CADASTRO COORDENACAO  |\n");
  addCoordenacaoTB(db, 3, 1, "diretora");


  printf("|  CADASTRO PRECEPTOR  |\n");
  addPreceptorTB(db, 4, 1);

  printf("|  CADASTRO RESIDENTE  |\n");
  addResidenteTB(db, 5, "1234567", 1, 1, "[0,0,0,0]");


  printf("|  CADASTRO ATIVIDADE  |\n");
  addAtividadeTB(db, 2, "criar plano alimentar", "crie um plano alimentar para um obeso", "11/06/2023", "12/06/2023", "ativo");



  // void* cellv = NULL;
  // int cell1 = 1;
  // cellv = &cell1;

  
  //cell = getCellVoid(db, NULL, "TURMA_TB", "ID", "ID > 0");
  //printf("%d nome: %s", 1, (char*)getCellVoid(db, NULL, "TURMA_TB", "NOME", "ID > 0"));
  // printf("%d nome: %d\n", 1, *(int*)cellv);

  // for(int i=0; i<10; i++){
    
    

  // }
  // printf("PRINT_GESTAO===\n");
  // lsID* lsGestao = NULL;
  // lsGestao = getTableIDLs(db, "USUARIO_TB", "TIPO = 'gestao'");

  // printLs(&lsGestao);
  // freeLs(&lsGestao);


  // printf("LOGIN===\n");
  // perfil = fazerLogin(db, "caio@gmail.com", "caio123");

  // printf("nome: %s\n", perfil->nome);


  // Usuario p1;
  // getUsuarioTB(db, &p1, 1);
  // printf("nome: %s", p1.nome);


  // Residente residente;
  // getResidente(db, &residente, 1);
  // printf("mat: %s", residente.matricula);

  // Turma t1;
  // getTurmaTB(db, &t1, 2);

  // printf("nomeTurma: %s", t1.nomeTurma);

  //=========================
  //Residente residente;
  //getResidente(db, &residente, 1);

  //Usuario usuario;
  //getUsuarioTB(db, &usuario, residente.usuarioFk);
  //=========================


  //printf(": %s", usuario.nome);



  // Usuario* decoyUser1 = NULL;
  // Usuario* decoyUser2 = NULL;
  // Residente* r1 = NULL;

  // decoyUser1 = (Usuario*)malloc(sizeof(Usuario));
  // decoyUser2 = (Usuario*)malloc(sizeof(Usuario));

  // r1 = (Residente*)malloc(sizeof(Residente));



  // decoyUser2->email = "arroba@123";
  // decoyUser2->tipoUsuario = r1;
  // decoyUser2->categoriaUsuario = (void*)r1;
  
  // r1->matricula = 1234567;

  // printf("%s\n", decoyUser2->email);
  // printf("%d\n", ((Residente*)decoyUser2->tipoUsuario)->matricula);
  
  
  // decoyUser1->tipoUsuario = (void*)decoyUser2;

  // ((Usuario*)(decoyUser1->tipoUsuario))->email = "bebe\n";
  
  // printf("%s\n\n", decoyUser2->email);





  //EXECUCAO COMECA AQUI, ^^^ COMENTARIOS ACIMA PODEM SER IGNORADOS ^^^
  perfil = NULL;




  start();
  


  sqlite3_close(db);

}
  printf("Hello World\n");
  
  return 0;
}



void start(){
  int input = 0;
  printf("\n\n");
  while(1){
    printf("  TELA INICIAL  \n\n"
      "Selecione uma opcao:\n\n"\
      "[-1] -> Sair\n"\
      "[1]  -> Fazer login\n"\
      "[2]  -> Fazer cadastro\n"\
      "[3]  -> Arquivar usuarios\n"\
      
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
      
      case 3:
        arquivarUsuarios(db, "usuarios.txt");;
        break;

      default:
        printf("Opcao invalida, tente novamente!\n");
        break;
    }

    printf("\n\n");
  }
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
    printf("| Pagina de Cadastro  |\n"\
      "Selecione o seu o tipo de usuario:\n"\
      "[-1] -> Cancelar\n"\
      "[1]  -> Residente\n"\
      "[2]  -> Preceptor\n"\
      "[3]  -> Coordenacao\n"\
      "[4]  -> Gestao\n"\
      "\n"
    );
    
    scanf(" %d", &input);
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
        printf("Opcao invalida, tente novamente\n");

        strcpy(tipo, "invalido");
        break;
    }

    if(strcmp(tipo, "invalido")){

      printf("Insira seu nome: ");
      scanf("%s", nome);
      getchar();

      printf("Insira seu e-mail: ");
      scanf("%s", email);
      getchar();
      
      printf("Insira sua senha: ");
      scanf("%s", senha);
      getchar();

      printf("\n\n");

      vef = addUsuarioTB(db, nome, email, senha, tipo);
      //printf("vef: %d\n", vef);
      
      if(vef == 0){
        Usuario* usuarioCriado = NULL;
        usuarioCriado = fazerLogin(db, email, senha);
        int usuario_fk = usuarioCriado->id;
        free(usuarioCriado);

        int input;
        


        //residente
        if(!strcmp(tipo, "residente")){
          int turma_fk;
          int preceptor_fk;
          char matricula[15];

          char* sql_cmd = NULL;
          sqlite3_stmt* sql_stmt = NULL;
          int ret = 0;
          int row = 1;

          lsID* listaTurmas = NULL;
          listaTurmas = getTableIDLs(db, "TURMA_TB", "ID != -1");
          int numTurmas = lenLs(&listaTurmas);

          lsID* listaPreceptores = NULL;
          listaPreceptores = getTableIDLs(db, "PRECEPTOR_TB", "ID != -1");
          int numPreceptores = lenLs(&listaPreceptores);

          printf("Insira sua matricula: ");
          scanf(" %s", matricula);
          getchar();

          printf( "\n\n");
          printf("|  TURMAS   |\n"\
            "  Selecione sua turma "\

            "\n"
          );

          
          strFOverwrite(&sql_cmd,  
            "SELECT NOME FROM TURMA_TB; "\

          "", NULL);

          ret = getStmt(db, &sql_stmt, sql_cmd);

          
          while (ret == SQLITE_ROW){
            printf(
              "[%d] -> %s\n"\
            "", row, sqlite3_column_text(sql_stmt, 0));

            ret = sqlite3_step(sql_stmt);

            row++;
          }
        
          sqlite3_finalize(sql_stmt);
          sql_stmt = NULL;
          row = 1;

          printf(": ");
          scanf(" %d", &input);
          getchar();
          turma_fk = getItemLs(&listaTurmas, input-1);


          printf("\n\n");
          printf("|  PRECEPTOR  |\n"\
            "Selecione seu preceptor "\

            "\n"
          );

          
          strFOverwrite(&sql_cmd,  
            "SELECT NOME FROM USUARIO_TB "\
            "WHERE (TIPO = 'preceptor'); "\

          "", NULL);

          ret = getStmt(db, &sql_stmt, sql_cmd);

          
          while (ret == SQLITE_ROW){
            printf(
              "[%d] -> %s\n"\
            "", row, sqlite3_column_text(sql_stmt, 0));

            ret = sqlite3_step(sql_stmt);

            row++;
          }
        
          sqlite3_finalize(sql_stmt);
          sql_stmt = NULL;
          row = 1;

          printf(": ");
          scanf(" %d", &input);
          getchar();
          preceptor_fk = getItemLs(&listaPreceptores, input-1);


          vef = addResidenteTB(db, usuario_fk, matricula, turma_fk, preceptor_fk, "[0,0,0,0]");


          




          freeLs(&listaTurmas);
          freeLs(&listaPreceptores);
          if(vef == 0){
            printf("Cadastro realizado com sucesso!\n\n");
            
            break;
          }
          
        }
        

        //preceptor
        if(!strcmp(tipo, "preceptor")){

        }


        //coordenacao
        if(!strcmp(tipo, "coordenacao")){
          int residencia_fk;
          char cargo[70];

          
          lsID* listaResidencias =NULL;
          listaResidencias = getTableIDLs(db, "RESIDENCIA_TB", "ID != -1");
          int numResidencias = lenLs(&listaResidencias);
          //printLs(&listaResidencias);
          
          char* condition;
          char* residencia;
          int str_len;
          printf("|   RESIDENCIAS   |\n"\
            "Selecione sua residencia: "\

            "\n"
          );
          
          
          // for(int i=0; i<numResidencias; i++){
          //   condition = strFOverwrite(NULL, "ID = %d", i+1);
          //   residencia = ((char*)getCellVoid(db, &str_len, "RESIDENCIA_TB", "NOME", condition));
          //   residencia[str_len] = '\0';

          //   printf(
          //     "[%d] -> %s\n"\
          //   "", i+1, residencia);


            
          //   free(condition);
          //   free(residencia);
          // }
          
          
          char* sql_cmd = NULL;
          sqlite3_stmt* sql_stmt = NULL;
          int ret = 0;
          int row = 1;

          strFOverwrite(&sql_cmd,  
            "SELECT NOME FROM RESIDENCIA_TB; "\

          "", NULL);

          ret = getStmt(db, &sql_stmt, sql_cmd);

          
          while (ret == SQLITE_ROW){
            printf(
              "[%d] -> %s\n"\
            "", row, sqlite3_column_text(sql_stmt, 0));

            ret = sqlite3_step(sql_stmt);

            row++;
          }
        
          sqlite3_finalize(sql_stmt);


          printf(": ");


          scanf(" %d", &input);
          getchar();

          
          residencia_fk = getItemLs(&listaResidencias, input-1);
          
          printf("Insira seu cargo: ");
          scanf(" %s", cargo);
          getchar();



          vef = addCoordenacaoTB(db, usuario_fk, residencia_fk, cargo);
          
          freeLs(&listaResidencias);
          if(vef == 0){
            printf("Cadastro realizado com sucesso!\n\n");
            
            break;
          }
          
        }

        //gestao
        if(!strcmp(tipo, "gestao")){
          char cargo[50];
          


          printf("Insira seu cargo: ");
          scanf(" %s", cargo);
          getchar();



          vef = addGestaoTB(db, usuario_fk, cargo);

          if(vef == 0){
            printf("Cadastro realizado com sucesso!\n\n");
            break;
          }
          

        }
      }

      if(vef){
        printf("Credenciais invalidas!\n"\
          "Deseja tentar novamente? (s/n): "
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
    printf("|  LOGIN   |\n"\
      "Insira seus dados.\n"\

      "\n"
    );

    printf("Insira seu e-mail: ");
    scanf("%s", email);
    getchar();
    
    printf("Insira sua senha: ");
    scanf("%s", senha);
    getchar();

    printf("\n\n");

    perfil = fazerLogin(db, email, senha);

    if(perfil == NULL){
      printf("Credenciais invalidas!\n"\
        "Deseja tentar novamente? (s/n): "
      );

      scanf("%c", &op);
      getchar();

      if(op == 'n'){
        break;
      }
    }else{
      printf("Login realizado com sucesso!\n"\
        "Nome de usuario: %s"\
        "\n\n"

        ,perfil->nome
      );
      
      break;
    }
    printf("\n\n");
  }
}





//navbar
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

void navbarGestao(){
int input = 0;

  while(1){
    printf("|   NavBar   \n\n"\
      "Selecione uma opcao:\n"\
      "[-1] -> Voltar\n"\
      "[1]  -> Perfil\n"\
      "[2]  -> Atividades\n"\
  
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
        printf("Ver atividades\n");
        break;
      
      default:
        printf("Opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}

void navbarCoordenacao(){
  int input = 0;

  while(1){
    printf("|  NavBar  |\n\n"\
      "Selecione uma opcao:\n"\
      "[-1] -> Voltar\n"\
      "[1]  -> Perfil\n"\
      "[2]  -> Atividades\n"\
  
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
        printf("Ver atividades\n");
        break;
      
      default:
        printf("Opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}

void navbarPreceptor(){
int input = 0;

  while(1){
    printf("|  NavBar  |\n\n"\
      "Selecione uma opcao:\n"\
      "[-1] -> Voltar\n"\
      "[1]  -> Perfil\n"\
      "[2]  -> Feedback\n"\
  
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
        printf("Opcao invalida!\n");
        break;
    }

    printf("\n\n");
  }
}

void navbarResidente(){
   int input = 0;

  while(1){
    printf("|  NavBar   |\n\n"\
      "Selecione uma opcao:\n"\
      "[-1] -> Voltar\n"\
      "[1]  -> Perfil\n"\
      "[2]  -> Home\n"\
      "[3]  -> Atividade\n"\
      "[4]  -> Feeedbacks\n"\
      "[5]  -> Notas\n"\
  
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
        if(feedbackcont == 1){
          feedbackResidente();
        }
        else{
          printf("  No momento, voce ainda nao tem nenhum feedback   \n");
        }
        break;
      
      case 5:
        notasResi();

        break;
  
      default:
        printf("Opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}


//perfil
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

void perfilCoordenacao() {
  char aux;
  while (1)
  {

    printf("\n [0] -> navBar\n\
          \nPerfil da Coordenacao\n"\
          "Nome: %s\n"\
          "ID: %d\n"\
          "Cargo: %s\n"\
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
      printf("Opcao invalida, tente novamente!\n");
    }
  }
}

void perfilPreceptor() {
  while (1) {
    printf("\n [0] -> navBar\n"\
        "Perfil do Preceptor \n"\
        "Nome: %s\n"\
        "ID: %d\n"\
        "Cargo: %s\n"\
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
        "|    Notificacao  |\n"\
        "1) Voce esta chegando atrasado!!\n"\
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
        "1) Amanha tem a conferencia! \n"\
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


//home
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
 
void homeGestao(){
    int input = 0;
    while (1)
    {
    printf("|    Tela Inicial Gestao   |\n"\
        "Selecione uma opcao\n"\
        "[-1] -> Logoff\n"\
        "[0]  -> NavBar\n"\
        "[1]  -> Ver residencias\n"\
        
        
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
          printf("Opcao invalida, tente novamente!\n");
          break;
      }


      printf("\n\n");
    }
}    

void homeCoordenacao(){
  int input = 0;
    printf("|  Tela Inicial Coordenacao |\n"\
        "Selecione uma opcao:\n"\
        "[-1] -> Logoff\n"\
        "[0]  -> NavBar\n"\
        "[1]  -> Ver turma\n"\
        
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
          telaPrecTurma();
          break;

        default:
          printf("opcao invalida\n");
          break;
      }


      printf("\n\n");
}

///A FAZER
void homePreceptor(){
  int input = 0;
    printf("|  Tela Inicial Coordenacao  |\n"\
        "Selecione uma opcao:\n"\
        "[-1] -> Logoff\n"\
        "[0]  -> NavBar\n"\
        "[1]  -> Ver turma\n"\
        
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
          printf("Turmas ficara aqui\n");
          break;

        default:
          printf("Opcao invalida, tente novamente!\n");
          break;
      }


      printf("\n\n");
}

void homeResidente(){
  int input = 0;
  while (1)
  {
  
  
    printf("|  Tela Inicial Residente  |\n"\
      "Selecione uma opcao\n"\
      "[-1] -> Logoff\n"\
      "[0]  -> Barra de navegacao\n"\
      "[1]  -> Todas atividades\n"\
      "[2]  -> Atividades do dia\n"\
      
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
        printf("|    TODAS ATIVIDADES     |\n");
        break;

      case 2:
        printf("|    ATIVIDADES DO DIA    |\n");
        break;

      default:
        printf("Opcao invalida, tente novamente!\n");
        break;
    }

    printf("\n\n");
  }
}


//residencias
void residencias(){
  int input;
  int residencia_id;
  char* pass_condicao;
  while (1){
    residencia_id = 0;

      
    lsID* listaResidencias =NULL;
    listaResidencias = getTableIDLs(db, "RESIDENCIA_TB", "ID != -1");
    int numResidencias = lenLs(&listaResidencias);
    //printLs(&listaResidencias);
    

    char* sql_cmd = NULL;
    sqlite3_stmt* sql_stmt = NULL;
    int ret = 0;
    int row = 1;

    printf("   RESIDENCIAS    \n"\
      "Selecione sua residencia: \n"\

      "\n"
    );

    printf(
      "[-1] -> Voltar\n"\
      "[0]  -> NavBar\n"\
    "");


    strFOverwrite(&sql_cmd,  
      "SELECT NOME FROM RESIDENCIA_TB; "\

    "", NULL);

    ret = getStmt(db, &sql_stmt, sql_cmd);

      while (ret == SQLITE_ROW){
        printf(
          "[%d] -> %s\n"\
        "", row, sqlite3_column_text(sql_stmt, 0));

        ret = sqlite3_step(sql_stmt);

        row++;
      }

    sqlite3_finalize(sql_stmt);


    printf(": ");

    //selecao do usuario
    scanf(" %d", &input);
    getchar();

    if(input == -1){
      break;
    }
    switch (input)
    {
    case 0:
      navBar();
      break;
    
    default:
      if(input>numResidencias){
        printf("Opcao invalida, tente novamente!\n");
        
      }else{
        residencia_id = getItemLs(&listaResidencias, input-1);
        pass_condicao = strFOverwrite(NULL,"RESIDENCIA_FK = %d", residencia_id);
        turmas(pass_condicao);
        free(pass_condicao);
      }
      break;
    }
      

      
    
    
    

  }
  
}


//turmas

void turmas(char* condicao){
  int input;
  int turma_id;
  char* pass_condicao;

  lsID* listaOpcoes =NULL;
  int numOpcoes;

  while(1){
    printf("   TURMAS   \n"\
      "Selecione sua turma: "\

      "\n"
    );
    printf(
      "[-1] -> Voltar\n"\
      "[0]  -> NavBar\n"\
    "");
    
    numOpcoes = printTableColumn(db, "TURMA_TB", "NOME", condicao);
    

    printf(": ");

    //selecao do usuario
    scanf(" %d", &input);
    getchar();

    if(input == -1){
      break;
    }
    switch (input){
    case 0:
      navBar();
      break;
    
    default:
      if(input>numOpcoes){
        printf("Opcao invalida, tente novamente!\n");
        
      }else{
        listaOpcoes = getTableIDLs(db, "TURMA_TB", condicao);
        turma_id = getItemLs(&listaOpcoes, input-1);

        pass_condicao = strFOverwrite(NULL, "ID = %d", turma_id);

        verTurma(turma_id);
      }
      break;
    }

  }
}

void verTurma(int turma_id){
  Turma turma;
  getTurmaTB(db, &turma, turma_id);
  
  int input;
  char* pass_condicao;


  while(1){
    printf("===%s===\n"\
      "Selecione uma opcao: "\

      "\n"
    , turma.nome);
    printf(
      "[-1] -> Voltar\n"\
      "[0]  -> NavBar\n"\
    "");
    printf(
      "[1] -> Ver atividades\n"\
      "[2] -> Add atividades\n"\
      "[3] -> Ver residentes\n"\
      "[4] -> Add residentes\n"\
      "[5] -> Ver preceptores\n"\
      "[6] -> Add preceptores\n"\
    "");

    //selecao do usuario
    printf(": ");
    scanf(" %d", &input);
    getchar();

    if(input == -1){
      break;
    }
    switch (input){
    case 0:
      navBar();
      break;
    
    case 1:
      
      break;

    case 2:

      addAtividade(&turma);
      break;

    case 3:
      pass_condicao = strFOverwrite(NULL, "TURMA_FK = %d", turma.id);
      residentes(pass_condicao);
      free(pass_condicao);
      break;

    case 4:
      addResidente(&turma);
      break;
    
    case 5:
      navBar();
      break;
    
    case 6:
      addPreceptor(&turma);
      break;
    
    default:
      printf("Opcao invalida, tente novamente!\n");
      
      break;
    }

  }
}


//residentes
void residentes(char* condicao){
  int input;
  int residente_id;

  lsID* listaOpcoes = NULL;
  int numOpcoes;

  while(1){
    printf("|  Residentes   |\n"\
      "Selecione um resdente: "\

      "\n"
    );
    printf(
      "[-1] -> Voltar\n"\
      "[0] ->  NavBar\n"\
    "");
    
    numOpcoes = printTableColumn(db, "USUARIO_TB", "NOME", condicao);
    
    //selecao do usuario
    printf(": ");
    scanf(" %d", &input);
    getchar();

    if(input == -1){
      break;
    }
    switch (input){
    case 0:
      navBar();
      break;
    
    default:
      if(input>numOpcoes){
        printf("Opcao invalida, tente novamente!\n");
        
      }else{
        listaOpcoes = getTableIDLs(db, "RESIDENTE_TB", condicao);
        residente_id = getItemLs(&listaOpcoes, input-1);
        verResidente(residente_id);
      }
      break;
    }

  }
}

void verResidente(int residente_id){
  Residente residente;
  getResidente(db, &residente, residente_id);

  Usuario usuario;
  getUsuarioTB(db, &usuario, residente.usuarioFk);
  

  int input;
  char* pass_condicao;


  while(1){
    printf("===%s===\n"\
      "Selecione uma opcao: "\

      "\n"
    , usuario.nome);
    printf(
      "[-1] -> Voltar\n"\
      "[0]  -> NavBar\n"\
    "");
    printf(
      "[1] -> Ver residentes\n"\
      "[2] -> Add residentes\n"\
      "[3] -> Ver residentes\n"\
      "[4] -> Add residentes\n"\
      "[5] -> Ver preceptores\n"\
      "[6] -> Add preceptores\n"\
    "");

    //selecao do usuario
    printf(": ");
    scanf(" %d", &input);
    getchar();

    if(input == -1){
      break;
    }
    switch (input){
    case 0:
      navBar();
      break;
    
    case 1:
      navBar();
      break;

    case 2:
      navBar();
      break;

    case 3:
      navBar();
      break;

    case 4:
      navBar();
      break;
    
    case 5:
      navBar();
      break;
    
    case 6:
      navBar();
      break;
    
    default:
      printf("Opcao invalida, tente novamente!\n");
      
      break;
    }

  }
}


void verAtividades(){
    printf("|  Atividades  |\n"\
      "Atividades Disponiveis ( 1 )\n"\
      "   Ver atividades [ 1 ] [ 2 ]\n"\
      "Atividades Passadas ( 3 )\n"\
      "\n:"
    );
    
}


void nutricao(){
    printf("NUTRICAO\n"\
      "Informacoes da residencia: \n"\
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
        if(aux == 2){
          menuPerfil();
        }
        else{
        printf("Nao existe por enquanto\n");
          return;
        }
}


void nutricao_turma1_preceptor(){
   printf("Preceptores \n"\
        "Informacoes da turma: \n"\
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
        if(aux == 2){
          menuPerfil();
        }
        else{
        printf("Nao existe por enquanto\n");
          return;
        }
}




// TIRAR O PARAMETRO DA FUNÇÃO E PEGAR DIRETO DO PERFIL
void printNomeAtividade(struct lsAtividade **head) {
  struct lsAtividade *temp = *head;
  if(temp==NULL){
    printf("Não existe nenhuma atividade.\n");
  }
  else{
    while (temp != NULL) {
    printf("%s\n", temp->atividade.nomeDaAtividade);
    temp = temp->next;
    }
  }
}


void telaPrecResidente(){
  while (1)
  {
    printf("|  RESIDENTE  |\n"
          "Escolha um residente:\n"
          "[-1] voltar\n"
          "[0]  NavBar\n"
          "[1]  Enviar um feedback\n"
          "[2]  Notas\n"
          
    );
    int aux;
    scanf("%d", &aux);
    switch (aux)
    {
    case -1:
      
      break;
    
    case 0:
      navBar();
      break;
    
    case 1:
      feedbackPreceptor();
      printf("Feedback Enviado\n");
      break;
    
    case 2:
      notasResi();
      break;
    
    default:
      break;
    }
  }
}

void telaPrecTurma(){
  while (1)
  {
    printf("|   TURMA   |\n"
          "Escolha um residente: \n"
          "[1] - Residente 1\n"
          "[2] - Residente 2\n"
          "[3] - Residente 3\n"
    );
    int aux;
    scanf("%d", &aux);
    getchar();
    switch (aux)
    {
    case 1:
      telaPrecResidente();
      break;
    
    case 2:
      telaPrecResidente();
      break;
    
    case 3:
      telaPrecResidente();
      break;
    
    default:
      break;
    }
  }
  
}

void notasResi() {
  char aux;

  do {
    printf("|   NOTAS   |\n"
            "NOTA 1: 8.7\n"
            "NOTA 2: 9.0\n"
            "NOTA 3: 6.6\n"
            "NOTA 4: 7.9\n"
            "Gostaria de sair desta pagina? (s/n)\n");
            
    scanf(" %c", &aux); 
      
  } while (aux != 's');
}




void addResidente(Turma* turma){
  
  int input;
  int vef = 1;

  char nome[200];
  char email[120];
  char senha[120];
  char op = '_';


  while(1){
    vef = 1;

    printf("|  Adicionar Residente   |\n"\
          "[-1] -> Cancelar\n"
          "[0]  -> Navbar\n"
          "[1]  -> Continuar\n"
      "\n"
    );
    
    scanf(" %d", &input);
    getchar();
    

    if(input == -1){
      break;
    }
    switch(input){
      case 1:
        vef = 0;
        break;
      
      case 0:
        navBar();
        break;
      
      default:
        printf("Opcao Invalida, tente novamente! \n");

        break;
    }

    
    if(vef==0){

      printf("Insira o nome do preceptor: ");
      fgets(nome, sizeof(nome), stdin);

      printf("Insira o e-mail do preceptor: ");
      fgets(email, sizeof(email), stdin);
      
      printf("Insira uma senha: ");
      fgets(senha, sizeof(senha), stdin);


      printf("\n\n");

      vef = addUsuarioTB(db, nome, email, senha, "residente");
      //printf("vef: %d\n", vef);
      
      if(vef == 0){
        Usuario* usuarioCriado = NULL;
        usuarioCriado = fazerLogin(db, email, senha);
        int usuario_fk = usuarioCriado->id;
        free(usuarioCriado);

        int input;
        


        //residente
        
        int turma_fk = turma->id;
        int preceptor_fk;
        char matricula[15];


        printf("Insira a matricula do residente: ");
        scanf(" %s", matricula);
        getchar();

        printf("Insira o ID do preceptor do residente: ");
        scanf(" %d", &preceptor_fk);
        getchar();
        


        


        vef = addResidenteTB(db, usuario_fk, matricula, turma_fk, preceptor_fk, "[0,0,0,0]");

        
        if(vef == 0){
          printf("Cadastro realizado com sucesso!\n\n");
          
          break;
        }
        
        
        
      }

      if(vef){
        printf("Credenciais invalidas!\n"\
          "Deseja tentar novamente? (s/n): "
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

void addPreceptor(Turma* turma){
  
  int input;
  int vef = 1;

  char nome[200];
  char email[120];
  char senha[120];
  char op = '_';


  while(1){
    vef = 1;

    printf("|  Adicionar Preceptor   |\n"\
          "[-1] -> Cancelar\n"
          "[0]  -> Navbar\n"
          "[1]  -> Continuar\n"
      "\n"
    );
    
    scanf(" %d", &input);
    getchar();
    

    if(input == -1){
      break;
    }
    switch(input){
      case 1:
        vef = 0;
        break;
      
      case 0:
        navBar();
        break;
      
      default:
        printf("Opcao Invalida, tente novamente! \n");

        break;
    }

    
    if(vef==0){

      printf("Insira o nome do preceptor: ");
      fgets(nome, sizeof(nome), stdin);

      printf("Insira o e-mail do preceptor: ");
      fgets(email, sizeof(email), stdin);
      
      printf("Insira uma senha: ");
      fgets(senha, sizeof(senha), stdin);

      printf("\n\n");

      vef = addUsuarioTB(db, nome, email, senha, "preceptor");
      //printf("vef: %d\n", vef);
      
      if(vef == 0){
        Usuario* usuarioCriado = NULL;
        usuarioCriado = fazerLogin(db, email, senha);
        int usuario_fk = usuarioCriado->id;
        free(usuarioCriado);

        int input;
        


        //preceptor
        
        int turma_fk = turma->id;
        
        


        


        vef = addPreceptorTB(db, usuario_fk, turma_fk);

        
        if(vef == 0){
          printf("Cadastro realizado com sucesso!\n\n");
          
          break;
        }
        
        
        
      }

      if(vef){
        printf("Credenciais invalidas!\n"\
          "Deseja tentar novamente? (s/n): "
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

void addAtividade(Turma* turma){
  
  int input;
  int vef = 1;

  int turma_fk = turma->id;
  char nome[200];
  char descricao[1000];
  char data_p[20];
  char data_e[20];

  char op = '_';


  while(1){
    vef = 1;

    printf("|  Adicionar Atividade   |\n"\
          "[-1] -> Cancelar\n"
          "[0]  -> Navbar\n"
          "[1]  -> Continuar\n"
      "\n"
    );
    
    scanf(" %d", &input);
    getchar();
    

    if(input == -1){
      break;
    }
    switch(input){
      case 1:
        vef = 0;
        break;
      
      case 0:
        navBar();
        break;
      
      default:
        printf("Opcao Invalida, tente novamente! \n");

        break;
    }

    
    if(vef==0){

      printf("Insira o nome da atividade: ");
      fgets(nome, sizeof(nome), stdin);

      printf("Insira a descricao da atividade: ");
      fgets(descricao, sizeof(descricao), stdin);
      
      printf("Insira a data de hoje: ");
      fgets(data_p, sizeof(data_p), stdin);

      printf("Insira a data de entrega da atividade: ");
      fgets(data_e, sizeof(data_e), stdin);

      printf("\n\n");

      vef = addAtividadeTB(db, turma_fk, nome, descricao, data_p, data_e, "ativo");
      //printf("vef: %d\n", vef);
      
      if(vef == 0){
        printf("Cadastro realizado com sucesso!\n\n");
        
        break;
      }
      

        
        
        
        
    }

    if(vef){
      printf("Credenciais invalidas!\n"\
        "Deseja tentar novamente? (s/n): "
      );

      scanf("%c", &op);
      getchar();

      if(op == 'n'){
        break;
      }
      
    }
  }
}


//OBS: Alteram arquivos.txt
void feedbackPreceptor(){
    FILE *file;
    char texto[200];

    file = fopen("arquivo.txt", "w+");

    if (file == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return;
    }

    printf("Digite o texto para um residente a ser escrito no arquivo:\n");
    fgets(texto, sizeof(texto), stdin);

    fprintf(file, "%s", texto);

    fclose(file);
    feedbackcont = 1;

    return;
}

void feedbackResidente(){
    FILE *file;
    char texto[200];
    char resposta;

    file = fopen("arquivo.txt", "r+");

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

        feedbackcont = 0;
    }
  return;
}


