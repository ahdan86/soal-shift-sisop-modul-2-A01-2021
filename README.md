# soal-shift-sisop-modul-2-A01-2021

## Soal Nomor 1

Pada suatu masa, hiduplah seorang Steven yang hidupnya pas-pasan. Steven punya pacar, namun sudah putus sebelum pacaran. Ketika dia galau memikirkan mantan, ia selalu menonton https://www.youtube.com/watch?v=568DH_9CMKI untuk menghilangkan kesedihannya.

Di lain hal Steven anak yang tidak amat sangat super membenci matkul sisop, beberapa jam setelah diputus oleh pacarnya dia menemukan wanita lain bernama Stevany, namun Stevany berkebalikan dengan Steven karena menyukai sisop. Steven ingin terlihat jago matkul sisop demi menarik perhatian Stevany.

Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi. (a) Dikarenakan Stevany sangat menyukai huruf Y, Steven ingin nama folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg (b) untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). (c) Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu meng-extract-nya setelah didownload serta (d) memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).

(e) Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany). (f) Setelah itu pada waktu ulang tahunnya Stevany, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip). Kemudian Steven meminta bantuanmu yang memang sudah jago sisop untuk membantunya mendapatkan hati Stevany. Bantu Woy!!

### Penjelasan

Pada soal, dinyatakan bahwa program akan berjalan otomatis sehingga program akan dibuat dalam bentuk daemon. Terdapat 2 prosedur utama dalam program ini.

Pertama adalah membuat 3 folder sesuai format, mendownload file-file dari 3 link yang berbeda, meng-unzip file yang telah didownload, memindahkannya ke dalam folder yang dilakukan 6 jam sebelum hari ulang tahun Stevany yang jatuh pada tanggal 9 April pukul 22:22 yaitu pukul 16:22.

Kedua adalah melakukan zip kepada 3 folder yang telah diisi oleh file yang diperlukan dan menghapus folder yang tidak diperlukan. Prosedur ini dilakukan tepat pada hari ulang tahun Stevany.

### Solusi

Untuk representasi daemon, digunakan program sebagai berikut

```c
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
          if(isSixHours && !isMade){
                isMade = 1;
                ....
          }
          if(isBirthday && !isZipped){
                isZipped = 1;
                ....
          }
    }
```

Variabel isMade dan isZipped digunakan agar tidak melakukan hal yang sama lebih dari sekali pada saat program berjalan secara otomatis. Variabel loop digunakan untuk melakukan debug penghentian program apabila waktu diatur pada jam tertentu. Untuk penentuan waktu, diperlukan sebuah `struct tm *` agar dapat melakukan pengesktrakan dari waktu yang ada. Untuk soal 1a-1d akan dijalankan di dalam `if (isSixHours && !isMade)` dan untuk soal 1e-1f akan dijalankan didalam ` if (isBirthday && !isZipped)`.

### No 1a

Untuk soal ini, dilakukan pembuatan 3 buah folder menggunakan fork dengan execv nama folder yang berbeda-beda menggunakan format nama tertentu.

```c
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
```

### No 1b

Untuk soal ini, program harus mendownload file foto, video, dan film dari link yang telah disediakan.

```c
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
```

Untuk melakukannya, dilakukan pengulangan sebanyak 3 kali. Untuk setiap pengulangannya, link download file akan diset berbeda-beda. Terakhir, dilakukan `sleep(15)` untuk setiap pengulangan.

### No 1C

Untuk soal ini, program harus mengextract file yang telah didownload. Langkah ini dilakukan dengan program sebagai berikut

```c
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
```

Untuk penamaan file, disesuaikan dengan jenis file yang didownload.

### No 1D

Untuk soal ini, program harus memindahkan file dari file yang telah diextract ke dalam file yang telah dibuat di soal 1a. Untuk melakukan ini, diperlukan sebuah pengulangan untuk melakukan read setiap file pada folder hasil extract dan memindahkannya ke dalam file yang baru.

```c
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
```

Untuk melakukan hal ini, diperlukan bantuan `struct dirent *` dan `DIR *` untuk melakukan read pada directory dan folder yang terdapat di dalamnya.

### No 1E

Untuk soal ini, program harus menzip folder yang telah diisi oleh file yang ada di folder extract.

```c
child_id = fork();
if(child_id == 0){       //zip all the folders
    char *argv[] = {"zip", "-r", "Lopyu_Stevany.zip", "Fylm", "Musyik", "Pyoto", NULL};
    execv("/usr/bin/zip", argv);
}
while (wait(NULL) != child_id);
```

`zip` menggunakan option `-r` untuk dapat melakukan `zip` directory beserta semua isi di dalamnya.

### No 1F

Untuk soal terakhir ini, program akan menghapus semua folder yang telah dibuat hingga hanya menyisakan folder zip dari hasil mendownload file dari link yang telah disediakan.

```c
child_id = fork();
if(child_id == 0){       //delete all unnecessary folders
    char *argv[] = {"rm", "-r", "Fylm", "Pyoto", "Musyik", "MUSIK", "FILM", "FOTO", NULL};
    execv("/usr/bin/rm", argv);
}
while (wait(NULL) != child_id);
```

option `-r` digunakan untuk menghapus directory bersama dengan semua file yang terdapat di dalamnya.
