#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

void listFilesRecursively(char *path);
void unzipfile() {
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); 
  }

  if (child_id == 0) {
    char *argv[] = {"mkdir", "/home/valda/Documents/Modul2/petshop", NULL};
    execv("/bin/mkdir", argv);
  } 
  else {
     char argv[] = {"unzip", "/home/valda/Documents/Modul2/pets.zip", ".jpg", "-d","/home/valda/Documents/Modul2/petshop", NULL};
     execv("/bin/unzip", argv);
  }
}

void ini_exec(char a[], char *arg[]){
    pid_t child_id;
    int status;
    child_id = fork();
    if (child_id == 0){
        execv (a, arg);
    }
    else {
        ((wait(&status))>0);
    }

}

void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            char file[100]= "";
            strcpy(file,dp->d_name);
            strtok(file, ";");
            //printf("%s\n", namafile);
            char namafile[100]= "/home/valda/Documents/Modul2/petshop/";
            strcat(namafile,file);
            char *argv[] = {"mkdir", "-p", namafile, NULL};
            ini_exec("/bin/mkdir", argv);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
}

int main()
{

    pid_t child_id;

    child_id = fork();

    //2a
    if(child_id != 0){
    unzipfile();

    //2b
    }else {
    char path[100] = "/home/valda/Documents/Modul2/petshop/";
    listFilesRecursively(path);
    }

  return 0;

}
