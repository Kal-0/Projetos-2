#include "utils.h"


int input = 0;


Usuario* perfil = NULL;


void navBar();
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

  sqlite3* db = NULL;
  char* sql_cmd;
  char* fdb_msg = NULL;
  
  int ret;
  

  //links para estudo do banco de dados sqlite3:
  //https://www.tutorialspoint.com/sqlite/
  //https://www.sqlitetutorial.net/


  //oppening or creating the database

  ret = sqlite3_open("BD/db.sqlite3", &db);


  if(ret){
    fprintf(stderr, "\n\nnao foi possivel abrir o banco de dados.\n ERRO: %s\n\n", sqlite3_errmsg(db));
  }
  else{
    fprintf(stderr, "\n\nabrindo o banco de dados.\n STATUS: %s\n\n", sqlite3_errmsg(db));
  }




  //criando tables
  sql_cmd =
    "CREATE TABLE USUARIO_TB( "\
      "ID INT PRIMARY KEY NOT NULL, "\
      "NOME TEXT NOT NULL, "\
      "EMAIL TEXT NOT NULL, "\
      "SENHA TEXT NOT NULL, "\
      "TIPO TEXT NOT NULL "\
    "); "\
    
    "CREATE TABLE GESTAO_TB( "\
      "ID INT PRIMARY KEY NOT NULL, "\
      "CARGO TEXT NOT NULL "\
    "); "\


  "";

  ret = sqlite3_exec(db, sql_cmd, NULL, 0, &fdb_msg);
  free(sql_cmd);

  if(ret){
    fprintf(stderr, "\n\nnao foi possivel acessar o banco de dados, \n ERRO: %s\n\n", fdb_msg);
    sqlite3_free(fdb_msg);
  } 
  else{
    fprintf(stderr, "\n\nbanco de dados acessado.\n STATUS: %s\n\n", sqlite3_errmsg(db));
  }




  //inserindo tables
  sql_cmd = 
    "INSERT INTO USUARIO_TB (ID,NOME,EMAIL,SENHA,TIPO) "\
    "VALUES (1, 'Paulo', 'paulinho@gmail.com', 'paulinho123', 'gestao' ); "\

  "";
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, &fdb_msg);
  free(sql_cmd);

  if(ret){
    fprintf(stderr, "\n\nnao foi possivel acessar o banco de dados, \n ERRO: %s\n\n", fdb_msg);
    sqlite3_free(fdb_msg);
  } 
  else{
    fprintf(stderr, "\n\nbanco de dados acessado.\n STATUS: %s\n\n", sqlite3_errmsg(db));
  }




//atualizando tables
  sql_cmd = 
    "UPDATE USUARIO_TB "\
    "SET "\
      "NOME = 'Paula', "\
      "EMAIL = 'paulao@gmail.com' "\
    ""\

    "WHERE (ID = 1); "\

  "";
  
  ret = sqlite3_exec(db, sql_cmd, NULL, 0, &fdb_msg);
  free(sql_cmd);

  if(ret){
    fprintf(stderr, "\n\nnao foi possivel acessar o banco de dados, \n ERRO: %s\n\n", fdb_msg);
    sqlite3_free(fdb_msg);
  } 
  else{
    fprintf(stderr, "\n\nbanco de dados acessado.\n STATUS: %s\n\n", sqlite3_errmsg(db));
  }



  sqlite3_close(db);








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
  
  printf("%s", decoyUser2->email);




  
  home();
    

  
  printf("Hello World\n");

  return 0;
}





void navBar(){
  while(1){
    printf("NavBar\n\n"\
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
        printf("ver perfil\n");
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
  while(1){
    printf("HOME\n"\
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
        printf("ver residencia\n");
        break;
      
      default:
        printf("opcao invalida\n");
        break;
    }

    printf("\n\n");
  }
}