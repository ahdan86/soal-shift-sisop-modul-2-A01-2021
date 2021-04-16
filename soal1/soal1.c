#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <dirent.h>

int main(){
    pid_t pid, sid;
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/home/erki/Documents/modul2_no2/")) < 0) {
        exit(EXIT_FAILURE);     
       }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int counter = 1;
    int isZipped = 0;
    int isMade = 0;
    int loop = 1;
    while(loop){
         pid_t child_id;

         time_t t = time(NULL);          //to set up time
         struct tm *tm = localtime(&t);
         int isSixHours = 0;
         int isBirthday = 0;
         int date = tm->tm_mday;
         int month = tm->tm_mon + 1;
         int hour = tm->tm_hour;
         int minute = tm->tm_min;
         int second = tm->tm_sec;

         if(date == 9 && month == 4){
             if(hour == 16 && minute == 22)     //6 hours condition
                  isSixHours = 1;
             else if ( hour == 22 && minute == 22)  //birthday time
                  isBirthday = 1;
             else if ( hour == 10 && minute == 22)    //debug for stopping the daemon
                  loop = 0;
         }

     //     printf ("%d %d - %d %d %d\n", date, month, hour, minute, second);  //debug

         if(isSixHours && !isMade){      //condition if it's 6 hours before birthday
             isMade = 1;
             child_id = fork();
             if(child_id == 0){                               //make directory
                   char *argv[] = {"mkdir",  "Musyik", NULL};
                   execv("/usr/bin/mkdir", argv);
             }
             while(wait(NULL) != child_id);

             child_id = fork();
             if(child_id == 0){
                   char *argv[] = {"mkdir",  "Fylm", NULL};
                   execv("/usr/bin/mkdir", argv);
             }
             while(wait(NULL) != child_id);

             child_id = fork();
             if(child_id == 0){
                   char *argv[] = {"mkdir",  "Pyoto", NULL};
                   execv("/usr/bin/mkdir", argv);
             }
             while(wait(NULL) != child_id);

             for(int count=1 ;count<=3;count++){             //download files
                  child_id = fork();
                  if(child_id == 0){
                       char directory[100], link[100], nameFile[100];
                       if(count==1){
                            strcpy(link, "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download");
                            strcpy(nameFile, "Musik_for_Stevany.zip");
                       }if(count==2){
                            strcpy(link, "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download");
                            strcpy(nameFile, "Film_for_Stevany.zip");
                       }if(count==3){
                            strcpy(link, "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download");
                            strcpy(nameFile, "Foto_for_Stevany.zip");
                       }
                       char *argv[] = {"wget", "-q", link, "-O", nameFile, NULL};
                    //    char *argv[] = {"wget",  link, "-O", nameFile, NULL};
                       execv("/usr/bin/wget", argv);  
                  } 
                  sleep(15);
             }

             while(wait(NULL) != child_id);
             for(int count=1 ;count<=3;count++){           //unzip folder
                  child_id = fork();
                  if(child_id == 0){
                       char directory[100], link[100], nameFile[100];
                       if(count==1){
                            strcpy(nameFile, "Musik_for_Stevany.zip");
                       }if(count==2){
                            strcpy(nameFile, "Film_for_Stevany.zip");
                       }if(count==3){
                            strcpy(nameFile, "Foto_for_Stevany.zip");
                       }
                       char *argv[] = {"unzip", nameFile, NULL};
                       execv("/usr/bin/unzip", argv);
                  } 
                  sleep(2);
             }

             while(wait(NULL) != child_id);
             for(int count=1 ;count<=3;count++){          //copy contents that unzipped
                  DIR *dp;
                  struct dirent* ep;

                  char dest[100], source[100], nameFile[100];
                  if(count==1){
                         strcpy(source, "/home/erki/Documents/modul2_no2/MUSIK/");
                         strcpy(dest, "/home/erki/Documents/modul2_no2/Musyik/");
                  }if(count==2){
                         strcpy(source, "/home/erki/Documents/modul2_no2/FILM/");
                         strcpy(dest, "/home/erki/Documents/modul2_no2/Fylm/");
                  }if(count==3){
                         strcpy(source, "/home/erki/Documents/modul2_no2/FOTO/");
                         strcpy(dest, "/home/erki/Documents/modul2_no2/Pyoto/");
                  }
                  dp = opendir(source);
                  if(dp != NULL){
                       while((ep = readdir(dp))){
                         child_id = fork();
                         if(child_id == 0){
                              // printf("DEBUG %s\n", ep->d_name); // debug print file
                              char file_source[355];
                              char file_dest[355];
                              sprintf(file_source, "%s%s", source, ep->d_name);
                              sprintf(file_dest, "%s%s", dest, ep->d_name);

                              if(strcmp(ep->d_name, ".") != 0 || strcmp(ep->d_name, "..") != 0){ //dont print current and parrent directory
                                   char *argv[] = {"mv", file_source, file_dest, NULL};
                                   execv("/usr/bin/mv", argv);  
                              }
                         }
                       }
                  }
             }
         }

         if(isBirthday && !isZipped){   //condition if it's birthday time
               isZipped = 1;
               child_id = fork();
               if(child_id == 0){       //zip all the folders
                    char *argv[] = {"zip", "-r", "Lopyu_Stevany.zip", "Fylm", "Musyik", "Pyoto", NULL};
                    execv("/usr/bin/zip", argv);
               }
               while (wait(NULL) != child_id);

               child_id = fork();
               if(child_id == 0){       //delete all unnecessary folders
                    char *argv[] = {"rm", "-r", "Fylm", "Pyoto", "Musyik", "MUSIK", "FILM", "FOTO", NULL};
                    execv("/usr/bin/rm", argv);
               }
               while (wait(NULL) != child_id);
         }
         counter++;
         sleep(2);
    } 
}
