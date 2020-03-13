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
        if (fork() == 0)
        {
          char folder_name_zip[100];
          sprintf(folder_name_zip, "%s.zip", folder_name);
          char *argv[] = {"zip", "-qrm", folder_name_zip, folder_name, NULL};
          execv("/usr/bin/zip", argv);
        }
        exit(0);
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