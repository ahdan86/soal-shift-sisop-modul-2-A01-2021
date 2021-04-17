#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <dirent.h>

void caesarCipher(char message[])
{
    char ch;
    int i;
    int key = 5;

    for(i = 0; message[i] != '\0'; ++i){
		ch = message[i];
		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;
			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}
			message[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;
			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}
			message[i] = ch;
		}
	}
}

void getTime(char namaFile[])
{
    time_t rawtime;
    char temp[80];
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(temp, 80, "%Y-%m-%d_%H:%M:%S", info);
    strcpy(namaFile, temp);
}

void makeDirectory(char str[])
{
    //Make Folder
    char *argv1[] = {"mkdir", "-m", "777", str, NULL};
    execv("/usr/bin/mkdir",argv1);
}

void downloadPicture(char str[])
{
    char changeDir[100];
    strcpy(changeDir,str);
    strcat(changeDir,"/");
    chdir(changeDir);

    printf("%s",changeDir);
    
    //Create Download Image Link
    char link[100];
    strcpy(link,"https://picsum.photos/");
    int size = ((int)time(NULL) % 1000)+ 50;                
    char epochTime[20];
    sprintf(epochTime,"%d",size);
    strcat(link, epochTime);
    
    pid_t child2;
    int count=0;
    while(count<10){
        child2 = fork();
            
        if(child2<0){
            exit(EXIT_FAILURE);
        }

        if(child2 == 0){
            char namaFile[100];
            getTime(namaFile);
            strcat(namaFile,".jpg");

            char *argv[] = {"wget", "-q", "-O", namaFile, link, NULL};
            execv("/usr/bin/wget", argv);
        }
        count++;
        if(count!=9) sleep(5);
    }
}

void checkSuccess()
{
    char checkDownload[20] = "Download Success";
    caesarCipher(checkDownload);
    FILE *new = fopen("status.txt","w");
    fputs(checkDownload, new);
    fclose(new);
}

void zipFolder(char namaTime[], char root[]){
    chdir(root);
    char *argv2[] = {"zip", "-m", "-q", "-r", namaTime, namaTime, NULL};
    execv("/usr/bin/zip",argv2);
}

void makeKillerZ(){
    //Generate Killer Program "-z" Argument
    char isiBashZ[100];
    // sprintf(isiBash,"#!/bin/sh\nrm -f killer.sh\nkill -9 %d",(int)getpid());
    strcpy(isiBashZ, "#!/bin/sh\nrm -f Killer.sh\npkill -f soal3");
    FILE *killer = fopen("Killer.sh","w");
    fputs(isiBashZ, killer);
    fclose(killer);
}

char isiBashX[100];
void makeKillerX(){
    // Generate Killer Program "-x" Argument;
    FILE *killer = fopen("Killer.sh","w");
    fputs(isiBashX, killer);
    fclose(killer);
}

void fungsiMain(int pilihan)
{
    if(pilihan == 0) {
        strcpy(isiBashX, "#!/bin/sh\nrm -f Killer.sh\nkill -9");
        int pid = (int)getpid();
        char strPid[10];
        sprintf(strPid, "%d", pid);
        strcat(isiBashX," ");
        strcat(isiBashX,strPid);
        makeKillerX();
    }
    else if(pilihan == 1) makeKillerZ();
    
    pid_t child1;

    while(1){
        
        //PathFolder
        char namaTime[50];
        getTime(namaTime);
        char str[100] = "/home/ahdan/Modul2/";

        //Path Awal
        char root[100];
        strcpy(root, str);
        
        strcat(str, namaTime);
        
        if((child1 = fork()) == 0){
            makeDirectory(str);
        }

        while(opendir(str) == NULL);

        if((child1 = fork()) == 0){
            downloadPicture(str);
            checkSuccess();
            zipFolder(namaTime,root);
        }
        sleep(40);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1){
        fungsiMain(1);
    }
    
    else if(argc == 2){
        if(!strcmp(argv[1],"-x")) fungsiMain(0);
        else if(!strcmp(argv[1],"-z")) fungsiMain(1);
    }
    
    else{
        printf("Argument amount is 1 or 0(default -z) only");
        return(0);
    }
}