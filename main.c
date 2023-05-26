#include "utils.h"

Usuario* perfil = NULL;

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


  

  Usuario* decoyUser1 = NULL;
  Usuario* decoyUser2 = NULL;
  

  decoyUser1 = (Usuario*)malloc(sizeof(Usuario));
  
  decoyUser2 = (Usuario*)malloc(sizeof(Usuario));

  decoyUser2->email = "arromba@123\n";
  
  printf("%s", decoyUser2->email);
  
  
  decoyUser1->categoriaUsuario = (void*)decoyUser2;

  ((Usuario*)(decoyUser1->categoriaUsuario))->email = "bebe\n";
  
  printf("%s", decoyUser2->email);




  int input = 0;
  while(1){
    
    printf("eee");
    
    scanf("%d", &input);
    if(input == -1){
      break;
    }
  }
    

  
  printf("Hello World\n");

  return 0;
}