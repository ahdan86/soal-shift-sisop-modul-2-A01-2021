#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define FULL_PATH "/home/erki/modul2/petshop"

void unzipFile(char *pets){
  pid_t pid;

//   printf("DEBUG Unzipping files...\n");

  pid = fork();
  if(pid == 0){
      char *args[] = {"unzip",  "-q", pets, "-d", FULL_PATH, NULL};
      execv("/usr/bin/unzip", args);
  }
  while(wait(NULL)!=pid);
}

void removeNotJpg(){
   struct dirent *dp;
   DIR *folder;
   folder = opendir(FULL_PATH);

    // printf("DEBUG removing %s\n", path);

   if(folder != NULL){  //open dire"%s/%s", PETSHOP_PATH, jenis);ctory
       while((dp = readdir(folder)) != NULL){  //read per names in the file
            if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){ // dont copy parent and current directory
                 if(dp->d_type == DT_DIR){
                      char removePath[400];
                      sprintf(removePath, "%s/%s", FULL_PATH, dp->d_name);

                      pid_t pid;
                      pid = fork();
                      if(pid == 0){
                         char *argv[] = {"rm", "-r", removePath, NULL};
                         execv("/usr/bin/rm", argv);  
                      }
                      while(wait(NULL) != pid);
                 }
            }
       }
   }
}

void categorize(char *sourcePath, char* fileName){
   pid_t pid;
   char *jenis = strtok(fileName, ";"),
        *nama = strtok(NULL, ";"), 
        *umur = strtok(NULL, ";");
    printf("DEBUG NAMA FILE %s %s %s\n", jenis, umur, nama);

    char jenisPath[400], txtPath[400], fileNamePath[400];
    sprintf(jenisPath, "%s/%s", FULL_PATH, jenis);
    sprintf(txtPath, "%s/%s/keterangan.txt", FULL_PATH, jenis);
    sprintf(fileNamePath, "%s/%s/%s.jpg", FULL_PATH, jenis, nama);


    pid = fork();                                               //make directory based on jenis
    if(pid == 0) {
        char *args[] = {"mkdir", "-p", jenisPath, NULL};
        execv("/usr/bin/mkdir", args);
    }
    while(wait(NULL) != pid);

    pid = fork();                                               //copy file to the new file based on cathegory
    if(pid == 0) {
        char *args[] = {"cp", sourcePath, fileNamePath, NULL};
        execv("/usr/bin/cp", args);
    }
    while(wait(NULL) != pid);
    printf("NAMA HAYYO %s\n\n", fileNamePath);


    char content[200];                                       // make keterangan.txt files
    sprintf(content, "nama : %s \numur : %s\n\n", nama, umur);

    FILE *ketTxt;
    ketTxt = fopen(txtPath, "a");

    if(ketTxt) {
        fprintf(ketTxt, "%s", content);
        fclose(ketTxt);
    }
}

int main(){
    //unzip files
    unzipFile("/home/erki/pets.zip");

    //delete unnecessary folders
    removeNotJpg();

    //put photos based on their chategory
    char temp[400];
    strcpy(temp, FULL_PATH);
    strcat(temp, "/");
    struct dirent *dp;
    DIR *folder;
    folder = opendir(temp);

      removeNotJpg();

    if(folder != NULL){  //open directory
       while((dp = readdir(folder)) != NULL){  //read per names in the file
            if(strcmp(dp->d_name, ".")&& strcmp(dp->d_name, "..") ){ // dont copy parent and current directory
                 if(dp->d_type == DT_REG){
                      char sourcePath[400], fileName[400];
                      sprintf(sourcePath, "%s/%s", FULL_PATH, dp->d_name);
                      sprintf(fileName, "%s",  dp->d_name);
                      

                      int len = strlen(fileName);
                      fileName[len-4] = '\0';                       //remove .zip

                      char *firstPet = strtok(fileName, "_");
                      char *secondPet = strtok(NULL, "_");
                      if(secondPet != NULL){                         //if it has second pet 
                          categorize(sourcePath, secondPet);
                      }
                      categorize(sourcePath,firstPet);

                      pid_t pid;
                      pid = fork();
                      if(pid == 0){
                         char *argv[] = {"rm", "-rf", sourcePath, NULL};
                         execv("/usr/bin/rm", argv);  
                      }
                      while(wait(NULL) != pid);
                 }
            }
       }
   }
}