# SoalShiftSISOP20_modul2_E04

Soal Shift Sistem Operasi 2020\
Modul 2\
Kelompok E 04

## 1. Program C yang Menyerupai Crontab
>Source code file: [Soal1](https://github.com/segara2410/SoalShiftSISOP20_modul2_E04/tree/master/soal1)

- Menggunakan template daemon yang ada di modul 2.
- Pengecekan argument dilakukan menggunakan
  ``` c 
  if(argc == 5)
  {
    int arg[4];
    for (int i = 1; i < 4; i++)
    {
      if (isNumber(argv[i]))
      {
        arg[i] = atoi(argv[i]);
        if(arg[i] < 0)
          printf("argument %d is not valid\n", i);
      }
      else if (isAsterisk(argv[i]))
        arg[i] = -1;
      else
      {
        printf("argument %d is not valid\n", i);
        exit(EXIT_FAILURE);
      }
    }

    if (arg[1] > 59 || arg[2] > 59 || arg[3] > 23)
    {
      printf("argument is not valid\n");
      exit(EXIT_FAILURE);
    }
    ...
  }
  else
  {
    printf("argument is not valid!\n");
    exit(EXIT_FAILURE);
  }
  ```
  dengan ketentuan argumen harus 5, bisa berupa angka (dicek oleh `isNumber()`) atau asterisk (dicek oleh `isAsterisk()`).
  Argumen yang dimasukkan harus sesuai dengan timetamp detik(0-59), menit(0-59), dan jam (0-24).

- Proses atau loop utama yang menjalankan argumen ke empat :
  ``` c
  while (1) 
  {
    time_t timer;
    struct tm* tm_info;  

    timer = time(NULL);
    tm_info = localtime(&timer);

    if ((tm_info->tm_sec == arg[1] || arg[1] == -1) && (tm_info->tm_min == arg[2] || arg[2] == -1) 
      && (tm_info->tm_hour == arg[3] || arg[3] == -1))
    {
      pid_t child_id;
      child_id = fork();

      int status;

      if (child_id == 0)
      {    
        char *argexec[] = {"bash", argv[4], NULL};
        execv("/bin/bash", argexec);
      }
    }

    sleep(1);
  }
  ```
  dengan ketentuan program akan mengecek waktu dan membuat child yang akan menjalankan argumen keempat jika argumen - argumen sebelumnya sudah sama dengan waktu sekarang.
  - Program menggunakan fungsi dan struct dari time.h untuk proses-proses yang berkaitan dengan waktu.
  - Pengencekan waktu :
    ```
    if ((tm_info->tm_sec == arg[1] || arg[1] == -1) && (tm_info->tm_min == arg[2] || arg[2] == -1) 
      && (tm_info->tm_hour == arg[3] || arg[3] == -1))
    ```
  - Child menjalankan bash :
    ```
    char *argexec[] = {"bash", argv[4], NULL}; 
    execv("/bin/bash", argexec);
    ```

## 2. Program C yang Mendownload dan Menzip Kumpulan Gambar
>Source code file: [Soal2](https://github.com/segara2410/SoalShiftSISOP20_modul2_E04/tree/master/soal2)

- Menggunakan template daemon yang ada di modul 2.
- Proses atau loop utama yang melakukan download dan zip :
  ```
  while (1) 
  {
    timer = time(NULL);
    tm_info = localtime(&timer);

    char folder_name[100];
    strftime(folder_name, 100, "%Y-%m-%d_%H:%M:%S", tm_info);

    pid_t child_id;
    child_id = fork();

    int status;

    if (child_id < 0) 
      exit(EXIT_FAILURE);

    if (child_id == 0)
    { 
      if (fork() == 0)
      {
        char *argv[] = {"mkdir", "-p", folder_name, NULL};
        execv("/bin/mkdir", argv);
      }
      else 
      {
        while ((wait(&status)) > 0);
        for (int i = 0; i < 20; i++)
        {
          if (fork() == 0)
          {
            chdir(folder_name);
            time_t file_timer;
            struct tm* file_tm_info;
        
            file_timer = time(NULL);
            file_tm_info = localtime(&file_timer);
        
            int t = (int)time(NULL);
            t = (t % 1000) + 100;
            
            char target[100];
            sprintf(target, "https://picsum.photos/%d", t);

            char file_name[100];
            strftime(file_name, 100, "%Y-%m-%d_%H:%M:%S", file_tm_info);
            char *argv[] = {"wget", target, "-qO", file_name, NULL};
            execv("/usr/bin/wget", argv);
          }
          sleep(5);
        }
        char folder_name_zip[100];
        sprintf(folder_name_zip, "%s.zip", folder_name);
        char *argv[] = {"zip", "-qrm", folder_name_zip, folder_name, NULL};
        execv("/usr/bin/zip", argv);
      }
    }
    else
      sleep(30);
      
  }
  ```
  - Program menggunakan fungsi dan struct dari time.h untuk proses-proses yang berkaitan dengan waktu.
  - Program membuat child yang akan menjalankan proses dari pembuatan folder hingga zip
    ```
    if (child_id == 0)
    { 
      if (fork() == 0)
      {
        char *argv[] = {"mkdir", "-p", folder_name, NULL};
        execv("/bin/mkdir", argv);
      }
      else 
      {
        while ((wait(&status)) > 0);
        for (int i = 0; i < 20; i++)
        {
          if (fork() == 0)
          {
            chdir(folder_name);
            time_t file_timer;
            struct tm* file_tm_info;
        
            file_timer = time(NULL);
            file_tm_info = localtime(&file_timer);
        
            int t = (int)time(NULL);
            t = (t % 1000) + 100;
            
            char target[100];
            sprintf(target, "https://picsum.photos/%d", t);

            char file_name[100];
            strftime(file_name, 100, "%Y-%m-%d_%H:%M:%S", file_tm_info);
            char *argv[] = {"wget", target, "-qO", file_name, NULL};
            execv("/usr/bin/wget", argv);
          }
          sleep(5);
        }
        char folder_name_zip[100];
        sprintf(folder_name_zip, "%s.zip", folder_name);
        char *argv[] = {"zip", "-qrm", folder_name_zip, folder_name, NULL};
        execv("/usr/bin/zip", argv);
      }
    }
    ```
    - Child kemudian membuat child lagi untuk membuat folder.
    - Setelah pembuatan folder selesai, Proses Child ini melakukan loop sebanyak 20 kali untuk membuat child yang mendownload gambar dengan ukuran epoch unix % 1000 + 100 pixel.
    - Setelah semua download selesai, Child ini melakukan zip -rm yang akan sekaligus menghapus folder yang akan dizip.
  - Program utama akan berhenti selama 30 detik sebelum proses selanjutnya.
- Pembuatan Killer executable :
  ```
  void generateKiller(char source[])
  {
    FILE *target;
    target = fopen("killer.sh", "w");
    int status;

    if (strcmp(source, "-a") == 0)
      fprintf(target, "#!/bin/bash\nkill -9 -%d", getpid());

    if (strcmp(source, "-b") == 0)
      fprintf(target, "#!/bin/bash\nkill %d", getpid());
    
    if(fork() == 0)
    {  
      if (fork() == 0)
      {
        char *argv[] = {"chmod", "u+x", "killer.sh", NULL};
        execv("/bin/chmod", argv);
      }
      else
      {
        while ((wait(&status)) > 0);    

        char *argv[] = {"mv", "killer.sh", "killer", NULL};
        execv("/bin/mv", argv);
      }
    }
      
    fclose(target);
  }
  ```
  - Untuk mode A killer akan membunuh semua proses (termasuk child) dengan minus pid, 
    sedangkan untuk mode B, killer hanya akan membunuh parent agar tidak membuat child baru.
  - killer dibuat menggunakan bash yang dijadikan executable dengan `chmod u+x killer.sh` lalu direname menjadi `killer`.

## 3. Program C Untuk Mengekstrak Folder dan File
>Source code file: [Soal2](https://github.com/segara2410/SoalShiftSISOP20_modul2_E04/tree/master/soal3)

- Pertama, pindahkan direktori dengan
  ```
  chdir("/home/segara/modul2"); 
  ```
- Buat child, untuk menjalankan setiap proses. Dalam hal ini, fork() dilakukan dua kali untuk membuat tiga child.
  Child ketiga tidak digunakan karena berjalan bersamaan dengan child kedua.
- Proses yang berjalan pertama adalah Child kedua yang melakukan unzip
  ```
  else if (n1 > 0 && n2 == 0) 
  { 
    char* argexec[] = {"unzip", "-oq", 
      "/home/segara/modul2/jpg.zip", NULL};
    execv("/usr/bin/unzip", argexec);
  }
  ```
- Child pertama akan berjalan setelah Child kedua
  ```
  else if (n1 == 0 && n2 > 0) 
  { 
    while ((wait(&status)) > 0);
    pid_t child = fork();
    if (child == 0) 
    {
      char *argexec[] = {"mkdir", "-p", 
        "/home/segara/modul2/indomie", NULL};
      execv("/bin/mkdir", argexec);
    }
    else
    {
      while ((wait(&status)) > 0);
      sleep(5);

      char *argexec[] = {"mkdir", "-p", 
        "/home/segara/modul2/sedaap", NULL};
      execv("/bin/mkdir", argexec);
    }
  } 
  ```
  - Proses ini kemudian membuat child yang membuat folder indomie.
  - Setelah pembuatan folder indomie selesai, proses ini akan membuat file sedaap.
- Kemudian parent berjalan setelah child pertama selesai
  ```
  if (n1 > 0 && n2 > 0) 
  { 
    while ((wait(&status)) > 0);

    DIR *d;
    struct dirent *dir;
    chdir("/home/segara/modul2/jpg/"); 
    d = opendir(".");
    if (d)
    {
      while ((dir = readdir(d)) != NULL)
      {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
          continue;

        if(isDirectory(dir->d_name))
        {
          if(fork() == 0)
          {
            char source_file[1000];
            sprintf(source_file, 
              "/home/segara/modul2/jpg/%s", 
              dir->d_name);

            char* argexec[] = {"mv", source_file,
              "/home/segara/modul2/indomie/", NULL};
            execv("/bin/mv", argexec);
          }
          else
          {
            while ((wait(&status)) > 0);
            
            if(fork() == 0)
            {
              if(fork() == 0)
              {
                char target_file[1000];
                FILE *target;
                sprintf(target_file, 
                  "/home/segara/modul2/indomie/%s/coba1.txt", 
                  dir->d_name);
                target = fopen(target_file, "w");
                fclose(target);

                sleep(3);
                
                FILE *second_target;
                sprintf(target_file, 
                  "/home/segara/modul2/indomie/%s/coba2.txt", 
                  dir->d_name);
                second_target = fopen(target_file, "w");
                fclose(second_target);
                exit(0);
              }
            }
            else
            {
              while ((wait(&status)) > 0);
              exit(0);
            }
          }
        }
        else
        {
          while ((wait(&status)) > 0);
          if(fork() == 0)
          {
            while ((wait(&status)) > 0);

            char source_file[1000];
            sprintf(source_file, 
              "/home/segara/modul2/jpg/%s", 
              dir->d_name);

            char* argexec[] = {"mv", source_file,
              "/home/segara/modul2/sedaap/", NULL};
            execv("/bin/mv", argexec);
          }
        }        
      }
    }
  }
  ```
  - Dalam parent akan dilakukan directory traverse pada jpg menggunakan fungsi dan struct dari dirent.h.
  - Dalam loop akan dilakukan pengecekan apakah isi directory tersebut merupakan file atau folder menggunakan fungsi isDirectory()
  ```
  int isDirectory(const char *path)
  {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
  }
  ```
  - Kemudian jika objek tersebut adalah folder, proses ini akan membuat child untuk memindahkan folder, kemudian proses ini membuat coba1.txt, lalu setelah 3 detik membuat coba2.txt.
  - Jika objek tersebut adalah file, proses ini akan membuat child untuk memindahkan ke sedaap. 
  - setelah selesai, proses ini melakukan hal yang sama untuk objek lainnya.
