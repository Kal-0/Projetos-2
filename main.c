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
  int ret;


  //oppening or creating the database
  ret = sqlite3_open("BD/database.bd", &db);
  if(ret){
    fprintf(stderr, "nao foi possivel abrir o banco de dados.\n ERRO: %s", sqlite3_errmsg(db));
  }

  

  Usuario* decoyUser1 = NULL;
  Usuario* decoyUser2 = NULL;
  

  decoyUser1 = (Usuario*)malloc(sizeof(Usuario));
  
  decoyUser2 = (Usuario*)malloc(sizeof(Usuario));

  decoyUser2->email = "arromba@123\n";
  
  printf("%s", decoyUser2->email);
  
  
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