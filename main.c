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
      "FOREIGN KEY (USUARIO_FK) REFERENCES USUARIO_TB(ID) "\
    "); "\

    "CREATE TABLE RESIDENTE_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "USUARIO_FK INTEGER NOT NULL UNIQUE, "\
      "MATRICULA INTEGER NOT NULL UNIQUE, "\
      "TURMA TEXT NOT NULL, "\
      "PRECEPTOR TEXT NOT NULL, "\
      "ATIVIDADES TEXT NOT NULL, "\
      "SUBMISSOES TEXT NOT NULL, "\
      "NOTAS TEXT NOT NULL, "\
      "FEEDBACKS TEXT NOT NULL, "\
      "FOREIGN KEY (USUARIO_FK) REFERENCES USUARIO_TB(ID) "\
    "); "\

    "CREATE TABLE RESIDENCIA_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "COORDENACAO TEXT NOT NULL, "\
      "TURMAS TEXT NOT NULL "\
    "); "\

    "CREATE TABLE TURMA_TB( "\
      "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "\
      "RESIDENCIA_FK INTEGER NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "ANO TEXT NOT NULL, "\
      "RESIDENTES TEXT NOT NULL, "\
      "PRECEPTORES TEXT NOT NULL, "\
      "ATIVIDADES TEXT NOT NULL, "\
      "FOREIGN KEY (RESIDENCIA_FK) REFERENCES RESIDENCIA_TB(ID) "\
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

  printf("nome: %s\n", perfil->nome);

  printf("fazer gestao===\n");

  fazerGestaoTB(&db, 999, "financeiro");




  Usuario* decoyUser1 = NULL;
  Usuario* decoyUser2 = NULL;
  Residente* r1 = NULL;

  decoyUser1 = (Usuario*)malloc(sizeof(Usuario));
  decoyUser2 = (Usuario*)malloc(sizeof(Usuario));

  r1 = (Residente*)malloc(sizeof(Residente));



  decoyUser2->email = "arromba@123";
  decoyUser2->categoriaUsuario = r1;
  
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
      "-1 -> sair\n"\
      "1 -> fazer login\n"\
      "2 -> fazer cadastro\n"\
      
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
        //cadastro();
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
      "-1 -> voltar\n"\
      "1 -> perfil\n"\
      "2 -> atividades\n"\
  
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
    printf("===HOME===\n"\
      "selecione uma opcao:\n"\
      "-1 -> voltar\n"\
      "0 -> navBar\n"\
      "1 -> ver residencias\n"\
      
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
        escolhausuario(2);
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


void residencias(){
  while (1)
  {
    printf("RESIDENCIAS\n"\
    "1) NUTRICAO\n"\
    "2) PSICOLOGIA\n"\
    "-1) Sair\n"\
    "Digite uma das opcoes\n"\
    
  );
    int aux;
    scanf("%d", &aux);
    getchar();
    switch (aux)
    {
    case 1:
      printf("NUTRICAO\n"\
      "Iformacoes da residencia: \n"\
      "    Numeros de turma ( 2 )\n"\
      "Turma 1  : Visualizar [ 1 ] \n"\
      "Turma 2  : Visualizar [ 2 ]\n"\
      "Opcao -1 : Sair\n"\
      "\ndigite umas das opcoes acima\n"
    );
      int aux;
      scanf("%d", &aux);
      getchar();
      switch (aux)
      {
      case 1:
        printf("TURMA {1}\n"\
        "Iformacoes da turma: \n"\
        "    Numeros de residentes: ( 4 )\n"\
        "    Numeros de preseptores responsaveis: ( 2 )\n"\
        "1) Visualizar residentes desta turma\n"\
        "2) Visualizar preceptores responsaveis por esta turma\n"\
        "3) Adicionar uma atividade para esta turma\n"\
        "-1) Sair\n"\
        "Digite uma das opcoes\n"\
        );
        int aux;
        scanf("%d", &aux);
        getchar();
        switch (aux)
        {
        case 1:
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
          verperfil();
        }
        else{
        printf("Nao existe por enquanto\n");
          break;
        }
          break;

        case 2:
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
          verperfil();
        }
        else{
        printf("Nao existe por enquanto\n");
          break;
        }
          break;

        case 3:
           printf("Nao existe add atividade por enquanto\n");
          break;
         
        default:
          break;
        }
        
        break;
      
      default:
        break;
      }

      break;
    
    default:
      break;
    }

  }
  
}


void homegestao(){
  printf("BEM VINDO A TELA INICIAL DA GESTÃO\n"\
    "Digite uma das seguintes opcoes\n"\
    "Opcao 1  : Visualizar residencias\n"\
    "Opcao 2  : Visualizar perfil\n"\
    "Opcao -1 : Sair\n"\
  );
  int aux;
  scanf("%d", &aux);
  getchar();
  switch (aux)
  {
  case 1:
  //Residencias
  residencias();
    break;

  case 2:
  //Perfil

    break;

  default:
    break;
  }

}


void escolhausuario(int escolha){
  switch (escolha)
  {
  case 1:
    //fluxo residente
    break;
  
  case 2:
    // fluxo gestão
    homegestao();
    break;

  default:
    break;
  }
}